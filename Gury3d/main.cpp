#include "render_base.h"

int main()
{
	printf("guryblox version 2.1.0 beta\nby whirlpool\n");
	GAppSettings settings;

	settings.window.width = 820;
	settings.window.height = 480;
	settings.window.resizable = true;
	settings.dataDir = ".\\content\\";

	Rendering::G3DApp app(settings);

	app.debugController.setActive(false);

	app.run();
	return 0;
}