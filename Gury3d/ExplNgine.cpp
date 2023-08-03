/* (2023) Xplicit Ngine */

#include "jointservice.h"
#include "runservice.h"
#include "workspace.h"
#include "stdout.h"
#include "bthelp.h"
#include "snaps.h"

#pragma comment(lib, "/bullet/BulletCollision.lib")
#pragma comment(lib, "/bullet/BulletDynamics.lib")
#pragma comment(lib, "/bullet/LinearMath.lib")

void myTickCallback(btDynamicsWorld* dynamicsWorld, btScalar timeStep) {

	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++) {

		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

		const btCollisionObject* objA = contactManifold->getBody0();
		const btCollisionObject* objB = contactManifold->getBody1();

		int numContacts = contactManifold->getNumContacts();

		for (int j = 0; j < numContacts; j++) {
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			RBX::PVInstance* p0, * p1;

			p0 = (RBX::PVInstance*)objA->getUserPointer();
			p1 = (RBX::PVInstance*)objB->getUserPointer();

			if (p0 && p1 && p0->body && p1->body)
			{
				/* call :touched signal */
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
	p->body->_body->setCenterOfMassTransform(ToTransform(cf));
}

void RBX::XplicitNgine::updateBody(RBX::PVInstance* part)
{
	btTransform transform;
	CoordinateFrame cframe;

	RBX::Joint* connector;

	if (!part->body->_body || 
		(part->body->_body && !part->body->_body->isActive())) return;

	connector = part->body->connector;
	transform = part->body->_body->getWorldTransform();

	/*
	if (connector)
	{
		switch (connector->type)
		{
			case JointType::Snap:
			{
				RBX::SnapJoint* snap;
				snap = (RBX::SnapJoint*)(connector);
				transform = snap->getPartCompoundPosition(part);
				break;
			}
			default: break;
		}
	}
	*/

	cframe = ToCoordinateFrame(transform);
	part->setCFrameNoPhysics(cframe);
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
	_world->stepSimulation(0.025f);
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