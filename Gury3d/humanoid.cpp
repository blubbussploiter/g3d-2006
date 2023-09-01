#include "humanoid.h"
#include "rbxmath.h"

#include "camera.h"

#include "sounds.h"
#include "ray.h"

#include "jointservice.h"
#include "bthelp.h"

RBX::Sound* whoosh = RBX::Sound::fromFile(GetFileInPath("/content/sounds/button.wav"));
RBX::Sound* uuhhh = RBX::Sound::fromFile(GetFileInPath("/content/sounds/uuhhh.wav"));
RBX::Sound* bsls_steps = RBX::Sound::fromFile(GetFileInPath("/content/sounds/bfsl-minifigfoots1.mp3"), 1);

RBX::Humanoid* RBX::Humanoid::modelIsCharacter(RBX::Instance* testModel)
{
    if (testModel)
        return testModel->findFirstChildOfClass<RBX::Humanoid>("Humanoid");
    else
        return 0;
}

void RBX::Humanoid::setHumanoidAttributes()
{
	if (!getParent())
		return;

	humanoidHead = static_cast<RBX::PVInstance*>(getParent()->findFirstChild("Head"));
	humanoidRootPart = static_cast<RBX::PVInstance*>(getParent()->findFirstChild("Torso"));

}

void RBX::Humanoid::correctHumanoidAttributes()
{
    if (checkHumanoidAttributes())
        return;

    setHumanoidAttributes();
}

void RBX::Humanoid::buildJoints()
{
    if (!checkHumanoidAttributes())
        correctHumanoidAttributes();
    RBX::JointService::singleton()->manualBuild(humanoidRootPart, humanoidHead);
    if (getRightLeg()) RBX::JointService::singleton()->manualBuild(humanoidRootPart, getRightLeg());
    if (getLeftLeg()) RBX::JointService::singleton()->manualBuild(humanoidRootPart, getLeftLeg());
    if (getRightArm()) RBX::JointService::singleton()->manualBuild(humanoidRootPart, getRightArm());
    if (getLeftArm()) RBX::JointService::singleton()->manualBuild(humanoidRootPart, getLeftArm());
}

bool RBX::Humanoid::isFalling()
{
    btVector3 lin;
    if (!checkHumanoidAttributes())
        return 0;
    lin = humanoidRootPart->body->_body->getLinearVelocity();
    return !(lin.y() >= -0.1);
}

bool RBX::Humanoid::isInAir()
{
    btVector3 lin;
    if (!checkHumanoidAttributes())
        return 0;
    lin = humanoidRootPart->body->_body->getLinearVelocity();
    return (lin.y() > 0.1);
}

bool RBX::Humanoid::isJoined()
{
    return (checkHumanoidAttributes() && RBX::JointService::singleton()->areJointed(humanoidHead, humanoidRootPart));
}

bool RBX::Humanoid::isGrounded()
{
    btVector3 btFrom, btTo;
    btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

    btFrom = g3Vector3(getRightLeg()->getPosition());
    btTo = btVector3(btFrom.x(), -1000, btFrom.z());

    RBX::RunService::singleton()->getPhysics()->_world->rayTest(btFrom, btTo, res); 
    return res.hasHit();
}

void RBX::Humanoid::setWalkDirection(Vector3 value)
{
    double x, z;
    double v4;

    if (value != walkDirection)
    {
        if (value == G3D::Vector3::zero())
            walkDirection = value;
        else
        {
            x = value.x;
            z = value.z;

            v4 = 1.0 / sqrt(z * z + x * x);
            walkDirection.x = x * v4;
            walkDirection.y = 0;
            walkDirection.z = z * v4;
            walkMode = DIRECTION_MOVE;
        }
    }
}

void RBX::Humanoid::setJump()
{
    if (isGrounded() && !isFalling() && !isInAir())
    {
        bsls_steps->stop();
        humanoidRootPart->body->_body->setActivationState(ACTIVE_TAG);
        humanoidRootPart->body->_body->setLinearVelocity(btVector3(0, jmpPower / 2.5f, 0));
        whoosh->play();
    }
}

void RBX::Humanoid::balance()
{
    PhysBody* body;
    body = humanoidRootPart->body;
    if (body) 
    {

    }

}

void RBX::Humanoid::onDied()
{
    if (isDead)
        return;

    isDead = 1;
    uuhhh->play();
}

void RBX::Humanoid::onStep()
{

    

    CoordinateFrame _old, look;
    btVector3 linVel;

    if (!health)
        onDied();

    if (!checkHumanoidAttributes())
    {
        correctHumanoidAttributes();
    }

    if (checkHumanoidAttributes())
    {
        if (humanoidRootPart->body->_body)
        {
            float y;
            y = humanoidRootPart->body->_body->getCenterOfMassPosition().y();
            if (y < -200)
            {
                onDied();
                getParent()->remove();
            }
        }
        balance();
    }

    if (!jointsBuilt && !isJoined())
    {
        buildJoints();

        if (humanoidRootPart->body->_body)
        {

            humanoidRootPart->body->_body->setSleepingThresholds(0.0, 0.0);
            humanoidRootPart->body->_body->setAngularFactor(0.0);

        }

        jointsBuilt = 1;
    }

    if (!isJoined() && jointsBuilt)
    {
        health = 0;
    }


}

RBX::PartInstance* RBX::Humanoid::getRightArm()
{
    return static_cast<RBX::PartInstance*>(getParent()->findFirstChild("Right Arm"));
}

RBX::PartInstance* RBX::Humanoid::getLeftArm()
{
    return static_cast<RBX::PartInstance*>(getParent()->findFirstChild("Left Arm"));
}

RBX::PartInstance* RBX::Humanoid::getRightLeg()
{
    return static_cast<RBX::PartInstance*>(getParent()->findFirstChild("Right Leg"));
}

RBX::PartInstance* RBX::Humanoid::getLeftLeg()
{
    return static_cast<RBX::PartInstance*>(getParent()->findFirstChild("Left Leg"));
}
