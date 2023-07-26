#include "model.h"
#include "runservice.h"
#include "jointservice.h"
#include "workspace.h"
#include "stdout.h"

#include "rightkeyboard.h"
#include "leftkeyboard.h"

Reflection::PropertyDescriptor<RBX::ModelInstance, RBX::ControllerTypes> RBX::ModelInstance::prop_controllerType("Controller", Reflection::Types::TYPE_Number, &RBX::ModelInstance::getController, &RBX::ModelInstance::setController, RBX::ModelInstance::properties);

void RBX::ModelInstance::setController(int c)
{
	/*
	switch (c)
	{
	case KeyboardRight:
	{
		controller = new RightKeyboardController();
		break;
	}
	case KeyboardLeft:
	{
		controller = new LeftKeyboardController();
		break;
	}
	}
	RBX::addController(controller);
 */
}

/* positional things */

void RBX::ModelInstance::move(Vector3 vect)
{
	RBX::Instance* child;
	for (unsigned int i = 0; i < getChildren()->size(); i++)
	{
		child = getChildren()->at(i);
		if (child && child->getClassName() == "PVInstance")
		{
			RBX::PVInstance* pv = (RBX::PVInstance*)child;
			pv->setPosition(pv->getPosition() + vect);
		}
	}
}

void RBX::ModelInstance::rotate(Vector3 rot)
{
	RBX::Instance* child;
	for (unsigned int i = 0; i < getChildren()->size(); i++)
	{
		child = getChildren()->at(i);
		if (child && child->getClassName() == "PVInstance")
		{
			RBX::PVInstance* pv = (RBX::PVInstance*)child;
			pv->setEulerRotation(rot.x, rot.y, rot.z);
		}
	}
}

void RBX::ModelInstance::lookAt(Vector3 vect)
{
	RBX::Instance* child;
	for (unsigned int i = 0; i < getChildren()->size(); i++)
	{
		child = getChildren()->at(i);
		if (child && child->getClassName() == "PVInstance")
		{
			RBX::PVInstance* pv = (RBX::PVInstance*)child;
			pv->lookAt(vect);
		}
	}
}


RBX::PartInstance* RBX::ModelInstance::getPrimaryPartInternal()
{
	RBX::Instances* children = new RBX::Instances();
	RBX::Workspace::singleton()->getPVInstances(getChildren(), children);
	RBX::PartInstance* result = 0;
	float lastArea = -1;
	for (unsigned int i = 0; i < children->size(); i++)
	{
		RBX::PartInstance* pv = (RBX::PartInstance*)(children->at(i));
		RBX::Extents extents = pv->getWorldExtents();
		float area = extents.area();
		if (area > lastArea)
		{
			lastArea = area;
			result = pv;
		}
	}
	children->clear();
	delete children;
	return result;
}

/* joints */

void RBX::ModelInstance::buildJoints()
{
	RBX::Instances* children;
	if (!primaryPart) primaryPart = getPrimaryPartInternal();
	children = getChildren();
	for (unsigned int i = 0; i < children->size(); i++)
	{
		RBX::Instance* child;
		child = children->at(i);
		if (child->getClassName() == "Model")
		{
			static_cast<RBX::ModelInstance*>(child)->buildJoints();
		}
	}
	RBX::StandardOut::print(RBX::MESSAGE_INFO, "ModelInstance::buildJoints(), building joints for '%s', joints to build : %d", getName().c_str(), children->size());
	RBX::JointService::singleton()->buildJoints(primaryPart);
}

void RBX::ModelInstance::breakJoints()
{
	/* update for jointservice */
}
