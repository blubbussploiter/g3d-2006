#ifndef BTHELP_H
#define BTHELP_H

#include <G3DAll.h>
#include "btBulletDynamicsCommon.h"

#define g3Vector3(g) (btVector3(g.x, g.y, g.z))
#define b3Vector3(b) (Vector3(b.x(), b.y(),b.z()))

static CoordinateFrame ToCoordinateFrame(btTransform transform)
{
	btVector3 physPosition;
	btMatrix3x3 physRotation;

	Matrix3 rotation;
	Vector3 position;

	float* matrix = new float[12];

	physRotation = transform.getBasis();
	physPosition = transform.getOrigin();

	position = b3Vector3(physPosition);
	physRotation.getOpenGLSubMatrix(matrix);

	rotation = Matrix3(matrix[0], matrix[1], matrix[2],
		matrix[4], matrix[5], matrix[6],
		matrix[8], matrix[9], matrix[10]);

	return CoordinateFrame(rotation, position);
}

static btTransform ToTransform(CoordinateFrame cframe)
{
	btVector3 physPosition;
	btQuaternion physRotation;

	Vector3 position;
	float y, p, r;

	cframe.rotation.toEulerAnglesYXZ(y, p, r);
	position = cframe.translation;

	physPosition = g3Vector3(position);
	physRotation = btQuaternion();

	physRotation.setEuler(y, p, r);
	return btTransform(physRotation, physPosition);
}

#endif