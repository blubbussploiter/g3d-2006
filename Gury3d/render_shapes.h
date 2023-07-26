#ifndef RENDER_SHAPES_H
#define RENDER_SHAPES_H

#include <G3DAll.h>
#include <vector>

#include "decal.h"
#include "runservice.h"

#include "instance.h"
#include "brickcolor.h"

#include "extents.h"
#include "joints.h"

namespace RBX
{

	class PVInstance;
	typedef std::vector<RBX::PVInstance*>* Touched;

	class PhysBody
	{
	public:
		int tag;
		bool isColliding;
		btRigidBody* _body;
		btCollisionObject* _collider;
		btCollisionShape* _shape;
		RBX::Joint* connector;
		Touched touching;
		PhysBody()
		{
			touching = new std::vector<RBX::PVInstance*>();
		}
	};

	class Renderable : public RBX::Instance
	{
	public:
		virtual void render(RenderDevice* rd) = 0;
		Renderable() { isRenderable = 1; }
		virtual ~Renderable() {}
	};

	class Surface
	{
	private:
		RBX::PVInstance* parent;
		SurfaceType surface;
		Decal* decal;
	public:
		Decal* getDecal();

		std::vector<Vector2> getTexCoords(NormalId f, Vector3 sz, bool isDrawingDecal=0);
		SurfaceType getSurfaceType() { return surface; }
		
		void setSurfaceType(SurfaceType srfc);
		CoordinateFrame getSurfaceCenter();

		Surface(SurfaceType surface, RBX::PVInstance* parent) : surface(surface), parent(parent) { }
	};

	class PVInstance : public Renderable
	{
	private:
		float elasticity, friction, erp, cfm;
	public:

		Vector3 size;
		Color3 color;

		CoordinateFrame cframe;

		Vector3 velocity;
		Vector3 rotVelocity;

		Surface* top;
		Surface* bottom;
		Surface* front;
		Surface* back;
		Surface* left;
		Surface* right;

		FormFactor formFactor;

		bool showControllerFlag;
		bool anchored;
		bool locked;

		float transparency;
		int shape;

	public:
		PhysBody* body;

		static Reflection::PropertyDescriptor<PVInstance, Vector3> prop_sz;
		static Reflection::PropertyDescriptor<PVInstance, Vector3> prop_pos;
		static Reflection::PropertyDescriptor<PVInstance, CoordinateFrame> prop_cframe;
		static Reflection::PropertyDescriptor<PVInstance, Color3> prop_color3;
		static Reflection::PropertyDescriptor<PVInstance, int> prop_shape;
		static Reflection::PropertyDescriptor<PVInstance, bool> prop_anchored;
		static Reflection::PropertyDescriptor<PVInstance, bool> prop_locked;
		static Reflection::PropertyDescriptor<PVInstance, float> prop_transparency;
		static Reflection::PropertyDescriptor<PVInstance, int> prop_frontSurface;
		static Reflection::PropertyDescriptor<PVInstance, int> prop_backSurface;
		static Reflection::PropertyDescriptor<PVInstance, int> prop_rightSurface;
		static Reflection::PropertyDescriptor<PVInstance, int> prop_leftSurface;
		static Reflection::PropertyDescriptor<PVInstance, int> prop_topSurface;
		static Reflection::PropertyDescriptor<PVInstance, int> prop_bottomSurface;
		static Reflection::PropertyDescriptor<PVInstance, int> prop_rawFormFactor;
		static Reflection::PropertyDescriptor<PVInstance, int>
			prop_formFactor;

		void setShape(int s) { shape = s; }
		int getShape() { return shape; }

		int getFrontSurface() { return front->getSurfaceType(); }
		void setFrontSurface(int s) { setFace(FRONT, (RBX::SurfaceType)s); }

		int getBackSurface() { return back->getSurfaceType(); }
		void setBackSurface(int s) { setFace(BACK, (RBX::SurfaceType)s); }

		int getRightSurface() { return right->getSurfaceType(); }
		void setRightSurface(int s) { setFace(RIGHT, (RBX::SurfaceType)s); }

		int getLeftSurface() { return left->getSurfaceType(); }
		void setLeftSurface(int s) { setFace(LEFT, (RBX::SurfaceType)s); }

		int getTopSurface() { return top->getSurfaceType(); }
		void setTopSurface(int s) { setFace(TOP, (RBX::SurfaceType)s); }

		int getBottomSurface() { return bottom->getSurfaceType(); }
		void setBottomSurface(int s) { setFace(BOTTOM, (RBX::SurfaceType)s); }

		FormFactor getFormFactor() { return formFactor; }
		void setFormFactor(int f) { formFactor = (FormFactor)f; }

		bool getShowControllerFlag() { return showControllerFlag; }
		void setShowControllerFlag(bool scf) { showControllerFlag = scf; }

		void setFace(NormalId f, SurfaceType s);

		virtual void render(RenderDevice* rd);
		virtual void renderFace(NormalId face, bool isAlpha = 0, bool isDrawingDecal=0);
		void think(RBX::PVInstance* part);
		
		void render3dSurface(RenderDevice* d, NormalId face);
		void renderSurface(NormalId face);
		Vector3 getSize() { return size; }
		void setSize(Vector3 s) { size = s; }
		Vector3 getPosition() { return cframe.translation; }
		
		void setPosition(Vector3 p) { setCFrame(CoordinateFrame(cframe.rotation, p)); }
		CoordinateFrame getCFrame() { return cframe; }

		void setCFrameNoPhysics(CoordinateFrame cf) { cframe = cf; }
		void setCFrame(CoordinateFrame cf) {cframe = cf; RBX::RunService::singleton()->getPhysics()->updateBodyCFrame(cframe, this);}
		
		Color3 getColor() { return color; }
		void setColor(Color3 c) { color = c; }
		bool getAnchored() { return anchored; }
		bool getLocked() { return locked; }

		void setAnchored(bool a) { anchored = a; RBX::RunService::singleton()->getPhysics()->updateAnchor(this);
		}
		Surface* getSurface(NormalId face);
		float getTransparency() { return transparency; }
		void setTransparency(float f) { transparency = f; }
		
		Vector3 getVelocity() { return velocity; }
		Vector3 getRotVelocity() { return rotVelocity; }
		Vector3 getEulerRotation() { float x, y, z; cframe.rotation.toEulerAnglesXYZ(x, y, z); return Vector3(x, y, z); }
		void setEulerRotation(float x, float y, float z) { setCFrame(CoordinateFrame(Matrix3::fromEulerAnglesXYZ(x, y, z), getCFrame().translation)); }
		void lookAt(Vector3 v)
		{
			CoordinateFrame cf;
			cf = getCFrame();
			cf.lookAt(v);
			setCFrame(cf);
		}

		void setLocked(bool l) { locked = l; }

		Box getBox() {
			Box box = Box(Vector3(size.x / 2.f, size.y / 2.4f, size.z / 2.f), Vector3(-size.x / 2.f, -size.y / 2.4f, -size.z / 2.f));
			CoordinateFrame c = CoordinateFrame(getCFrame().rotation, getPosition());
			return c.toWorldSpace(box); 
		}

		Instance* clone() const {
			RBX::PVInstance* i = new PVInstance(*this);
			i->body = new PhysBody();
			RBX::RunService::singleton()->getPhysics()->createBody(i);
			return i;
		}

		Extents getLocalExtents() { return Extents(-getSize() / 2, getSize() / 2); }
		Extents getWorldExtents()
		{ 
			Extents localExtents = getLocalExtents();
			return localExtents.toWorldSpace(getCFrame());
		}

		float getElasticity() { return elasticity; }
		void setElasticity(float el) { elasticity = el; }
		float getFriction() { return friction; }
		void setFriction(float f) { friction = f; }
		float getERP() { return erp; }
		float getCFM() { return cfm; }
		void setCFM(float _cfm) { cfm = _cfm; }
		void setERP(float _erp) { erp = _erp; }

		PVInstance()
		{
			cframe = CoordinateFrame(Vector3(0.f, 0.f, 0.f));
			size = Vector3(4.f, 1.2f, 2.f);
			color = Color3(0.639216f, 0.635294f, 0.643137f);

			setClassName("PVInstance");
			setName("PVInstance");

			isAffectedByPhysics = 1;

			top = new Surface(SurfaceType::Studs, this);
			bottom = new Surface(SurfaceType::Inlet, this);
			front = new Surface(SurfaceType::Smooth, this);
			back = new Surface(SurfaceType::Smooth, this);
			left = new Surface(SurfaceType::Smooth, this);
			right = new Surface(SurfaceType::Smooth, this);

			top->getDecal()->setFace(NormalId::TOP);
			bottom->getDecal()->setFace(NormalId::BOTTOM);
			front->getDecal()->setFace(NormalId::FRONT);
			back->getDecal()->setFace(NormalId::BACK);
			left->getDecal()->setFace(NormalId::LEFT);
			right->getDecal()->setFace(NormalId::RIGHT);

			elasticity = 0.5f;
			friction = 0.300000012f;

			setFormFactor(FormFactor::Symmetric);

			shape = part;
			body = new PhysBody();
		}
		virtual ~PVInstance() { RBX::RunService::singleton()->getPhysics()->removeBody(this); }
	};

	namespace Primitives
	{
		extern void rawCylinderAlongX(Color4 color, float radius, float axis);
		extern void drawBall(RenderDevice* d, RBX::PVInstance* base);
		extern void drawCylinder(RenderDevice* d, RBX::PVInstance* base);
	}

	static bool getAxisOnUpVector(RBX::PVInstance* p)
	{
		return p->getCFrame().upVector().y >= 1;
	}

	static float getAffectedFormFactor(RBX::PVInstance* pv)
	{
		switch (pv->getFormFactor())
		{
		case RBX::Brick:
		{
			return 1.2f;
		}
		case RBX::Plate:
		{
			return 0.4f;
		}
		case RBX::Symmetric:
		{
			return 1.0f;
		}
		}
	}
}

#endif