#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <G3DAll.h>
#include <vector>

#include "camera.h"

#include "extents.h"
#include "view.h"

namespace RBX
{

	class Workspace : public RBX::Instance
	{
	private:
		Camera* currentCamera;
	public:
		/* deprecated, use Datamodel->workspace */

		static Workspace* singleton();
		Extents getGameExtents();

		void wakeUpModels();
		void onDescendentAdded(RBX::Instance* descendent);

		Camera* getCurrentCamera();
		void setCurrentCamera(Camera* camera);

		Workspace() 
		{
			setName("PVWorkspace"); 
			setClassName("PVWorkspace");

			isParentLocked = 1; 
		}
	};
	void getPVInstances(RBX::Instances* instances, RBX::Instances* pvs);
}

#endif
