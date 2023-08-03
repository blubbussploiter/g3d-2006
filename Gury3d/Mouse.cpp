#include "Mouse.h"
#include "camera.h"
#include "players.h"
#include "humanoid.h"
#include "ray.h"
#include "strings.h"

unsigned int Rendering::mouse_glid;
bool Rendering::isOverGuiObject;

float RBX::Mouse::x = 0;
float RBX::Mouse::y = 0;

Vector3 RBX::Mouse::dir = Vector3::zero();
Vector3 RBX::Mouse::hitWorld = Vector3::zero();

RBX::PVInstance* RBX::Mouse::target = 0;
TextureRef Rendering::cursor_custom = 0;

TextureRef cursor_far;
TextureRef cursor_close;

float Rendering::szx = 150, Rendering::szy = 150;

void RBX::Mouse::update(float _x, float _y)
{
	x = _x;
	y = _y;
}

bool Rendering::shouldRenderAsFar() /* check distance later, new member, ->getClosestPoint for roblox ray */ /* FIX THIS */
{
	//RBX::PVInstance* o = RBX::Mouse::getTarget();
	//if (o)
	//	printf("o = %s\n", o->getName().c_str());
	//return (!o);
	return 0;
}

RBX::PVInstance* RBX::Mouse::getTarget(RBX::PVInstance* ignorePart) /* ignore part for dragger tool */
{
	RBX::Camera* camera = RBX::Camera::singleton();
	Ray ray;

	ray = camera->camera->worldRay(x, y, camera->rd->getViewport());
	target = RBX::World::getPartFromG3DRay(ray);

	return target;
}

void Rendering::renderCursor(UserInput* ui, RenderDevice* rd)
{
	Vector2 mpos;
	Vector2 sz;

	mpos = ui->mouseXY();
	sz = Vector2(szx, szy);

	mpos.x -= sz.x /2;
	mpos.y -= sz.y /2;

	if (cursor_far.isNull() && cursor_close.isNull())
	{
		cursor_far = Texture::fromFile(GetFileInPath("/content/textures/ArrowFarCursor.png"));
		cursor_close = Texture::fromFile(GetFileInPath("/content/textures/ArrowCursor.png"));
	}

	mouse_glid = cursor_close->openGLID();

	if (shouldRenderAsFar())
		mouse_glid = cursor_far->openGLID();

	if (!cursor_custom.isNull())
		mouse_glid = cursor_custom->openGLID();

	rd->push2D();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, mouse_glid);
	glBegin(GL_QUADS);

	glTexCoord2d(0.0, 0.0);
	glVertex2f(mpos.x, mpos.y);

	glTexCoord2d(1.0, 0.0);
	glVertex2f(mpos.x + sz.x, mpos.y);

	glTexCoord2d(1.0, 1.0);
	glVertex2f(mpos.x + sz.x, mpos.y + sz.y);

	glTexCoord2d(0.0, 1.0);
	glVertex2f(mpos.x, mpos.y + sz.y);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	rd->pop2D();
}