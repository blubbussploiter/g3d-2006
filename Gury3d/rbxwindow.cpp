
#include "rbxwindow.h"
#include "datamodel.h"
#include "camera.h"

LONG oldWndProc;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_MOUSEWHEEL:
	{
		short d = HIWORD(wParam);
		RBX::Camera::singleton()->cam_zoom(d > 0);
		return 0;
	}
	case WM_CLOSE:
	case WM_QUIT:
	{
		RBX::Datamodel::getDatamodel()->close();
		FreeConsole();
		break;
	}
	}
	return CallWindowProcA((WNDPROC)oldWndProc, hwnd, msg, wParam, lParam);
}