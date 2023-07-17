#include <math.h>

#include "players.h"
#include "camera.h"

#include "runservice.h"
#include "sounds.h"
#include "humanoid.h"

void RBX::Network::PlayerController::mv_update()
{
	PVInstance* primaryPart;
	RBX::ModelInstance* c;
	RBX::Camera* cam;
	RBX::Humanoid* h;

	c = RBX::Network::Players::findLocalCharacter();
	if (!c) return;
	h = c->findFirstChildOfClass<RBX::Humanoid>("Humanoid");
	if (!h) return;

	primaryPart = plr->character->primaryPart;
	cam = RBX::Camera::singleton();

	if (!primaryPart || !cam)
		return;

	/* camera clipping */

	if (cam->isUsingRightMouse || cam->isZooming)
	{
		Vector3 pos = cam->cframe.translation;
		Vector3 ppos = primaryPart->getPosition();

		float dist = (ppos - pos).magnitude();

		cam->isZooming = 0;

		/* semi deep */

		if (dist <= 5)
		{
			RBX::setModelTransparency(plr->character, 0.5);

			/* deeper */

			if (dist <= 2.5)
				RBX::setModelTransparency(plr->character, 1);
		}
		else
			RBX::setModelTransparency(plr->character, 0);
	}

	if (!moving())
	{
		h->setWalkDirection(Vector3::zero());
	}

	h->step();
}

void RBX::Network::PlayerController::move()
{
	CoordinateFrame o;
	Vector3 mov;
	Vector3 look;
	RBX::Camera* camera;
	RBX::ModelInstance* character;
	RBX::Humanoid* humanoid;

	character = RBX::Network::Players::findLocalCharacter();

	if (!moving() || !RBX::RunService::singleton()->isRunning)
		return;

	if (!character || (character && !RBX::Humanoid::modelIsCharacter(character))) return;

	humanoid = RBX::Humanoid::modelIsCharacter(character);

	camera = RBX::Camera::singleton();
	o = camera->cframe;
	look = o.lookVector();

	switch (dir())
	{
	case Movement::Forward:
	{
		mov = normalize(o.lookVector());
		break;
	}
	case Movement::Backwards:
	{
		mov = normalize(-o.lookVector());
		break;
	}
	case Movement::Right:
	{
		mov = normalize(o.rightVector());
		break;
	}
	case Movement::Left:
	{
		mov = normalize(-o.rightVector());
		break;
	}
	case Movement::Jump:
	{
		humanoid->setJump();
		setdir(Movement::Idle);
		return;
	}
	case Movement::Idle: { return; }
	}

	humanoid->setWalkDirection(mov);
}