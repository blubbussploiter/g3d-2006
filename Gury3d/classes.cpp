#include "classes.h"

#include "part.h"
#include "camera.h"
#include "lighting.h"
#include "workspace.h"
#include "model.h"

RBX::Instance* RBX::fromName(std::string className)
{

	if (className == "Part")
	{
		return new RBX::PartInstance();
	}

	if (className == "Model")
	{
		return new RBX::ModelInstance();
	}

	if (className == "Lighting")
	{
		return RBX::Lighting::singleton();
	}

	if (className == "Camera")
	{
		return RBX::Camera::singleton();
	}

	if (className == "Workspace")
	{
		return RBX::Workspace::singleton();
	}

	return 0;
}
