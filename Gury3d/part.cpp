
#include "render_shapes.h"
#include "lighting.h"
#include "part.h"
#include "mesh.h"

Reflection::PropertyDescriptor<RBX::PartInstance, bool> RBX::PartInstance::prop_anchored("Anchored", Reflection::Types::TYPE_Bool, &RBX::PartInstance::getAnchored, &RBX::PartInstance::setAnchored, RBX::PartInstance::properties);
Reflection::PropertyDescriptor<RBX::PartInstance, Vector3> RBX::PartInstance::prop_pos("Position", Reflection::Types::TYPE_Vector3, &RBX::PartInstance::getPosition, &RBX::PartInstance::setPosition, RBX::PartInstance::properties);
Reflection::PropertyDescriptor<RBX::PartInstance, Vector3> RBX::PartInstance::prop_sz("size", Reflection::Types::TYPE_Vector3, &RBX::PartInstance::getSize, &RBX::PartInstance::setSize, RBX::PartInstance::properties);
Reflection::PropertyDescriptor<RBX::PartInstance, CoordinateFrame> RBX::PartInstance::prop_cframe("CFrame", Reflection::Types::TYPE_CFrame, &RBX::PartInstance::getCFrame, &RBX::PartInstance::setCFrame, RBX::PartInstance::properties);
Reflection::PropertyDescriptor<RBX::PartInstance, int> RBX::PartInstance::prop_shape("shape", Reflection::Types::TYPE_Number, &RBX::PartInstance::getShape, &RBX::PartInstance::setShape, RBX::PartInstance::properties);
Reflection::PropertyDescriptor<RBX::PartInstance, int> RBX::PartInstance::prop_brickcolor("BrickColor", Reflection::Types::TYPE_Number, &RBX::PartInstance::getBrickColor, &RBX::PartInstance::setBrickColor, RBX::PartInstance::properties);
Reflection::PropertyDescriptor<RBX::PartInstance, float> RBX::PartInstance::prop_transparency("Transparency", Reflection::Types::TYPE_Number, &RBX::PartInstance::getTransparency, &RBX::PartInstance::setTransparency, RBX::PartInstance::properties);

float getRadius(Vector3 v)
{
	return (v.x + v.y + v.z) / 3;
}

void RBX::PartInstance::setFace(FACES f, SURFACES s)
{
	switch (f)
	{
	case FACES::TOP:
	{
		top->setSurfaceType(s);
		top->getDecal()->setFace(f);
		break;
	}
	case FACES::BOTTOM:
	{
		bottom->setSurfaceType(s);
		bottom->getDecal()->setFace(f);
		break;
	}
	case FACES::LEFT:
	{
		left->setSurfaceType(s);
		left->getDecal()->setFace(f);
		break;
	case FACES::RIGHT:
	{
		right->setSurfaceType(s);
		right->getDecal()->setFace(f);
		break;
	}
	case FACES::FRONT:
	{
		front->setSurfaceType(s);
		front->getDecal()->setFace(f);
		break;
	}
	case FACES::BACK:
	{
		back->setSurfaceType(s);
		back->getDecal()->setFace(f);
		break;
	}
	}
	}
}

void RBX::PartInstance::render(RenderDevice* rd)
{
	color = brickColor->getColor();

	if (findFirstChildOfClass<RBX::Render::Mesh>("PVMesh"))
	{
		findFirstChildOfClass<RBX::Render::Mesh>("PVMesh")->render(rd);
		return;
	}

	PVInstance::render(rd);
}