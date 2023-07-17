#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <G3DAll.h>
#include "render_shapes.h"

namespace RBX
{
	class Workspace : public RBX::Instance
	{
	public:
		static Workspace* singleton();
		void updatePVInstances(RBX::Instances* instances);
		void getPVInstances(RBX::Instances* instances, RBX::Instances* pvs);
		void renderPVInstances(RenderDevice* d, RBX::Instances* instances, bool renderOpaque);
		void render(RenderDevice* d);
		void update();
		Workspace() { setName("PVWorkspace"); setClassName("PVWorkspace"); isParentLocked = 1; }
	};
}

#endif
