#include "rbx.h"
#include "crashreporter.h"

int main()
{
	GAppSettings settings;

	RBX::CrashReporter::singleton()->start();

	settings.window.width = 820;
	settings.window.height = 480;
	settings.window.resizable = true;
	settings.dataDir = ".\\content\\";

	Rendering::G3DApp app(settings);

	app.debugController.setActive(false);

	app.run();
	return 0;
}