#include "controller.h"

Movement::Controller* Movement::controller = 0;

void Movement::Controller::handleInput(UserInput* ui)
{
	if (ui->keyDown(SDLK_w))
		direction = MovementDirections::Forward;
	if (ui->keyDown(SDLK_s))
		direction = MovementDirections::Backwards;
	if (ui->keyDown(SDLK_a))
		direction = MovementDirections::Left;
	if (ui->keyDown(SDLK_d))
		direction = MovementDirections::Right;
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

Movement::Controller* Movement::getCurrentController()
{
	return controller;
}

void Movement::setCurrentController(Controller* c)
{
	controller = c;
}
