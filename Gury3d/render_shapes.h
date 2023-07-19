#ifndef RENDER_SHAPES_H
#define RENDER_SHAPES_H

#include <G3DAll.h>
#include <vector>

#include "decal.h"
#include "runservice.h"

#include "instance.h"
#include "brickcolor.h"

namespace RBX
{

	class PVInstance;

	enum SURFACES
	{
		Smooth,
		Weld,
		Glue,
		Studs,
		Inlet,
	};

	enum shape
	{
		ball,
		part,
		cylinder
	};

	class Renderable : public RBX::Instance
	{
	public:
		virtual void render(RenderDevice* rd) = 0;
		Renderable() { isRenderable = 1; }
		virtual ~Renderable() {}
	};

	class PhysBody
	{
	public:
		dBodyID body;
		dGeomID geom[1];
		dMass* mass;
		PVInstance* touchedPV;
	};

	class Surface
	{
	private:
		SURFACES surface;
		Decal* decal;
	public:
		Decal* getDecal();

		std::vector<Vector2> getTexCoords(FACES f, Vector3 sz, bool isDrawingDecal=0);
		SURFACES getSurfaceType() { return surface; }
		void setSurfaceType(SURFACES srfc);

		Surface(SURFACES surface) : surface(surface) { }
	};

	class PVInstance : public Renderable
	{
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

		bool anchored;
		bool locked;
		float transparency;
		int shape;

	public:
		PhysBody* body;
		virtual void render(RenderDevice* rd);
		virtual void renderFace(FACES face, bool isAlpha = 0, bool isDrawingDecal=0);
		void think();
		void setBody(dBodyID d) { body->body = d; }
		void renderSurface(FACES face);
		Vector3 getSize() { return size; }
		void setSize(Vector3 s) { size = s; }
		Vector3 getPosition() { return cframe.translation; }
		void setPosition(Vector3 p) { setCFrame(CoordinateFrame(cframe.rotation, p)); }
		CoordinateFrame getCFrame() { return cframe; }
		void setCFrameNoPhysics(CoordinateFrame cf) { cframe = cf; }
		void setCFrame(CoordinateFrame cf) { cframe = cf; RBX::RunService::singleton()->getPhysics()->updateBodyCFrame(cframe, this);}
		Color3 getColor() { return color; }
		void setColor(Color3 c) { color = c; }
		bool getAnchored() { return anchored; }
		bool getLocked() { return locked; }
		void setAnchored(bool a) { anchored = a;}
		Surface* getSurface(FACES face);
		float getTransparency() { return transparency; }
		void setTransparency(float f) { transparency = f; }
		Vector3 getVelocity() { return velocity; }
		Vector3 getRotVelocity() { return rotVelocity; }
		Vector3 getEulerRotation() { float x, y, z; cframe.rotation.toEulerAnglesXYZ(x, y, z); return Vector3(x, y, z); }
		void setEulerRotation(float x, float y, float z) { cframe.rotation = Matrix3::fromEulerAnglesXYZ(x, y, z); RBX::RunService::singleton()->getPhysics()->updateBodyCFrame(cframe, this); }
		void lookAt(Vector3 v) {
			cframe.lookAt(v); RBX::RunService::singleton()->getPhysics()->updateBodyCFrame(cframe, this);
		}
		void setLocked(bool l) { locked = l; }
		Box getBox() {
			Box box = Box(Vector3(size.x / 2, size.y / 2.4, size.z / 2), Vector3(-size.x / 2, -size.y / 2.4, -size.z / 2));
			CoordinateFrame c = CoordinateFrame(getCFrame().rotation, getPosition());
			return c.toWorldSpace(box); 
		}
		Instance* clone() const {
			RBX::PVInstance* i = new PVInstance(*this);
			i->body = new PhysBody();
			RBX::RunService::singleton()->getPhysics()->createBody(i);
			return i;
		}
		PVInstance() :
			cframe(), // will initialize identity cframe by default
			size(4.0f, 1.2f, 2.0f),
			color(0.639216f, 0.635294f, 0.643137f)
		{
			setClassName("PVInstance");
			setName("PVInstance");

			isAffectedByPhysics = 1;

			top = new Surface(SURFACES::Studs);
			bottom = new Surface(SURFACES::Inlet);
			front = new Surface(SURFACES::Smooth);
			back = new Surface(SURFACES::Smooth);
			left = new Surface(SURFACES::Smooth);
			right = new Surface(SURFACES::Smooth);

			top->getDecal()->setFace(FACES::TOP);
			bottom->getDecal()->setFace(FACES::BOTTOM);
			front->getDecal()->setFace(FACES::FRONT);
			back->getDecal()->setFace(FACES::BACK);
			left->getDecal()->setFace(FACES::LEFT);
			right->getDecal()->setFace(FACES::RIGHT);

			shape = part;
			body = new PhysBody();
		}
		virtual ~PVInstance() { RBX::RunService::singleton()->getPhysics()->removeBody(this); }
	};

	static bool getAxisOnUpVector(RBX::PVInstance* p)
	{
		return p->getCFrame().upVector().y >= 1;
	}
}

#endif