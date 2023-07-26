#include "GuiRoot.h"

void RBX::Gui::GuiMessage::render(RenderDevice* d)
{
	Vector2 end;
	Vector2 bounds;

	end = Vector2(d->width(), d->height());
	bounds = singleton()->font->get2DStringBounds(text);

	Draw::box(Box(Vector3(0, 0, 0), Vector3(end.x, end.y, 0)), d, Color4(0.5, 0.5, 0.5, 0.3), Color4::CLEAR);
	singleton()->font->draw2D(d, text, Vector2((end.x / 2) - (bounds.x / 2), end.y/2), 12, Color3::white(), Color3::black());
}