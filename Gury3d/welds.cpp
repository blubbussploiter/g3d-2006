#include "welds.h"
#include "runservice.h"

void RBX::Physics::Weld::weld(
	RBX::PVInstance* p0, RBX::PVInstance* p1)
{
	part0 = p0;
	part1 = p1;
}

void RBX::Physics::Weld::breakWeld()
{
	dGeomID geom0, geom1;
	Vector3 position;

	if (!part0->body->body || !part1->body->body)
		return;

	geom0 = part0->body->geom[0];
	geom1 = part1->body->geom[0];

	part0->body->body = oPart0;
	part1->body->body = oPart1;

	position = part1->getPosition();

	dBodyDisable(weldBody);
	dBodySetData(weldBody, 0);

	dGeomClearOffset(geom0);
	dGeomClearOffset(geom1);

	dGeomSetBody(geom0, oPart0);
	dGeomSetBody(geom1, oPart1);

	hasBeenCalledBroken = 1;
}

bool RBX::Physics::Weld::weldBroken()
{
	if (!part0 && !part1) return 0;
	return (part0->body->body != weldBody || part1->body->body != weldBody);
}

dBodyID RBX::Physics::Weld::getExistingBody(RBX::PVInstance* p0)
{
	RBX::Instance* data;
	dGeomID geom;

	if (!p0->body->body)
		return 0;

	geom = p0->body->geom[0];

	data = (RBX::Instance*)dBodyGetData(dGeomGetBody(geom));

	if (!data)
		return 0;

	if (data->getClassName() == "Weld")
	{
		return ((Weld*)(data))->weldBody;
	}

	return 0;
}

void RBX::Physics::Weld::createWeld()
{
	if (!part0->body->body || !part1->body->body)
		return;

	if (weldJnt)
		return;

	if (!weldBody)
	{
		const float* part0_pos;
		const float* part1_pos;

		const float* part0_rot;
		const float* part1_rot;

		dGeomID geom0;
		dGeomID geom1;

		geom0 = part0->body->geom[0];
		geom1 = part1->body->geom[0];

		oPart0 = part0->body->body;
		oPart1 = part1->body->body;

		if (!geom0 || !geom1)
			return;

		part0_pos = dGeomGetPosition(geom0);
		part1_pos = dGeomGetPosition(geom1);

		part0_rot = dGeomGetRotation(geom0);
		part1_rot = dGeomGetRotation(geom1);

		weldBody = getExistingBody(part1);

		if (!weldBody)
			weldBody = getExistingBody(part0);

		if(!weldBody)
			weldBody = dBodyCreate(RBX::RunService::singleton()->getPhysics()->physWorld);

		dMass mass0;
		dMass mass1;

		dBodyGetMass(part0->body->body, &mass0);
		dBodyGetMass(part1->body->body, &mass1);

		dMassAdd(&mass0, &mass1);
		
		dBodySetMass(weldBody, &mass0);
		dBodySetData(weldBody, this);

		dBodySetLinearVel(weldBody, 0, 0, 0);

		dGeomSetBody(geom0, weldBody);
		dGeomSetBody(geom1, weldBody);

		dGeomClearOffset(geom0);
		dGeomClearOffset(geom1);

		dGeomSetOffsetPosition(geom0, part0_pos[0], part0_pos[1], part0_pos[2]);
		dGeomSetOffsetPosition(geom1, part1_pos[0], part1_pos[1], part1_pos[2]);

		dGeomSetOffsetRotation(geom0, part0_rot);
		dGeomSetOffsetRotation(geom1, part1_rot);

		part0->body->body = weldBody;
		part1->body->body = weldBody;
	}
}
