#include "ray.h"
#include "workspace.h"

#include "rbx.h"
#include "GuiRoot.h"

RBX::PVInstance* RBX::World::Ray::getPartFromRay()
{
	RBX::Instances* instances = new RBX::Instances();
	RBX::PVInstance* part = 0;
	nearest = inf();

	RBX::Workspace::singleton()->getPVInstances(RBX::Workspace::singleton()->getChildren(), instances);
	for (unsigned int i = 0; i < instances->size(); i++)
	{
		RBX::PVInstance* child = static_cast<RBX::PVInstance*>(instances->at(i));

		Box b = child->getBox();
		if (std::find(ignore->begin(), ignore->end(), child) != ignore->end())
		{
			continue;
		}

		float newdistance = g3dRay.intersectionTime(b);
		if (G3D::isFinite(newdistance) && nearest > abs(newdistance))
		{
			nearest = newdistance;
			lastIntersectionTime = newdistance;
			p = g3dRay.origin + (g3dRay.direction * nearest);
			part = child;
		}
	}

	instances->clear();
	delete instances;

	return part;
}

Vector3 RBX::World::Ray::getNormalFromRay()
{
	RBX::Instances* instances = new RBX::Instances();
	Vector3 normal = Vector3::zero();
	nearest = inf();

	RBX::Workspace::singleton()->getPVInstances(RBX::Workspace::singleton()->getChildren(), instances);

	for (unsigned int i = 0; i < instances->size(); i++)
	{
		Box b = static_cast<RBX::PVInstance*>(instances->at(i))->getBox();

		float newdistance = g3dRay.intersectionTime(b);
		if (G3D::isFinite(newdistance) && nearest > abs(newdistance))
		{
			nearest = newdistance;
			lastIntersectionTime = newdistance;
			normal = g3dRay.origin + (g3dRay.direction * nearest);
		}
	}

	instances->clear();
	delete instances;

	return normal;
}