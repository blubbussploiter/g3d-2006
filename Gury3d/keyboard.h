#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "controller.h"
#include "keyboard.h"

namespace RBX
{

	class KeyboardController : public RBX::Controller
	{
	private:
		Instances* motors;
	public:
		void move();

		void mv_update();
		void handleInput(UserInput* ui);

		void moveCharacter(RBX::Instance* character);
		void bindCharacter(RBX::Instance* character);

		void bindMotors(RBX::Instance* character);

		KeyboardController()
		{
			motors = new Instances();
		}
	};
}

#endif