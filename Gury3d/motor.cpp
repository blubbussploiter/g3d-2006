#include "stdout.h"
#include "render_shapes.h"
#include "motor.h"

void RBX::MotorJoint::createJoint()
{
	checkJoint();

	if (constraint) return;

	RBX::StandardOut::print(RBX::MESSAGE_INFO, "Creating Motor for '%s' and '%s'", part0->getName().c_str(), part1->getName().c_str());

	btTransform bc0, bc1;

	bc0 = part0->body->_body->getWorldTransform().inverse();
	bc1 = part1->body->_body->getWorldTransform().inverse();

	btGeneric6DofConstraint* motorConstraint = new btGeneric6DofConstraint(*part0->body->_body, *part1->body->_body, bc0, bc1, 0);

	motorConstraint->setAngularLowerLimit(btVector3(-SIMD_EPSILON, -SIMD_EPSILON, -SIMD_EPSILON));
	motorConstraint->setAngularUpperLimit(btVector3(SIMD_EPSILON, SIMD_EPSILON, SIMD_EPSILON));

	btRotationalLimitMotor* motor = motorConstraint->getRotationalLimitMotor(0);

	motor->m_maxMotorForce = 10.0f;
	motor->m_targetVelocity = 10.0f;
	motor->m_enableMotor = true;

	constraint = motorConstraint;
	RBX::RunService::singleton()->getPhysics()->_world->addConstraint(constraint, 1);

}