#ifndef WELDS_H
#define WELDS_H
#include <ode/ode.h>
#include "render_shapes.h"

namespace RBX
{
	namespace Physics
	{
		class Weld : public RBX::Instance
		{
		private:
			dJointID weldJnt;
			dBodyID weldBody;
			dBodyID oPart0, oPart1;
		public:
			RBX::PVInstance* part0;
			RBX::PVInstance* part1;
			bool hasBeenCalledBroken;
			void weld(RBX::PVInstance* p0, RBX::PVInstance* p1);
			void breakWeld();
			dBodyID getExistingBody(RBX::PVInstance* p0);
			void createWeld();
			bool weldBroken();
			Weld()
			{
				setClassName("Weld");
				setName("Weld");
				part0 = 0;
				part1 = 0;
				weldJnt = 0;
				isAffectedByPhysics = 1;
			}
			~Weld() { breakWeld(); }
		};
	}
}
#endif