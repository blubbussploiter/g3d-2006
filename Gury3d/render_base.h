/* guryblox 2006 recreation by whirlpool okay ehrr */
/* version 2.1.1 source */

#ifndef RENDER_BASE_H
#define RENDER_BASE_H

#include <G3DAll.h>

//#define DEBUGGING

namespace Rendering
{
	class G3DApp;
	class G3DApplet;

	extern G3DApp* global_app;

	class G3DApp : public GApp {
	public:
		G3DApp(const GAppSettings& settings) : GApp(settings) {};
		void main();
	};

	class G3DApplet : public GApplet {
	private:
		G3DApp* app;
		SkyRef sky;
	public:

		void doGraphics();
		void doLogic();

		G3DApplet(G3DApp* app) : GApplet(app), app(app) { sky = Sky::create(app->renderDevice, app->dataDir + "sky/");  }
	};
}

#endif