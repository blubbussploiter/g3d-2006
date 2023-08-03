#include "ray.h"
#include "workspace.h"

#include "rbx.h"
#include "GuiRoot.h"

RBX::PVInstance* RBX::World::Ray::getPartFromRay()
{	
	return getPartFromG3DRay(g3dRay, ignore);
}

Vector3 RBX::World::Ray::getNormalFromRay()
{
	Vector3 normal = Vector3::zero();
	RBX::Instances* instances;
	nearest = inf();

	instances = RBX::Workspace::singleton()->getPvs();

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

	return normal;
}

RBX::PVInstance* RBX::World::getPartFromG3DRay(G3D::Ray ray, RBX::Instances* ignore)
{
	RBX::Instances* instances;
	RBX::PVInstance* part = 0;
	float nearest = inf();

	instances = RBX::Workspace::singleton()->getPvs();
	for (unsigned int i = 0; i < instances->size(); i++)
	{
		RBX::PVInstance* child = static_cast<RBX::PVInstance*>(instances->at(i));

		Box b = child->getBox();

#ifdef DEBUGGING

		Rendering::global_app->renderDevice->pushState();
		Draw::box(b, Rendering::global_app->renderDevice);
		RBX::Gui::singleton()->font->draw3D(Rendering::global_app->renderDevice, child->getName(), CoordinateFrame(Rendering::global_app->renderDevice->getCameraToWorldMatrix().rotation, child->getPosition() + Vector3(0, 1.5, 0)), 0.03 * 10, Color3::yellow(), Color3::black(), G3D::GFont::XALIGN_CENTER, G3D::GFont::YALIGN_CENTER);
		Rendering::global_app->renderDevice->popState();

#endif

		if (ignore)
		{
			if (std::find(ignore->begin(), ignore->end(), child) != ignore->end())
			{
				continue;
			}
		}

		float newdistance = ray.intersectionTime(b);

		if (G3D::isFinite(newdistance) && nearest > abs(newdistance))
		{
			nearest = newdistance;
			part = child;
		}
	}

	return part;
}
