#ifndef HUMANOID_H
#define HUMANOID_H

#include "part.h"
#include "snaps.h"

#include <G3DAll.h>

namespace RBX
{
	class Humanoid : public RBX::Instance
	{
	private:

		enum WalkMode
		{
			DIRECTION_JUMP,
			DIRECTION_MOVE,
		};

		Vector3 walkDirection;
		Vector3 walkRotationVelocity;
		
		WalkMode walkMode;

	public:

		RBX::PVInstance* humanoidRootPart;
		RBX::PVInstance* humanoidHead;

		float health;
		float maxHealth;
		float jmpPower;
		bool isDead;

		Humanoid()
		{
			health = 100.0f;
			maxHealth = 100.0f;
			jmpPower = 30;
			setClassName("Humanoid");
			setName("Humanoid");
		}

		bool isGrounded();
		bool isFalling();
		bool isInAir();
		bool isJoined() { return 1;/* update for jointservice */ }

		bool checkHumanoidAttributes() { return (humanoidHead && humanoidRootPart && (humanoidRootPart->body && humanoidHead->body) && health > 0); }

		void balance();
		void onDied();
		bool occluding(RBX::PVInstance* p, Vector3 dir);
		/* corrects humanoidRootPart and humanoidHead */
		void correctHumanoidAttributes();
		void correctOccludingSlope();
		/* get Humanoid from model if its a character (has humanoid) */
		static Humanoid* modelIsCharacter(RBX::Instance* testModel);
		/* sets humanoidRootPart and humanoidHead accordingly */
		void setHumanoidAttributes();
		void setWalkDirection(Vector3 walkDir);
		void setJump();
		void step();
		virtual ~Humanoid() {}
	};
}
#endif