#include "view.h"
#include "lighting.h"

RBX::View* rbx_view;

void RBX::View::onWorkspaceDescendentAdded(RBX::Instance* descendent)
{
	if (descendent->isRenderable)
	{
		RBX::Scene::singleton()->onWorkspaceDescendentAdded((RBX::Render::Renderable*)descendent);
	}
}

void RBX::View::onWorkspaceDescendentRemoved(RBX::Instance* descendent)
{

}

void RBX::View::render(RenderDevice* rd)
{
	rd->pushState();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	RBX::Lighting::singleton()->begin(rd);

	rd->setPolygonOffset(-0.2f);

	RBX::Scene::singleton()->opaquePass(rd);
	RBX::Scene::singleton()->transparentPass(rd);

	RBX::Lighting::singleton()->end(rd);

	RBX::Scene::singleton()->darkPass(rd);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	rd->popState();
}

RBX::View* RBX::View::singleton()
{
	if (!rbx_view) rbx_view = new RBX::View();
	return rbx_view;
}
