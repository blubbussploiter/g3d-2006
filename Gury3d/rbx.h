/* guryblox 2006 recreation by whirlpool okay ehrr */
/* version 2.1.1 source */

#ifndef RBX_H
#define RBX_H

#include <G3DAll.h>

//#define DEBUGGING

namespace RBX
{
	static std::string Format(std::string fmt, ...)
	{
		va_list argPtr;
		std::string _fmt;

		va_start(argPtr, fmt);
		_fmt = G3D::vformat(fmt.c_str(), argPtr);
		va_end(argPtr);

		return _fmt;
	}
}

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
		void onSimulation(RealTime rdt, SimTime sdt, SimTime idt);

		G3DApplet(G3DApp* app) : GApplet(app), app(app) { sky = Sky::create(app->renderDevice, app->dataDir + "sky/");  }
	};
}

#endif