#ifndef SCENE_H
#define SCENE_H

#include "render.h"
#include "runservice.h"

namespace RBX
{
	class Scene : public RBX::Instance
	{
	private:
		std::vector<RBX::Render::Renderable*> objects;
	public:
		void onWorkspaceDescendentAdded(RBX::Render::Renderable* descendent);
		void updatePhysicsObjects();
		void baseRender(RenderDevice* rd, bool(*rule)(RBX::Render::Renderable*), void(*render)(RBX::Render::Renderable*, RenderDevice*));
		void opaquePass(RenderDevice* rd);
		void transparentPass(RenderDevice* rd);
		void darkPass(RenderDevice* rd);
		static Scene* singleton();
		std::vector<RBX::Render::Renderable*> getArrayOfObjects();
	};
}

#endif