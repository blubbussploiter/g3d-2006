#include "appmanager.h"

#include "selection.h"
#include "players.h"

#include "camera.h"
#include "stdout.h"

#include "Mouse.h"

RBX::Datamodel* RBX::Experimental::Application::getDatamodel()
{
	if (!datamodel)
	{
		datamodel = new RBX::Datamodel();
		datamodel->open();
	}
	return datamodel;
}

RBX::Camera* RBX::Experimental::Application::getCamera()
{
	if (!camera)
	{
		camera = new RBX::Camera();
		camera->setCamera(new GCamera());
	}
	return camera;
}

void RBX::Experimental::Application::onFocus()
{
	resizeWithParent();
	window->makeCurrent();
	getDatamodel()->workspace->setCurrentCamera(getCamera());
}

void RBX::Experimental::Application::doUserInput()
{
	GEvent e;

	userInput->beginEvents();

	while (pollEvent(e))
	{
		userInput->processEvent(e);

		msg.message = 0;
		msg.wParam = 0;
		msg.lParam = 0;

	}

	window->pollEvent(e);

	userInput->endEvents();
}

void RBX::Experimental::Application::onSimulation(RealTime rdt, SimTime sdt, SimTime idt)
{
	if (getDatamodel()->runService->isRunning)
	{
		getDatamodel()->step();
	}
	SoundService::singleton()->update();
}

void RBX::Experimental::Application::onGraphics()
{

	renderDevice->clear();
	renderDevice->setProjectionAndCameraMatrix(*camera->getCamera());

	renderDevice->setShadeMode(RenderDevice::ShadeMode::SHADE_SMOOTH);
	sky->render(RBX::Lighting::singleton()->getParameters());

	RBX::Selection::renderSelection(renderDevice);
	RBX::View::singleton()->render(renderDevice);

	RBX::Network::getPlayers()->updatePlayerList();

	renderDevice->push2D();

	getDatamodel()->guiRoot->render(renderDevice);
	Rendering::renderCursor(userInput, renderDevice);

	renderDevice->pop2D();

	sky->renderLensFlare(RBX::Lighting::singleton()->getParameters());
}

void RBX::Experimental::Application::onLogic()
{
	window->setMouseVisible(false);

	if (userInput->keyPressed(SDLK_o))
	{
		RBX::Camera::singleton()->cam_zoom(0);
	}
	if (userInput->keyPressed(SDLK_i))
	{
		RBX::Camera::singleton()->cam_zoom(1);
	}

	getCamera()->update(userInput);

	RBX::ControllerService::singleton()->updateControllers(userInput);
	RBX::Gui::singleton()->doButtonLogic(userInput, renderDevice);
}

void RBX::Experimental::Application::onInit()
{
	RBX::StandardOut::print(RBX::MESSAGE_INFO, "OnInit()");
	
	RBX::AppManager::singleton()->initOneTimeAppliances();
	setWindowLong();

	getCamera();
	getDatamodel();

	RBX::ControllerService::singleton()->addController(RBX::Camera::singleton());

	getDatamodel()->loadContent("D:\\1622_arkitects_place_3-24-2006.rbxl");
	//getDatamodel()->players->createLocalPlayer(0);
	//getDatamodel()->players->localPlayer->loadCharacter();
	//getDatamodel()->runService->run();

}

void RBX::Experimental::Application::mainProcessStep()
{
	RealTime desiredFrameDuration = 1.0 / fps;
	RealTime now = System::time();

	lastTime = now;
	RealTime timeStep = now - lastTime;

	doUserInput();
	onLogic();

	renderDevice->beginFrame();
	onGraphics();
	renderDevice->endFrame();

	double rate = simTimeRate;
	RealTime rdt = timeStep;
	SimTime  sdt = timeStep * rate;
	SimTime  idt = desiredFrameDuration * rate;

	onSimulation(rdt, sdt, idt);

	now = System::time();

	// Compute accumulated time

	System::sleep(max(0.0, desiredFrameDuration - (now - lastWaitTime)));
	lastWaitTime = System::time();
}

void RBX::Experimental::Application::start()
{
	if (!isInitialized)
	{
		RBX::StandardOut::print(RBX::MESSAGE_INFO, "Application::start()");

		isInitialized = true;
		isThinking = true;

		onInit();

	}
}