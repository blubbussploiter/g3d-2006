#ifndef VIEW_H
#define VIEW_H

#include "scene.h"

namespace RBX
{
	class View : public Instance
	{
	public:
		void onWorkspaceDescendentAdded(RBX::Instance* descendent);
		void onWorkspaceDescendentRemoved(RBX::Instance* descendent);
		void render(RenderDevice* rd);
		static View* singleton();
	};
}

#endif