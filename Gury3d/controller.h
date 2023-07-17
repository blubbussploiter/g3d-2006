#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <G3DAll.h>

namespace Movement
{
	enum MovementDirections
	{
		Idle,
		Forward,
		Backwards,
		Left,
		Right,
		Jump
	};

	class Controller
	{
	private:
		bool isMoving;
		float speed;
		MovementDirections direction;
	public:
		void handleInput(UserInput* ui);
		float getSpeed() { return speed * 0.4; }
		void setSpeed(float s) { speed = s; }
		bool moving() { return isMoving; }
		void setmoving(bool m) { isMoving = m; }
		void setdir(MovementDirections d) { direction = d; }
		virtual void move() {};
		virtual void mv_update() {}
		MovementDirections dir() {return direction;}
		Controller() : speed(1) {}
	};

	extern Controller* controller;
	Controller* getCurrentController();
	void setCurrentController(Controller* c);
}
#endif