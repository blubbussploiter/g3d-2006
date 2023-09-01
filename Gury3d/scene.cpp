#include "scene.h"

#include "pvinstance.h"
#include "datamodel.h"

RBX::Scene* RBX::Scene::singleton()
{
	return RBX::Datamodel::getDatamodel()->scene;
}

std::vector<RBX::Render::Renderable*> RBX::Scene::getArrayOfObjects()
{
	return objects;
}

bool opaqueRule(RBX::Render::Renderable* r)
{
	return (r->transparency == 0);
}

bool transparentRule(RBX::Render::Renderable* r)
{
	return (r->transparency > 0 && r->transparency < 1);
}

bool physicsRule(RBX::Render::Renderable* r)
{
	return r->isAffectedByPhysics;
}

void renderRenderable(RBX::Render::Renderable* r, RenderDevice* rd)
{
	r->render(rd);
}

void render3DSurface(RBX::Render::Renderable* r, RenderDevice* rd)
{
	r->render3DSurfaces(rd);
}

void update3DObject(RBX::Render::Renderable* r, RenderDevice* rd)
{
	RBX::PVInstance* pv = (RBX::PVInstance*)(r);
	if (pv->body)
	{
		RBX::RunService::singleton()->getPhysics()->createBody(pv);
	}
}

void RBX::Scene::baseRender(RenderDevice* rd, bool(*rule)(RBX::Render::Renderable*), void(*render)(RBX::Render::Renderable*, RenderDevice*))
{
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		RBX::Render::Renderable* r = objects.at(i);
		if (rule(r))
		{
			render(r, rd);
		}
	}
}

void RBX::Scene::updatePhysicsObjects()
{
	baseRender(0, physicsRule, update3DObject);
}

void RBX::Scene::opaquePass(RenderDevice* rd)
{
	baseRender(rd, opaqueRule, renderRenderable);
}

void RBX::Scene::transparentPass(RenderDevice* rd)
{
	baseRender(rd, transparentRule, renderRenderable);
}

void RBX::Scene::darkPass(RenderDevice* rd)
{
	baseRender(rd, opaqueRule, render3DSurface);
}

void RBX::Scene::onWorkspaceDescendentAdded(RBX::Render::Renderable* descendent)
{
	if (descendent->isRenderable)
	{
		RBX::Render::Renderable* r = static_cast<RBX::Render::Renderable*>(descendent);

		if (r->isSpecialShape)
		{
			RBX::Render::Renderable* p = (RBX::Render::Renderable*)r->getParent();
			if (p->isRenderable)
			{
				p->specialShape = r;
			}
		}

		objects.push_back(r);
	}
}