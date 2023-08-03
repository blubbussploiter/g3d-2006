#include "model.h"
#include "runservice.h"
#include "jointservice.h"
#include "workspace.h"
#include "stdout.h"

#include "rightkeyboard.h"

const Reflection::PropertyDescriptor<RBX::ModelInstance, RBX::ControllerTypes> RBX::ModelInstance::prop_controllerType("Controller", Reflection::Types::TYPE_Number, &RBX::ModelInstance::getController, &RBX::ModelInstance::setController, RBX::ModelInstance::properties);

void RBX::ModelInstance::createController()
{
	RBX::Instances* children;
	children = getChildren();
	for (unsigned int i = 0; i < children->size(); i++)
	{
		RBX::Instance* child;
		child = children->at(i);
		if (child->getClassName() == "Model")
		{
			static_cast<RBX::ModelInstance*>(child)->makeController();
		}
	}
	makeController();
}

void RBX::ModelInstance::makeController()
{
	if (controller) return;
	switch (controllerType)
	{
		case KeyboardRight:
		{
			RightKeyboardController* rkc = new RightKeyboardController();
			rkc->bindCharacter(this);
			controller = rkc;
			break;
		}
		case KeyboardLeft:
		{
			KeyboardController* lkc = new KeyboardController();
			lkc->bindCharacter(this);
			controller = lkc;
			break;
		}
		default: return;
	}

	RBX::StandardOut::print(RBX::MESSAGE_INFO, "Created Controller 0x%08X for '%s'", controller, getName().c_str());
	RBX::addController(controller);
}

void RBX::ModelInstance::setController(int c)
{
	controllerType = (ControllerTypes)c;
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
	RBX::PartInstance* result = 0;
	RBX::Instances* children = new Instances();

	float lastArea = -1;
	RBX::getPVInstances(getChildren(), children);

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
	RBX::JointService::singleton()->buildJoints(this);
}

void RBX::ModelInstance::breakJoints()
{
	/* update for jointservice */
}
