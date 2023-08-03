#ifndef RUNSERVICE_H
#define RUNSERVICE_H

#include <G3DAll.h>
#include "btBulletDynamicsCommon.h"

#include "instance.h"

#define BODY_MASS 10.333f
#define contains(v, i) (std::find(v.begin(), v.end(), i) != v.end())

namespace RBX
{
	class PhysBody; class PVInstance;
}

namespace RBX
{

	class XplicitNgine
	{
	public:

		btBroadphaseInterface* _broadphase;
		btDefaultCollisionConfiguration* _collisionConfiguration;
		btCollisionDispatcher* _dispatcher;
		btSequentialImpulseConstraintSolver* _solver;
		btDiscreteDynamicsWorld* _world;

		void update();
		void init();
		void close();
		void checkBodies(RBX::Instances* PVInstances);
		void createBody(RBX::PVInstance* part);
		void removeBody(RBX::PVInstance* part);
		void resetBody(RBX::PVInstance* part);
		void updateBody(RBX::PVInstance* part);
		void updateBodyCFrame(CoordinateFrame cf, RBX::PVInstance* p);
		void updateAnchor(RBX::PVInstance* part);

		bool areColliding(RBX::PVInstance* part1, RBX::PVInstance* part2);

		int getNumberOfGeoms() {
			if (_world)
			{
				return _world->getNumCollisionObjects();
			}
			return 0;
		}

		int getNumberOfConstraints() {
			if (_world)
			{
				return _world->getNumConstraints();
			}
			return 0;
		}

		XplicitNgine()
		{
			_broadphase = new btDbvtBroadphase();
			_collisionConfiguration = new btDefaultCollisionConfiguration();
			_dispatcher = new btCollisionDispatcher(_collisionConfiguration);
			_solver = new btSequentialImpulseConstraintSolver();
			_world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);
			_world->setGravity(btVector3(0, -9.8f, 0));
		}
	};

	class RunService
	{
	private:
		XplicitNgine* physics;
	public:
		bool isRunning;
		void run();
		void stop();
		void reset();
		void update();
		void heartbeat();
		void updateSteppers(RBX::Instances* steppers);
		XplicitNgine* getPhysics() { return physics; }
		/* deprecated, use Datamodel->runService */
		static RunService* singleton();
		RunService()
		{
			isRunning = 0;
			physics = new XplicitNgine();
			physics->init();
		}
	};

}

#endif