#include "GuiRoot.h"
#include "render_base.h"

#include "rbxwindow.h"

bool inFullScreen = 0;
int x, y, width, height;

void RBX::Gui::MenuBar::onExitBtnClick(RBX::Gui::GuiButton* btn)
{
	exit(0);
}

void RBX::Gui::MenuBar::onFullscreenBtnClick(GuiButton* btn)
{
	HDC windowHDC;
	int fullscreenWidth, fullscreenHeight, colourBits, refreshRate;

	if (!x && !y && !width && !height)
	{
		width = Rendering::global_app->renderDevice->getWidth();
		height = Rendering::global_app->renderDevice->getHeight();
	}

	windowHDC = Rendering::global_app->renderDevice->getWindowHDC();

	fullscreenWidth = GetDeviceCaps(windowHDC, DESKTOPHORZRES);
	fullscreenHeight = GetDeviceCaps(windowHDC, DESKTOPVERTRES);
	colourBits = GetDeviceCaps(windowHDC, BITSPIXEL);
	refreshRate = GetDeviceCaps(windowHDC, VREFRESH);

	if (!inFullScreen)
	{
		enterFullscreen(WindowFromDC(windowHDC), fullscreenWidth, fullscreenHeight, colourBits, refreshRate);
		inFullScreen = 1;
		btn->title = "x Fullscreen";
	}
	else
	{
		exitFullscreen(WindowFromDC(windowHDC), 0,0, width, height, 0, 0);
		inFullScreen = 0;
		btn->title = "Fullscreen";
	}
}
