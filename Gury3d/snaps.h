#ifndef SNAPS_H
#define SNAPS_H

#include "joints.h"

namespace RBX
{
	class SnapJoint : public Joint
	{
	private:
		btRigidBody* rPart0, * rPart1, *_body;
		btCollisionShape* sPart0, * sPart1;
		btCompoundShape* _shape;
		btTransform principal;
		btVector3 bodyInertia;
	public:
		bool hasBeenCalledBroken;
		SnapJoint* getPreExistingSnap(RBX::PVInstance* p0);
		void updateBodies(btCollisionShape* shape);
		void breakJoint();
		void createJoint();
		bool snapBroken();
		SnapJoint()
		{
			part0 = 0;
			part1 = 0;
			_shape = 0;
			sPart0 = 0;
			sPart1 = 0;
			isAffectedByPhysics = 1;
			hasBeenCalledBroken = 0;
			setClassName("Snap");
			setName("Snap");
		}
		~SnapJoint() { breakJoint(); }
	};
}

#endif