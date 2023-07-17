#include "GuiRoot.h"
#include "camera.h"

#include "sounds.h"

void RBX::Gui::CameraPanMenu::onArrowUpBtnClick(GuiButton* btn)
{
	RBX::Camera::singleton()->tiltDown();
}

void RBX::Gui::CameraPanMenu::onArrowDwnBtnClick(GuiButton* btn)
{
	RBX::Camera::singleton()->tiltUp();
}

void RBX::Gui::CameraPanMenu::onZoomOutBtnClick(GuiButton* btn)
{
	RBX::Camera::singleton()->cam_zoom(0);
}

void RBX::Gui::CameraPanMenu::onZoomInBtnClick(GuiButton* btn)
{
	RBX::Camera::singleton()->cam_zoom(1);
}
