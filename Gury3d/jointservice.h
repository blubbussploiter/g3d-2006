#ifndef JOINTSERVICE_H
#define JOINTSERVICE_H

#include "rbx.h"
#include "runservice.h"

#include "pvenums.h"

#include "instance.h"

#include "snaps.h"
#include "hinge.h"

namespace RBX
{
	class JointService : public RBX::Instance
	{
	public:
		NormalId getIntersectingFace(RBX::PVInstance* p0, RBX::PVInstance* p1);
		void applyJoint(RBX::PVInstance* p0, RBX::PVInstance* p1, NormalId intersect);
		bool areJointed(RBX::PVInstance* p0, RBX::PVInstance* p1);
		void buildJoints(RBX::PVInstance* pv);
		void update();
		static JointService* singleton();
	};
}

#endif