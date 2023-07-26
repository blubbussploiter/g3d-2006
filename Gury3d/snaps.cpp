#include "snaps.h"
#include "runservice.h"
#include "stdout.h"
#include "bthelp.h"

#include "render_shapes.h"

RBX::SnapJoint* RBX::SnapJoint::getPreExistingSnap(RBX::PVInstance* p0)
{
	RBX::Joint* joint = p0->body->connector;
	if (joint && joint->getClassName() == "Snap")
		return (RBX::SnapJoint*)joint;
	return 0;
}

void RBX::SnapJoint::updateBodies(btCollisionShape* shape)
{
	btDiscreteDynamicsWorld* _world = RBX::RunService::singleton()->getPhysics()->_world;

	_world->removeRigidBody(part0->body->_body);
	_world->removeRigidBody(part1->body->_body);

	part0->body->_body->setCollisionShape(shape);
	part0->body->_body->setMassProps(BODY_MASS, bodyInertia);
	part0->body->_body->updateInertiaTensor();
	part1->body->_body->setCollisionShape(shape);
	part1->body->_body->setMassProps(BODY_MASS, bodyInertia);
	part1->body->_body->updateInertiaTensor();
	part0->body->_body->activate(1);
	part1->body->_body->activate(1);

	_world->addRigidBody(part0->body->_body);
	_world->addRigidBody(part1->body->_body);

}

void RBX::SnapJoint::breakJoint()
{
	hasBeenCalledBroken = 1;
}

bool RBX::SnapJoint::snapBroken()
{
	if (!part0 && !part1) return 0;
}

void RBX::SnapJoint::createJoint()
{
	if (!part0 || !part1 ||
		(part0 && part1 && !part0->body->_body || !part1->body->_body))
		return;

	if (constraint) return;

	checkJoint();

	if (constraint) return;

	btTransform bc0, bc1;

	bc0 = part0->body->_body->getWorldTransform().inverse();
	bc1 = part1->body->_body->getWorldTransform().inverse();

	btGeneric6DofConstraint* snapConstraint = new btGeneric6DofConstraint(*part0->body->_body, *part1->body->_body, bc0, bc1, 0);

	snapConstraint->setLinearUpperLimit(btVector3(0.0f, 0.0f, 0.0f));
	snapConstraint->setLinearLowerLimit(btVector3(0.0f, 0.0f, 0.0f));
	snapConstraint->setAngularLowerLimit(btVector3(-SIMD_EPSILON, -SIMD_EPSILON, -SIMD_EPSILON));
	snapConstraint->setAngularUpperLimit(btVector3(SIMD_EPSILON, SIMD_EPSILON, SIMD_EPSILON));

	btRotationalLimitMotor* motor = snapConstraint->getRotationalLimitMotor(0);

	motor->m_maxMotorForce = 0.0f;
	motor->m_enableMotor = true;

	constraint = snapConstraint;
	RBX::RunService::singleton()->getPhysics()->_world->addConstraint(constraint, 1);

}


/*
void RBX::Snap::createJoint()
{
	checkJoint();

	if (_shape) return;

	btTransform bc0, bc1;
	btCompoundShape* snapShape = 0; 

	btScalar* masses;

	Snap* preSnap;

	bc0 = part0->body->_body->getWorldTransform().inverse();
	bc1 = part1->body->_body->getWorldTransform().inverse();

	RBX::StandardOut::print(RBX::MESSAGE_INFO, "JointService::buildJoints(), Snapped %s and %s", part0->getName().c_str(), part1->getName().c_str());

	masses = new btScalar[2] { BODY_MASS / 2, BODY_MASS/2 };
	preSnap = getPreExistingSnap(part0);

	if(preSnap)
	{
		preSnap->_shape->addChildShape(preSnap->principal.inverse() * bc1, part1->body->_shape);
	}

	if (!preSnap)
	{
		preSnap = getPreExistingSnap(part1);

		if (preSnap)
		{
			preSnap->_shape->addChildShape(preSnap->principal.inverse() * bc0, part0->body->_shape);
		}
	}

	if (preSnap)
	{
		snapShape = preSnap->_shape;
	}

	if (!snapShape)
	{
		snapShape = new btCompoundShape();

		snapShape->calculatePrincipalAxisTransform(masses, principal, bodyInertia);

		snapShape->addChildShape(principal.inverse() * bc0, part0->body->_shape);
		snapShape->addChildShape(principal.inverse() * bc1, part1->body->_shape);

	}
	
	snapShape->calculateLocalInertia(BODY_MASS, bodyInertia);
	_shape = snapShape;

	sPart0 = part0->body->_shape;
	sPart1 = part1->body->_shape;

	part0->body->connector = this;
	part1->body->connector = this;

	updateBodies(_shape);

	//part0->body->_shape = _shape;
	//part1->body->_shape = _shape;

	//part0->body->_body->setUserPointer((void*)this);
	//part1->body->_body->setUserPointer((void*)this);
}
*/