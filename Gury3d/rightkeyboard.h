#ifndef RIGHTKEYBOARD_H
#define RIGHTKEYBOARD_H

#include "controller.h"
#include "keyboard.h"

namespace RBX
{
	class RightKeyboardController : public RBX::KeyboardController
	{
	public:
		void handleInput(UserInput* ui);
	};
}

#endif