#include "ray.h"
#include "workspace.h"

#include "render_base.h"
#include "GuiRoot.h"

RBX::PVInstance* RBX::World::Ray::getPartFromRay()
{
	RBX::Instances* instances = new RBX::Instances();
	RBX::PVInstance* part = 0;
	nearest = inf();

	RBX::Workspace::singleton()->getPVInstances(RBX::Workspace::singleton()->getChildren(), instances);
	for (int i = 0; i < instances->size(); i++)
	{
		RBX::PVInstance* child = static_cast<RBX::PVInstance*>(instances->at(i));

		Box b = child->getBox();
#ifdef DEBUGGING
		Rendering::global_app->renderDevice->pushState();
		Draw::box(b, Rendering::global_app->renderDevice);
		RBX::Gui::singleton()->font->draw3D(Rendering::global_app->renderDevice, child->getName(), CoordinateFrame(Rendering::global_app->renderDevice->getCameraToWorldMatrix().rotation, child->getPosition() + Vector3(0, 1.5, 0)), 0.03 * 10, Color3::yellow(), Color3::black(), G3D::GFont::XALIGN_CENTER, G3D::GFont::YALIGN_CENTER);
		Rendering::global_app->renderDevice->popState();
#endif
		if (std::find(ignore->begin(), ignore->end(), child) != ignore->end())
			continue;

		float newdistance = g3dRay.intersectionTime(b);
		if (G3D::isFinite(newdistance) && nearest > abs(newdistance))
		{
			nearest = newdistance;
			lastIntersectionTime = newdistance;
			p = g3dRay.origin + (g3dRay.direction * nearest);
			part = child;
		}
	}

	return part;
}