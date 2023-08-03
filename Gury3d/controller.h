#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <G3DAll.h>
#include "instance.h"

namespace RBX
{
	enum MovementDirections
	{
		Idle,
		Forward,
		Backwards,
		Left,
		Right,
		ForwardRight,
		ForwardLeft,
		BackwardsRight,
		BackwardsLeft,
		Jump
	};

	enum ControllerTypes
	{
		None,
		KeyboardLeft,
		KeyboardRight
	};

	class Controller : public RBX::Instance
	{
	private:
		bool isMoving;
		bool isDisabled;
		float speed;
		MovementDirections direction;
	public:
		float getSpeed() { return speed; }
		void setSpeed(float s) { speed = s; }
		bool moving() { return isMoving; }
		void setmoving(bool m) { isMoving = m; }
		void setdir(MovementDirections d) { direction = d; }
		void disable(bool d) { isDisabled = d; }
		bool disabled() { return isDisabled; }
		virtual void handleInput(UserInput* ui);
		virtual void move() {};
		virtual void mv_update() {}
		MovementDirections dir() {return direction;}
		Controller() : speed(0.2f) {}
	};

	void updateControllers(UserInput* ui);
	void addController(Controller* c);
}
#endif