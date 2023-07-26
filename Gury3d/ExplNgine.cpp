/* (2023) Xplicit Ngine */
/* probably made by whirlpool, mainly based off of https://github.com/Vulpovile/Blocks3D/blob/develop/src/source/XplicitNgine/XplicitNgine.cpp, especially removeBody (i do not know how to do ode or any bullshit like that so) */

#include "runservice.h"
#include "workspace.h"
#include "stdout.h"
#include "GuiRoot.h"
#include "bthelp.h"

//#pragma comment(lib, "/bullet/BulletMultiThreaded.lib")
#pragma comment(lib, "/bullet/BulletCollision.lib")
#pragma comment(lib, "/bullet/BulletDynamics.lib")
#pragma comment(lib, "/bullet/LinearMath.lib")

void myTickCallback(btDynamicsWorld* dynamicsWorld, btScalar timeStep) {

	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();

	//RBX::StandardOut::print(RBX::MESSAGE_INFO, "Num of manifolds = %d", numManifolds);

	for (int i = 0; i < numManifolds; i++) {

		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

		const btCollisionObject* objA = contactManifold->getBody0();
		const btCollisionObject* objB = contactManifold->getBody1();

		int numContacts = contactManifold->getNumContacts();

		//RBX::StandardOut::print(RBX::MESSAGE_INFO, "Num of contacts = %d", numContacts);

		for (int j = 0; j < numContacts; j++) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			RBX::PVInstance* p0, * p1;

			p0 = (RBX::PVInstance*)objA->getUserPointer();
			p1 = (RBX::PVInstance*)objB->getUserPointer();

			if (p0 && p1 && p0->body && p1->body)
			{
				/* clear after using touching.. maybe? */

				//p0->body->touching->clear();
				//p1->body->touching->clear();
				if (p0->body && p1->body)
				{
					if(!contains((*p0->body->touching), p1))
						p0->body->touching->push_back(p1);
					if (!contains((*p1->body->touching), p0))
						p1->body->touching->push_back(p0);
				}
			}
		}
	}
}

void RBX::XplicitNgine::createBody(RBX::PVInstance* part)
{
	/* if not body, create body, else update it :) */

	Vector3 size;
	btVector3 bodyInertia;

	btTransform transform;

	btDefaultMotionState* motionState;
	btRigidBody::btRigidBodyConstructionInfo* bodyCI;

	if (part->body->_body)
	{
		updateBody(part);
		return;
	}

	size = part->getSize();
	size.y *= getAffectedFormFactor(part);
	size /= 2;

	switch (part->shape)
	{
	case RBX::ball:
	{
		part->body->_shape = new btSphereShape(part->getSize().y / 2);
		break;
	}
	case RBX::cylinder:
	{
		part->body->_shape = new btCylinderShape(btVector3(size.x, size.y, size.z));
		break;
	}
	default: 
	{
		part->body->_shape = new btBoxShape(g3Vector3(size));
		break;
	}
	}

	transform = ToTransform(part->getCFrame());
	motionState = new btDefaultMotionState(transform);

	part->body->_shape->calculateLocalInertia(BODY_MASS, bodyInertia);

	bodyCI = new btRigidBody::btRigidBodyConstructionInfo(BODY_MASS, motionState, part->body->_shape, bodyInertia);

	bodyCI->m_restitution = 0.0f;
	bodyCI->m_friction = part->getFriction();

	part->body->_body = new btRigidBody(*bodyCI);
	part->body->_body->setUserPointer((void*)part);

	part->body->_body->setCcdMotionThreshold(1e-7);
	part->body->_body->setCcdSweptSphereRadius(1.0f);

	if (!part->getAnchored())
		_world->addRigidBody(part->body->_body);
	else
	{
		part->body->_collider = new btCollisionObject();

		part->body->_collider->setWorldTransform(transform);
		part->body->_collider->setCollisionShape(part->body->_shape);

		part->body->_collider->setUserPointer((void*)part);

		_world->addCollisionObject(part->body->_collider);
	}
	
}

void RBX::XplicitNgine::removeBody(RBX::PVInstance* part)
{
	if (part->body->_body)
	{
		delete part->body->_body->getMotionState();
		delete part->body->_body;
	}

	delete part->body->_shape;
}

void RBX::XplicitNgine::resetBody(RBX::PVInstance* part)
{
	removeBody(part);
	createBody(part);
}

void RBX::XplicitNgine::updateBodyCFrame(CoordinateFrame cf, RBX::PVInstance* p)
{
	if (!p->body->_body) return;
	p->body->_body->setWorldTransform(ToTransform(cf));
	p->body->_body->setCenterOfMassTransform(ToTransform(cf));
}

void RBX::XplicitNgine::updateBody(RBX::PVInstance* part)
{
	btTransform transform;
	CoordinateFrame cframe;

	if (!part->body->_body) return;

	transform = part->body->_body->getInterpolationWorldTransform();
	cframe = ToCoordinateFrame(transform);

	part->setCFrameNoPhysics(cframe);

	btVector3 low, hi;

	part->body->_shape->getAabb(transform, low, hi);

#ifdef DEBUGGING

	/* debug shit dont look at it okay thanks bye */

	Rendering::global_app->renderDevice->pushState();
	Draw::box(Box(b3Vector3(low), b3Vector3(hi)), Rendering::global_app->renderDevice);
	RBX::Gui::singleton()->font->draw3D(Rendering::global_app->renderDevice, part->getName(), CoordinateFrame(Rendering::global_app->renderDevice->getCameraToWorldMatrix().rotation, part->getPosition() + Vector3(0, 1.5, 0)), 0.03 * 10, Color3::yellow(), Color3::black(), G3D::GFont::XALIGN_CENTER, G3D::GFont::YALIGN_CENTER);
	Rendering::global_app->renderDevice->popState();

#endif
}

void RBX::XplicitNgine::checkBodies(RBX::Instances* PVInstances)
{
	RBX::PVInstance* part;
	for (unsigned int i = 0; i < PVInstances->size(); i++)
	{
		part = (RBX::PVInstance*)PVInstances->at(i);

		if (part && part->getClassName() == "PVInstance" && part->isAffectedByPhysics)
		{
			createBody(part);
		}
		checkBodies(part->getChildren());
	}
}

void RBX::XplicitNgine::updateAnchor(RBX::PVInstance* part)
{

}

void RBX::XplicitNgine::update()
{
	_world->stepSimulation(0.015f, 0);
}

void RBX::XplicitNgine::close()
{
	RBX::StandardOut::print(RBX::MESSAGE_INFO, "XplicitNgine::close()");
	delete _world;
	delete _solver;
	delete _collisionConfiguration;
	delete _dispatcher;
	delete _broadphase;
}

void RBX::XplicitNgine::init()
{
	_world->setInternalTickCallback(myTickCallback);
	checkBodies(RBX::Workspace::singleton()->getChildren());
}

bool RBX::XplicitNgine::areColliding(RBX::PVInstance* part1, RBX::PVInstance* part2)
{
	//return part1->body->_shape.;
	return 1;
}