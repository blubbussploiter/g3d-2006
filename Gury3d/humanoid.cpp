#include "humanoid.h"
#include "rbxmath.h"

#include "camera.h"

#include "sounds.h"
#include "ray.h"

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

bool RBX::Humanoid::isFalling()
{
  //  const dReal* lin;
    if (!checkHumanoidAttributes())
        return 0;
  //  lin = dBodyGetLinearVel(humanoidRootPart->body->body);
  //  return !(lin[1] >= -0.1);
}

bool RBX::Humanoid::isInAir()
{
 //   const dReal* lin;
    if (!checkHumanoidAttributes())
        return 0;
  //  lin = dBodyGetLinearVel(humanoidRootPart->body->body);
   // return (lin[1] > 0.1);
}

bool RBX::Humanoid::occluding(RBX::PVInstance* p, Vector3 dir)
{
    RBX::PVInstance *rt1;
    RBX::World::Ray* r1;

    if (!checkHumanoidAttributes() || !p->body)
        return 0;

    r1 = new RBX::World::Ray(p->getPosition(),
       dir);

    r1->setIgnoreList(new Instances{ p->getParent() });
    rt1 = r1->getPartFromRay();

    return (rt1 && rt1->body);
}

bool RBX::Humanoid::isGrounded()
{
    RBX::PVInstance *rt1;
    RBX::World::Ray* r1;

    if (!checkHumanoidAttributes())
        return 0;

    r1 = new RBX::World::Ray(humanoidRootPart->getPosition(), 
        -humanoidRootPart->getCFrame().upVector());

    r1->setIgnoreList(new Instances{ getParent() });
    rt1 = r1->getPartFromRay();

    if (!rt1 || isFalling() || isInAir())
        return 0;

    return (rt1 != 0);
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
        //dBodySetLinearVel(humanoidRootPart->body->body, walkDirection.x * 2, jmpPower / 2.5, walkDirection.z * 2);
        whoosh->setStartPosition(1.2f);
        whoosh->play();
    }
}

void RBX::Humanoid::balance()
{
    PhysBody* body;
    body = humanoidRootPart->body;
    if (body) 
    {
        //dBodySetAngularVel(body->body, 0, 0, 0);
    }

}

void RBX::Humanoid::onDied()
{
    if (isDead)
        return;

    isDead = 1;
    uuhhh->play();
}

void RBX::Humanoid::step()
{
    return;

    Vector3 orig;
    Vector3 pos;
    Vector3 wlk;

    CoordinateFrame _old, look;

    if (!health)
        onDied();

    if (checkHumanoidAttributes())
    {
        if (humanoidHead->getPosition().y < -200 && health > 0)
        {
            onDied();
            getParent()->remove();
        }
        balance();
    }

    if (!isJoined()) health = 0;

    switch (walkMode)
    {
        case DIRECTION_MOVE:
        {

            if (!checkHumanoidAttributes())
            {
                correctHumanoidAttributes();
                return;
            }

            if (!isGrounded())
            {
                bsls_steps->stop();
                return;
            }

            orig = humanoidHead->getPosition();
            _old = humanoidHead->getCFrame();
            look = _old;

            pos = orig + walkDirection;
            look.lookAt(pos);

            if (walkDirection != Vector3::zero())
            {
                CoordinateFrame lerpd;
                wlk = lerp(_old.translation, pos, 0.65f);

                if (!bsls_steps->isPlaying())
                    bsls_steps->play();

                lerpd = _old.lerp(look, RBX::Camera::singleton()->getLerp());
                humanoidHead->setCFrame(lerpd);

                /*  walk code here: PLEASE someone. PLEASE just change this its SO ASS!!! */

                humanoidHead->setPosition(wlk);

            }
            else
                bsls_steps->stop();

            break;
        }
        default: break;
    }
    
}
