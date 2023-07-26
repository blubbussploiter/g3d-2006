
#include "rbx.h"
#include "part.h"

#include "GuiRoot.h"
#include "Mouse.h"

#include "players.h"
#include "camera.h"
#include "jointservice.h"

#include "workspace.h"
#include "sounds.h"

#include "runservice.h"
#include "serializer.h"

#include "rbxwindow.h"
#include "selection.h"

#include "clonebin.h"
#include "gametool.h"

#include "datamodel.h"
#include "lighting.h"

#pragma comment(lib, "fmod_vc.lib")
Rendering::G3DApp* Rendering::global_app = 0;

void Rendering::G3DApplet::onSimulation(RealTime rdt, SimTime sdt, SimTime idt)
{
	if (RBX::RunService::singleton()->isRunning)
	{
		RBX::Datamodel::getDatamodel()->step();
	}
	app->userInput->window()->setCaption(RBX::Format("Gury3d - FPS: %g, Geoms: %d, Constraints: %d", app->renderDevice->getFrameRate(), RBX::RunService::singleton()->getPhysics()->getNumberOfGeoms(), RBX::RunService::singleton()->getPhysics()->getNumberOfConstraints()));
}

void Rendering::G3DApplet::doGraphics()
{
	RBX::Network::Player* plr;

	/* update physics per render.. yada yada */

	app->renderDevice->clear();

	RBX::Camera::singleton()->update(app);
	app->renderDevice->setProjectionAndCameraMatrix(app->debugCamera);

	app->renderDevice->setShadeMode(RenderDevice::ShadeMode::SHADE_SMOOTH);

	sky->render(RBX::Lighting::singleton()->getParameters());

	RBX::Selection::renderSelection(app->renderDevice);

	RBX::SoundService::singleton()->update();
	RBX::Datamodel::getDatamodel()->workspace->render(app->renderDevice);

	RBX::Network::getPlayers()->updatePlayerList();

	RBX::Selection::update(app->userInput);

	RBX::Datamodel::getDatamodel()->guiRoot->render(app->renderDevice);
	Rendering::renderCursor(app->userInput, app->renderDevice);

	sky->renderLensFlare(RBX::Lighting::singleton()->getParameters());

	if (RBX::Network::getPlayers())
	{
		plr = RBX::Network::getPlayers()->localPlayer;

		if (plr)
		{
			RBX::Network::getPlayers()->localPlayer->backpack->updateGui(app->renderDevice, app->userInput);
			if (plr->activeBin)
				plr->activeBin->update(app->renderDevice, app->userInput);
		}
	}

}

void Rendering::G3DApplet::doLogic()
{
	UserInput* ui;
	ui = app->userInput;

	if (ui->keyPressed(SDLK_o))
		RBX::Camera::singleton()->cam_zoom(0);
	if(ui->keyPressed(SDLK_i))
		RBX::Camera::singleton()->cam_zoom(1); 

	//if (ui->keyPressed(SDLK_F1))
	//{
	//	if (RBX::RunService::singleton()->isRunning)
	//		RBX::RunService::singleton()->stop();
	//	else
	//		RBX::RunService::singleton()->run();
	///}

	if (ui->appHasFocus())
	{
		ui->window()->setMouseVisible(0);
		RBX::Gui::singleton()->doButtonLogic(ui, app->renderDevice);

		RBX::updateControllers(app->userInput);

		hook_mouse();
	}
	else
		ui->window()->setMouseVisible(1);

	if (RBX::Network::getPlayers() && RBX::Network::getPlayers()->localPlayer)
	{
		RBX::Network::Player* plr = RBX::Network::getPlayers()->localPlayer;

		plr->backpack->keypress(ui);
		RBX::Mouse::update(app->userInput->getMouseX(), app->userInput->getMouseY());

		if (ui->keyPressed(SDLK_BACKSPACE))
			plr->disposeActiveBin();

		if (plr->character)
		{
			if (ui->keyPressed(SDLK_ESCAPE))
				plr->character->breakJoints();

			plr->setAsController();
		}

	}

}

void Rendering::G3DApp::main()
{

	RBX::Camera::cameraInit(&debugCamera, renderDevice);
	RBX::addController(RBX::Camera::singleton());

	RBX::RunService::singleton();

	Rendering::global_app = this;

	RBX::Gui::GuiList* playerList;

	RBX::Gui::GuiButton* toolboxBtn;
	RBX::Gui::GuiButton* editModeBtn;
	RBX::Gui::GuiButton* helpBtn;
	RBX::Gui::GuiButton* fullscreenBtn;
	RBX::Gui::GuiButton* exitBtn;

	RBX::Gui::GuiImageButton* arrowDownBtn;
	RBX::Gui::GuiImageButton* arrowUpBtn;
	RBX::Gui::GuiImageButton* zoomInBtn;
	RBX::Gui::GuiImageButton* zoomOutBtn;

	/* Menu bar */

	toolboxBtn = new RBX::Gui::GuiButton();
	toolboxBtn->position = Vector2(0, 0);
	toolboxBtn->size = Vector2(120, 20);
	toolboxBtn->title = "Toolbox";
	toolboxBtn->disabled = true;

	editModeBtn = new RBX::Gui::GuiButton();
	editModeBtn->position = Vector2(120, 0);
	editModeBtn->size = Vector2(120, 20);
	editModeBtn->title = "Edit Mode";
	editModeBtn->disabled = true;

	helpBtn = new RBX::Gui::GuiButton();
	helpBtn->position = Vector2(240, 0);
	helpBtn->size = Vector2(120, 20);
	helpBtn->title = "Help...";

	fullscreenBtn = new RBX::Gui::GuiButton();
	fullscreenBtn->position = Vector2(360, 0);
	fullscreenBtn->size = Vector2(120, 20);
	fullscreenBtn->title = "Fullscreen";
	fullscreenBtn->onClick = RBX::Gui::MenuBar::onFullscreenBtnClick;

	exitBtn = new RBX::Gui::GuiButton();
	exitBtn->position = Vector2(480, 0);
	exitBtn->size = Vector2(120, 20);
	exitBtn->title = "Exit";
	exitBtn->onClick = RBX::Gui::MenuBar::onExitBtnClick;

	/* Playerlist */

	playerList = new RBX::Gui::GuiList();
	playerList->position = Vector2(130, 10);
	playerList->listTitle = "Player List";
	playerList->size = Vector2(130, 0);
	playerList->visible = false;
	playerList->alignRight = true;

	/* CameraPanMenu */

	arrowUpBtn = new RBX::Gui::GuiImageButton();
	arrowUpBtn->alignBottomRight = true;
	arrowUpBtn->position = Vector2(70, 75);
	arrowUpBtn->size = Vector2(30, 30);
	arrowUpBtn->texture = Texture::fromFile(GetFileInPath("/content/textures/CameraTiltUp.png"));
	arrowUpBtn->hoverTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraTiltUp_ovr.png"));
	arrowUpBtn->clickTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraTiltUp_dn.png"));
	arrowUpBtn->onClick = RBX::Gui::CameraPanMenu::onArrowUpBtnClick;

	arrowDownBtn = new RBX::Gui::GuiImageButton();
	arrowDownBtn->alignBottomRight = true;
	arrowDownBtn->position = Vector2(70, 40);
	arrowDownBtn->size = Vector2(30, 30);
	arrowDownBtn->texture = Texture::fromFile(GetFileInPath("/content/textures/CameraTiltDown.png"));
	arrowDownBtn->hoverTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraTiltDown_ovr.png"));
	arrowDownBtn->clickTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraTiltDown_dn.png"));
	arrowDownBtn->onClick = RBX::Gui::CameraPanMenu::onArrowDwnBtnClick;

	zoomInBtn = new RBX::Gui::GuiImageButton();
	zoomInBtn->alignBottomRight = true;
	zoomInBtn->position = Vector2(40, 75);
	zoomInBtn->size = Vector2(30, 30);
	zoomInBtn->texture = Texture::fromFile(GetFileInPath("/content/textures/CameraZoomIn.png"));
	zoomInBtn->hoverTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraZoomIn_ovr.png"));
	zoomInBtn->clickTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraZoomIn_dn.png"));
	zoomInBtn->onClick = RBX::Gui::CameraPanMenu::onZoomInBtnClick;

	zoomOutBtn = new RBX::Gui::GuiImageButton();
	zoomOutBtn->alignBottomRight = true;
	zoomOutBtn->position = Vector2(40, 40);
	zoomOutBtn->size = Vector2(30, 30);
	zoomOutBtn->texture = Texture::fromFile(GetFileInPath("/content/textures/CameraZoomOut.png"));
	zoomOutBtn->hoverTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraZoomOut_ovr.png"));
	zoomOutBtn->clickTexture = Texture::fromFile(GetFileInPath("/content/textures/CameraZoomOut_dn.png"));
	zoomOutBtn->onClick = RBX::Gui::CameraPanMenu::onZoomOutBtnClick;

	RBX::Gui::singleton()->add(toolboxBtn);
	RBX::Gui::singleton()->add(editModeBtn);
	RBX::Gui::singleton()->add(helpBtn);
	RBX::Gui::singleton()->add(fullscreenBtn);
	RBX::Gui::singleton()->add(exitBtn);
	RBX::Gui::singleton()->add(playerList);
	RBX::Gui::singleton()->add(arrowDownBtn);
	RBX::Gui::singleton()->add(arrowUpBtn);
	RBX::Gui::singleton()->add(zoomInBtn);
	RBX::Gui::singleton()->add(zoomOutBtn);

	RBX::Network::getPlayers()->setPlayerList(playerList);
	//RBX::Network::getPlayers()->createLocalPlayer(0);
	//RBX::Network::getPlayers()->localPlayer->setName("Player0");

	RBX::Datamodel::getDatamodel()->runService->run();
	//RBX::Network::getPlayers()->localPlayer->loadCharacter();
	//(new RBX::CloneBin())->setParent(RBX::Network::getPlayers()->localPlayer->backpack);
	//(new RBX::GameTool())->setParent(RBX::Network::getPlayers()->localPlayer->backpack);

	//RBX::Serializer::load(GetFileInPath("/sunset.rbxl"));
	//RBX::Serializer::load("D:\\pirate.rbxl");
	//RBX::Serializer::load("D:\\sh3.rbxl");
	//RBX::Serializer::load("D:\\hauntedmound.rbxl");
	//RBX::Serializer::load("D:\\hauntedmound.rbxl");
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\829.rbxmx");
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\sharkrock.rbxl");
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\sunset.rbxl");
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\crossroads.rbxl");
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\sh3.rbxl");
	//RBX::Datamodel::getDatamodel()->loadContent(GetFileInPath("/g3dplace.rbxl"));
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\bullet-test.rbxl");
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\chaos.rbxlx");
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\g3dplace.rbxl");
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\pirate.rbxl");
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\inhalf.rbxl");
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\hq.rbxl");
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\The-Divide.rbxlx");
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\guy.rbxl");
	RBX::Datamodel::getDatamodel()->loadContent("D:\\blank.rbxl");
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\crossroads.rbxl");
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\mountain.rbxl");
	//RBX::Datamodel::getDatamodel()->loadContent("D:\\multiplier.rbxl");

	//RBX::Camera::singleton()->zoomExtents();

	G3DApplet* a = new G3DApplet(this);
	a->setDesiredFrameRate(30);
	(a)->run();
}