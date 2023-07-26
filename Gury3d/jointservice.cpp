
#include "stdout.h"

#include "workspace.h"
#include "jointservice.h"

#include "rbxmath.h"
#include "ray.h"

#include "snaps.h"
#include "hinge.h"
#include "motor.h"

RBX::JointService* jointService;

RBX::NormalId RBX::JointService::getIntersectingFace(RBX::PVInstance* p0, RBX::PVInstance* p1) /* not 100% sure this works */
{
	RBX::World::Ray* ray;
	RBX::NormalId face;
	Vector3 normal;

	face = RBX::NormalId::UNDEFINED;
	ray = new RBX::World::Ray(p1->getPosition(), p0->getPosition());

	normal = ray->getNormalFromRay();
	if (normal != Vector3::zero())
	{
		float x, y, z;
		float v5, v6, v7, v9;

		x = dot(normal, Vector3::unitX());
		y = dot(normal, Vector3::unitY());
		z = dot(normal, Vector3::unitZ());
		v6 = fabs(x);
		v7 = fabs(z);
		v5 = v7;

		if (v6 > v5)
		{
			if (v7 < v6)
			{
				if (x <= 0.0)
					return RBX::LEFT;
				return RBX::RIGHT;
			}
			v9 = z;
		}
		v9 = z;
		if (v7 >= v5)
		{
			if (v9 <= 0.0)
				return RBX::FRONT;
			return RBX::BACK;
		}
		if (y <= 0.0)
			return RBX::BOTTOM;
		return RBX::TOP;
	}

	return face;
}

void RBX::JointService::applyJoint(RBX::PVInstance* p0, RBX::PVInstance* p1, NormalId intersect)
{
	RBX::Surface* surface = p0->getSurface(intersect);

	if (!surface) return;

	switch (surface->getSurfaceType())
	{
		case RBX::SurfaceType::Motor:
		{
			RBX::MotorJoint* motor = new RBX::MotorJoint();
			motor->setParent(this);
			motor->make(p0, p1);
			break;
		}
		default:
		{
			RBX::SnapJoint* snap = new RBX::SnapJoint();
			snap->setParent(this);
			snap->make(p0, p1);
			break;
		}
	}
}

bool RBX::JointService::areJointed(RBX::PVInstance* p0, RBX::PVInstance* p1)
{
	RBX::Joint* j0, * j1;
	bool b = 0;

	if (!p0->body || !p1->body) return 0;

	j0 = p0->body->connector;
	j1 = p1->body->connector;

	if (j0)
	{
		return (j0->part0 == p0 || j0->part1 == p1);
	}

	if (j1)
	{
		return (j1->part0 == p0 || j1->part1 == p1);
	}

	return b;
}

void RBX::JointService::buildJoints(RBX::PVInstance* pv)
{
	RBX::RunService::singleton()->getPhysics()->update();
	if (pv)
	{
		if (pv->getClassName() != "PVInstance") return;

		Touched touched;
		touched = pv->body->touching;

		int size = touched->size();

		for (unsigned int i = 0; i < touched->size(); i++)
		{	
			RBX::PVInstance* t = touched->at(i);
			RBX::NormalId intersect = getIntersectingFace(pv, t);
			applyJoint(t, pv, intersect);
		}

		touched->clear();

	}
}

void RBX::JointService::update()
{
	RBX::Instances* children = getChildren();
	for (unsigned int i = 0; i < children->size(); i++)
	{
		RBX::Joint* joint = (RBX::Joint*)(children->at(i));
		joint->createJoint();
	}
}

RBX::JointService* RBX::JointService::singleton()
{
	if (!jointService) jointService = new JointService();
	return jointService;
}