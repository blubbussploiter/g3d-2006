/* (2023) Xplicit Ngine */
/* probably made by whirlpool, mainly based off of https://github.com/Vulpovile/Blocks3D/blob/develop/src/source/XplicitNgine/XplicitNgine.cpp, especially removeBody (i do not know how to do ode or any bullshit like that so) */

#include "runservice.h"
#include "workspace.h"

#define MAX_CONTACTS 4

#pragma comment(lib, "ode.lib")

void collision(void* data, dGeomID o1, dGeomID o2)
{

	int i;

	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	RBX::PVInstance* p1;
	RBX::PVInstance* p2;

	dContact contact[MAX_CONTACTS];

	dWorldID world;
	dJointGroupID group;

	world = RBX::RunService::singleton()->getPhysics()->physWorld;
	group = RBX::RunService::singleton()->getPhysics()->contactgroup;

	if (b1 && b2 && dAreConnected(b1, b2)) return;

	if (int numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact)))
	{
		if (numc <= 0) return;
		for (i = 0; i < numc; i++)
		{

			contact[i].surface.mode = dContactBounce | dContactSoftERP | dContactSoftCFM | dContactApprox1;

			// Define contact surface properties
			contact[i].surface.bounce = 0.5f; //Elasticity
			contact[i].surface.mu = 0.4f; //Friction
			contact[i].surface.slip1 = 0.0f;
			contact[i].surface.slip2 = 0.0f;
			contact[i].surface.soft_erp = 0.8f;
			contact[i].surface.soft_cfm = 0.01f;

			dJointID c = dJointCreateContact(world, group, contact + i);
			dJointAttach(c, b1, b2);

			if (b1)
			{

				p1 = (RBX::PVInstance*)dGeomGetData(o1);
				p2 = (RBX::PVInstance*)dGeomGetData(o2);

				if (p1)
				{
					if (!p1->body->touchedPV && p2)
						p1->body->touchedPV = p2;
					else
						p1->body->touchedPV = 0;
				}
				if (p2)
				{
					if (!p2->body->touchedPV && p1)
						p2->body->touchedPV = p1;
					else
						p2->body->touchedPV = 0;
				}
			}
		}

	}
}

void RBX::XplicitNgine::createBody(RBX::PVInstance* part)
{
	if (part->body->body && part->body->geom[0])
		return;

	printf("creating body for '%s'\n", part->getName().c_str());

	Matrix3 g3dRot = part->getCFrame().rotation;
	float rotation[12] = g3d2ode(g3dRot);

	part->body->body = dBodyCreate(physWorld);

	dMass mass;
	mass.setBox(sqrt(part->getSize().x * 2),sqrt((part->getSize().y / 1.2) * 2),sqrt(part->getSize().z * 2), 0.7f);

	part->body->mass = &mass;

	dBodySetMass(part->body->body, &mass);
	dBodySetData(part->body->body, part);

	switch(part->shape)
	{
		case RBX::ball:
		{
			part->body->geom[0] = dCreateSphere(physSpace, part->getSize().y / 2);
			break;
		}
		default:
		{
			part->body->geom[0] = dCreateBox(physSpace, part->getSize().x, part->getSize().y/1.2, part->getSize().z);
			break;
		}
	}

	dBodySetLinearVel(part->body->body, 0, 0, 0);

	dBodySetPosition(part->body->body, part->getPosition().x, part->getPosition().y, part->getPosition().z);
	dBodySetRotation(part->body->body, rotation);

	dGeomSetPosition(part->body->geom[0], part->getPosition().x, part->getPosition().y, part->getPosition().z);
	dGeomSetRotation(part->body->geom[0], rotation);

	if(!part->getAnchored())
		dGeomSetBody(part->body->geom[0], part->body->body);

	dGeomSetData(part->body->geom[0], part);
}

void RBX::XplicitNgine::removeBody(RBX::PVInstance* part)
{
	if (part->body->body != NULL)
	{
		dBodyEnable(part->body->body);
		dGeomEnable(part->body->geom[0]);

		if (part->getAnchored())
		{
			dGeomSetBody(part->body->geom[0], part->body->body);
			dGeomEnable(part->body->geom[0]);
			updateBody(part);
			update();
		}

		/* future joints stuff 

		for (int i = 0; i < dBodyGetNumJoints(part->body->body); i++) {
			dBodyID b1 = dJointGetBody(dBodyGetJoint(part->body->body, i), 0);
			dBodyID b2 = dJointGetBody(dBodyGetJoint(part->body->body, i), 1);

			if (b1 != NULL)
			{
				dBodyEnable(b1);
				PartInstance* part = (PartInstance*)dBodyGetData(b1);
				if (part != NULL)
					dGeomEnable(part->physGeom[0]);
			}

			if (b2 != NULL)
			{
				dBodyEnable(b2);
				PartInstance* part = (PartInstance*)dBodyGetData(b2);
				if (part != NULL)
					dGeomEnable(part->physGeom[0]);
			}
			dJointDestroy(dBodyGetJoint(partInstance->physBody, i));
		}

		*/

		dBodyDestroy(part->body->body);
		dGeomDestroy(part->body->geom[0]);
		part->body = NULL;
	}
}

void RBX::XplicitNgine::updateBodyCFrame(CoordinateFrame cf, RBX::PVInstance* p)
{
	Matrix3 g3dRot = cf.rotation;
	Vector3 g3dPos = cf.translation;

	updateAnchor(p);

	if (!p->body->body || !p->body->geom[0])
		return;

	float rotation[12] = { g3dRot[0][0], g3dRot[0][1], g3dRot[0][2], 0,
					g3dRot[1][0], g3dRot[1][1], g3dRot[1][2], 0,
					g3dRot[2][0], g3dRot[2][1], g3dRot[2][2], 0 };

	dBodySetPosition(p->body->body,g3dPos.x, g3dPos.y, g3dPos.z);
	dBodySetRotation(p->body->body, rotation);

	dGeomSetPosition(p->body->geom[0], g3dPos.x, g3dPos.y, g3dPos.z);
	dGeomSetRotation(p->body->geom[0], rotation);
}

void RBX::XplicitNgine::updateBody(RBX::PVInstance* part)
{
		
	updateAnchor(part);

	if(!part->isAffectedByPhysics || !part->body->body)
		return;

	part->think();

	const dReal* physPosition = dGeomGetPosition(part->body->geom[0]);
	const dReal* physRotation = dGeomGetRotation(part->body->geom[0]);

	part->setCFrameNoPhysics(CoordinateFrame(
		Matrix3(physRotation[0], physRotation[1], physRotation[2],
			physRotation[4], physRotation[5], physRotation[6],
			physRotation[8], physRotation[9], physRotation[10]),
		Vector3(physPosition[0], physPosition[1], physPosition[2])));
}

void RBX::XplicitNgine::updateAnchor(RBX::PVInstance* part)
{
	if (!part->body->body || !part->body->geom[0]) return;
	if (!part->getAnchored())
		dGeomSetBody(part->body->geom[0], part->body->body);
	else
		dGeomSetBody(part->body->geom[0], 0);
}

void RBX::XplicitNgine::update()
{
	checkBodies(RBX::Workspace::singleton()->getChildren());
	dJointGroupEmpty(contactgroup);
	dSpaceCollide(physSpace, 0, &collision);
	dWorldQuickStep(physWorld, 0.09998f);
}

void RBX::XplicitNgine::checkBodies(RBX::Instances* PVInstances)
{
	RBX::PVInstance* part;
	for (size_t i = 0; i < PVInstances->size(); i++)
	{
		part = (RBX::PVInstance*)PVInstances->at(i);

		if (part && part->getClassName() == "PVInstance" && part->isAffectedByPhysics)
		{
			createBody(part);
		}
		checkBodies(part->getChildren());
	}
}

void RBX::XplicitNgine::init()
{
	checkBodies(RBX::Workspace::singleton()->getChildren());
}