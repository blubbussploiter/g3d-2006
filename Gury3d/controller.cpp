#include "controller.h"

std::vector<RBX::Controller*> controllers = std::vector<RBX::Controller*>();

void RBX::Controller::handleInput(UserInput* ui)
{

	if (ui->keyDown(SDLK_w))
	{
		direction = MovementDirections::Forward;
	}
	if (ui->keyDown(SDLK_s))
	{
		direction = MovementDirections::Backwards;
	}

	if (ui->keyDown(SDLK_a))
	{
		switch (direction)
		{
		case Forward: direction = ForwardLeft; break;
		case Backwards: direction = BackwardsLeft; break;
		default: {	direction = MovementDirections::Left; break; }
		}
	}
	if (ui->keyDown(SDLK_d))
	{
		switch (direction)
		{
		case Forward: direction = ForwardRight; break;
		case Backwards: direction = BackwardsRight; break;
		default: {	direction = MovementDirections::Right; break; }
		}
	}
	if (ui->keyPressed(SDLK_SPACE))
		direction = MovementDirections::Jump;

	if (ui->keyReleased(SDLK_w) ||
		ui->keyReleased(SDLK_a) ||
		ui->keyReleased(SDLK_s) ||
		ui->keyReleased(SDLK_d))
		isMoving = false;

	if (ui->keyDown(SDLK_w) ||
		ui->keyDown(SDLK_a) ||
		ui->keyDown(SDLK_s) ||
		ui->keyDown(SDLK_d) ||
		ui->keyPressed(SDLK_SPACE))
		isMoving = true;
}

void RBX::updateControllers(UserInput* ui)
{
	for (unsigned int i = 0; i < controllers.size(); i++)
	{
		RBX::Controller* c = controllers.at(i);
		if (c && !c->disabled())
		{
			c->handleInput(ui);
			c->mv_update();
			c->move();
		}
	}
}

void RBX::addController(Controller* c)
{
	controllers.push_back(c);
}
