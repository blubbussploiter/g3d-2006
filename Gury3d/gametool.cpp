
#include "Mouse.h"
#include "gametool.h"
#include "selection.h"
#include "ray.h"

float getOccludingY(RBX::PVInstance* p)
{
	RBX::PVInstance* target;
	target = RBX::Mouse::getTarget(p);
	if (target && (target != p))
	{ 
		float y;
		if (RBX::getAxisOnUpVector(p))
			y = p->getSize().y;
		else
			y = ceil(p->getSize().z / 2) + 1.2;
		return target->getPosition().y + y;
	}
	return p->getPosition().y;
}

void RBX::GameTool::update(RenderDevice* rd, UserInput* ui)
{
	RBX::PVInstance* pv = RBX::Selection::selection, *i;
	Vector3 vec, hit, pos;

	if (!RBX::Selection::down)
	{
		i = RBX::Mouse::getTarget();
		if (dragged)
		{
			if (dragged->body && dragged->body->_body)
			{
				dragged->body->_body->setLinearVelocity(lastLinearVelocity);
				dragged->body->_body->setAngularVelocity(lastAngularVelocity);
			}
			dragged = 0;
		}
		if (active)
		{
			if(!i || (i && i->getLocked())) Rendering::cursor_custom = Rendering::cursor_far;
			if(i && !i->getLocked())
				Rendering::cursor_custom = Texture::fromFile(GetFileInPath("/content/textures/DragCursor.png"));
		}
		return;
	}

	if (pv)
	{
		Vector3 originrot;

		lastPosition = pv->getPosition();
		originrot = pv->getEulerRotation();
		hit = RBX::Mouse::getHit();

		if (!pv->body || (pv->body && !pv->body->_body)) return;
		if (!isFinite(hit.x) || !isFinite(hit.y) || !isFinite(hit.z)) return;

		hit.x = ceil(hit.x);
		hit.y = ceil(hit.y);
		hit.z = ceil(hit.z);

		vec.x = hit.x;
		vec.z = hit.z;

		if (((int)pv->getSize().x) % 2 == 1)
			vec.x += 0.5;

		vec.y = getOccludingY(pv);

		if (((int)pv->getSize().z) % 2 == 1)
			vec.z += 0.5;

		vec.x = floor(vec.x);
		vec.y = floor(vec.y);
		vec.z = floor(vec.z);

		if (isNaN(hit.x) || isNaN(hit.y) || isNaN(hit.z)) return;

		pv->setPosition(vec);

		if (ui->keyReleased(SDLK_t))
		{
			//float g3dRot[12] = g3d2ode((pv->getCFrame() * Matrix3::fromEulerAnglesXYZ(0, 0, toRadians(90))).rotation);
			//dGeomSetRotation(pv->body->geom[0], g3dRot);
		}

		if (ui->keyReleased(SDLK_r))
		{
			//float g3dRot[12] = g3d2ode((pv->getCFrame() * Matrix3::fromEulerAnglesXYZ(0, toRadians(90), 0)).rotation);
			//dGeomSetRotation(pv->body->geom[0], g3dRot);
		}

		lastPosition = vec;
		dragged = pv;

		//lastLinVel = (dReal*)dBodyGetLinearVel(dragged->body->body);
		//lastAngVel = (dReal*)dBodyGetAngularVel(dragged->body->body);

		lastLinearVelocity = dragged->body->_body->getLinearVelocity();
		lastAngularVelocity = dragged->body->_body->getAngularVelocity();

		if (lastLinearVelocity != btVector3(0,0,0) &&
			lastAngularVelocity != btVector3(0, 0, 0))
		{
			dragged->body->_body->setLinearVelocity(btVector3(0, 0, 0));
			dragged->body->_body->setAngularVelocity(btVector3(0, 0, 0));
		}

		Rendering::cursor_custom = Texture::fromFile(GetFileInPath("/content/textures/GrabRotateCursor.png"));
	}
}

void RBX::GameTool::deactivate()
{
	RBX::Selection::canSelect = 0;
	RBX::Selection::selection = 0;
	RBX::Selection::down = 0;

	if (dragged)
	{
		RBX::Selection::selection = 0;
		if (dragged->body && dragged->body->_body)
		{
			dragged->body->_body->setLinearVelocity(lastLinearVelocity);
			dragged->body->_body->setAngularVelocity(lastAngularVelocity);
		}
		dragged = 0;
	}

	Rendering::cursor_custom = Rendering::cursor_far;
	
}

void RBX::GameTool::activate()
{
	RBX::Selection::canSelect = 1;
}
