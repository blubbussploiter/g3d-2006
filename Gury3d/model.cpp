#include "model.h"
#include "runservice.h"
#include "workspace.h"
#include "welds.h"

void RBX::ModelInstance::move(Vector3 vect)
{
	RBX::Instance* child;
	for (int i = 0; i < getChildren()->size(); i++)
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
	for (int i = 0; i < getChildren()->size(); i++)
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
	for (int i = 0; i < getChildren()->size(); i++)
	{
		child = getChildren()->at(i);
		if (child && child->getClassName() == "PVInstance")
		{
			RBX::PVInstance* pv = (RBX::PVInstance*)child;
			pv->lookAt(vect);
		}
	}
}

void RBX::ModelInstance::breakJoints()
{
	RBX::Instances* pvs = new RBX::Instances();
	RBX::Workspace::singleton()->getPVInstances(getChildren(), pvs);
	for (int i = 0; i < pvs->size(); i++)
	{
		for (int o = 0; o < pvs->at(i)->getChildren()->size(); o++)
		{
			RBX::Physics::Weld* w = static_cast<RBX::Physics::Weld*>(pvs->at(i)->getChildren()->at(o));
			if (w && w->getClassName() == "Weld")
			{
				w->remove();
			}
		}
	}
}
