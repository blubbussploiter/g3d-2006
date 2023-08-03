#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <G3DAll.h>
#include <vector>

#include "render_shapes.h"

namespace RBX
{
	class Workspace : public RBX::Instance
	{
	private:
		Instances pvs;
	public:
		/* deprecated, use Datamodel->workspace */
		static Workspace* singleton();
		Extents getGameExtents();
		Instances* getPvs();
		void wakeUpModels();
		void wakeUpPVS();
		void renderPVInstances(RenderDevice* d, bool renderOpaque);
		void removePV(RBX::PVInstance* pv);
		void render(RenderDevice* d);
		void update();
		Workspace() 
		{
			pvs = Instances();
			setName("PVWorkspace"); 
			setClassName("PVWorkspace");
			isParentLocked = 1; 
		}
	};
	void getPVInstances(RBX::Instances* instances, RBX::Instances* pvs);
}

#endif
