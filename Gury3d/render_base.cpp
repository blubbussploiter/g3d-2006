#include "render_base.h"
#include "part.h"

#include "GuiRoot.h"
#include "Mouse.h"

#include "players.h"
#include "camera.h"

#include "workspace.h"
#include "sounds.h"

#include "runservice.h"
#include "serializer.h"

#include "rbxwindow.h"
#include "selection.h"

#include "clonebin.h"
#include "gametool.h"

#pragma comment(lib, "fmod_vc.lib")
Rendering::G3DApp* Rendering::global_app = 0;

void Rendering::G3DApplet::doGraphics()
{

	RBX::Network::Player* plr;

	app->renderDevice->clear();

	RBX::Camera::singleton()->update(app);
	app->renderDevice->setProjectionAndCameraMatrix(app->debugCamera);

	app->renderDevice->setShadeMode(RenderDevice::ShadeMode::SHADE_SMOOTH);

	LightingParameters params = LightingParameters(toSeconds(14, 0, PM));
	sky->render(params);

	RBX::Selection::renderSelection(app->renderDevice);

	RBX::Workspace::singleton()->update();
	RBX::Workspace::singleton()->render(app->renderDevice);

	RBX::Network::getPlayers()->updatePlayerList();

	RBX::SoundService::singleton()->update();
	sky->renderLensFlare(params);

	RBX::Selection::update(app->userInput);

	RBX::Gui::singleton()->render(app->renderDevice);
	Rendering::renderCursor(app->userInput, app->renderDevice);

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

	if (ui->appHasFocus())
	{
		ui->window()->setMouseVisible(0);
		RBX::Gui::singleton()->doButtonLogic(ui, app->renderDevice);
		hook_mouse();
		if (Movement::getCurrentController())
		{
			Movement::getCurrentController()->handleInput(ui);
			Movement::getCurrentController()->mv_update();
			Movement::getCurrentController()->move();
		}
	}
	else
		ui->window()->setMouseVisible(1);

	if (RBX::RunService::singleton())
		RBX::RunService::singleton()->update();

	if (!RBX::Network::getPlayers() || (RBX::Network::getPlayers() && !RBX::Network::getPlayers()->localPlayer))
		return;

	RBX::Network::Player* plr = RBX::Network::getPlayers()->localPlayer;

	plr->backpack->keypress(ui);
	RBX::Mouse::update(app->userInput->getMouseX(), app->userInput->getMouseY());

	if (ui->keyPressed(SDLK_BACKSPACE))
		plr->disposeActiveBin();

	if (ui->keyPressed(SDLK_ESCAPE))
		if(plr->character)
			plr->character->breakJoints();
}

void Rendering::G3DApp::main()
{

	RBX::Camera::cameraInit(&debugCamera, renderDevice);
	Movement::setCurrentController(RBX::Camera::singleton());

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
	RBX::Network::getPlayers()->createLocalPlayer(0);
	RBX::Network::getPlayers()->localPlayer->setName("whirlpool");

	RBX::Network::getPlayers()->localPlayer->loadCharacter();
	(new RBX::CloneBin())->setParent(RBX::Network::getPlayers()->localPlayer->backpack);
	(new RBX::GameTool())->setParent(RBX::Network::getPlayers()->localPlayer->backpack);
	RBX::RunService::singleton()->run();

	RBX::Serializer::load(GetFileInPath("/sunset.rbxl"));
	//RBX::Serializer::load("D:\\sh3.rbxl");
	//RBX::Serializer::load("D:\\hauntedmound.rbxl");
	//RBX::Serializer::load("D:\\blank.rbxl");

	G3DApplet* a = new G3DApplet(this);
	a->setDesiredFrameRate(30);
	(a)->run();
}