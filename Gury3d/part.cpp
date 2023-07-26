
#include "render_shapes.h"
#include "lighting.h"
#include "part.h"
#include "mesh.h"

//RBX::EnumDescriptor<RBX::FormFactor>::addPair("Symmetric");

Reflection::PropertyDescriptor<RBX::PartInstance, int> RBX::PartInstance::prop_brickcolor("BrickColor", Reflection::Types::TYPE_Number, &RBX::PartInstance::getBrickColor, &RBX::PartInstance::setBrickColor, RBX::PartInstance::properties);

float getRadius(Vector3 v)
{
	return (v.x + v.y + v.z) / 3;
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