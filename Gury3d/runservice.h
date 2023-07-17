#ifndef RUNSERVICE_H
#define RUNSERVICE_H

#include <ode/ode.h>
#include <ode/odecpp.h>
#include <G3DAll.h>

#include "instance.h"

#define g3d2ode(g3dRot) { g3dRot[0][0], g3dRot[0][1], g3dRot[0][2], 0, g3dRot[1][0], g3dRot[1][1], g3dRot[1][2], 0, g3dRot[2][0], g3dRot[2][1], g3dRot[2][2], 0 }

namespace RBX
{
	class PhysBody; class PVInstance;
}

namespace RBX
{

	class XplicitNgine
	{
	public:

		dWorldID physWorld;
		dSpaceID physSpace;
		dJointGroupID contactgroup;

		void update();
		void init();
		void checkBodies(RBX::Instances* PVInstances);
		void createBody(RBX::PVInstance* part);
		void removeBody(RBX::PVInstance* part);
		void updateBody(RBX::PVInstance* part);
		void updateBodyCFrame(CoordinateFrame cf, RBX::PVInstance* p);
		void updateAnchor(RBX::PVInstance* part);
		XplicitNgine()
		{
			physWorld = dWorldCreate();
			physSpace = dHashSpaceCreate(0);
			contactgroup = dJointGroupCreate(0);

			dWorldSetGravity(physWorld, 0, -9.81F, 0);
			dWorldSetAutoDisableSteps(physWorld, 5);
			dWorldSetAutoDisableLinearThreshold(physWorld, 0.1);
			dWorldSetAutoDisableAngularThreshold(physWorld, 0.1);
			//dWorldSetERP(physWorld, 0.8);
		}
	};

	class RunService
	{
	private:
		XplicitNgine* physics;
	public:
		bool isRunning;
		void run();
		void update();
		XplicitNgine* getPhysics() { return physics; }
		static RunService* singleton();
	};

}

#endif