#include "GuiRoot.h"

#include "rbxwindow.h"
#include "3DStudio.h"

bool inFullScreen = 0;
int x, y, width, height;

void setFullScreen(bool s)
{
	HDC windowHDC;
	int fullscreenWidth, fullscreenHeight, colourBits, refreshRate;

	if (!x && !y && !width && !height)
	{
		CRect rc;

		theApp.GetMainWnd()->GetWindowRect(rc);

		width = rc.Width();
		height = rc.Height();
	}

	windowHDC = GetWindowDC(theApp.GetMainWnd()->GetSafeHwnd());

	fullscreenWidth = GetDeviceCaps(windowHDC, DESKTOPHORZRES);
	fullscreenHeight = GetDeviceCaps(windowHDC, DESKTOPVERTRES);
	colourBits = GetDeviceCaps(windowHDC, BITSPIXEL);
	refreshRate = GetDeviceCaps(windowHDC, VREFRESH);

	if (s)
	{
		enterFullscreen(WindowFromDC(windowHDC), fullscreenWidth, fullscreenHeight, colourBits, refreshRate);
	}
	else
	{
		exitFullscreen(WindowFromDC(windowHDC), 0, 0, width, height, 0, 0);
	}
}

void RBX::Gui::MenuBar::onExitBtnClick(RBX::Gui::GuiButton* btn)
{
	
}

void RBX::Gui::MenuBar::onFullscreenBtnClick(GuiButton* btn)
{

	if (!inFullScreen)
	{
		inFullScreen = 1;
		btn->title = "x Fullscreen";
	}
	else
	{
		inFullScreen = 0;
		btn->title = "Fullscreen";
	}

	setFullScreen(inFullScreen);

}
