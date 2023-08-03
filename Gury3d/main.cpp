#include "rbx.h"
#include "crashreporter.h"
#include "strings.h"

int main()
{
	RBX::CrashReporter::singleton()->start();

	GAppSettings settings;

	settings.window.width = 820;
	settings.window.height = 480;
	settings.window.resizable = true;
	settings.dataDir = ConFileInPath("\\content\\");

	Rendering::G3DApp app(settings);

	app.debugController.setActive(false);

	app.run();
	return 0;
}