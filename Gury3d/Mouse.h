#ifndef MOUSE_H
#define MOUSE_H

#include <G3DAll.h>
#include "render_shapes.h"

namespace Rendering
{
	extern TextureRef cursor_custom;
	extern float szx, szy;
	extern unsigned int mouse_glid;
	extern bool isOverGuiObject;
	extern bool shouldRenderAsFar();
	extern void renderCursor(UserInput* ui, RenderDevice* rd);
}

namespace RBX
{
	class Mouse
	{
	private:
		static float x, y;
		static Vector3 hitWorld, dir;
	public:
		static RBX::PVInstance* target;
		static RBX::PVInstance* getTarget(RBX::PVInstance* ignorePart=0);
		static Vector3 getDir() { getTarget(); return dir; }
		static Vector3 getHit() { getTarget(); return hitWorld; }
		static void update(float x, float y);
	};
}
#endif