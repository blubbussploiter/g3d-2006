#include "render_shapes.h"

void getSurface(RBX::Decal* d, RBX::SURFACES s)
{
	GImage surfaces;
	GImage surface;

	float y = 0;

	std::string fn = GetFileInPath("/content/textures/Surfaces.png");

	surfaces = GImage();
	surfaces.load(fn);

	switch (s)
	{
		case RBX::Weld:
		{
			y = 256;
			break;
		}
		case RBX::Glue:
		{
			y = 384;
			break;
		}
		case RBX::Studs:
		{
			y = 0;
			break;
		}
		case RBX::Inlet:
		{
			y = 128;
			break;
		}
	}

	bool b = surfaces.copySubImage(surface, surfaces, 0, y, 64, 64);
	d->setTexture(Texture::fromGImage(fn, surface, TextureFormat::AUTO, Texture::TILE));
}

std::vector<Vector2> RBX::Surface::getTexCoords(FACES face, Vector3 sz, bool isDrawingDecal)
{
	float u;
	float v;

	switch (face)
	{
		case TOP:
		case BOTTOM:
		{
			u = (sz.x);
			v = (sz.z);
			break;
		}
		case FRONT:
		case BACK:
		{
			u = (-sz.x);
			v = (sz.y);
			break;
		}
		case LEFT:
		case RIGHT:
		{
			u = (sz.z);
			v = (sz.y);
			break;
		}
	}

	if (!isDrawingDecal)
	{
		return
		{
			Vector2(0, u), /* first coord (left top corner?)*/
			Vector2(0, 0), /* second coord (left bottom corner?) */
			Vector2(v, 0), /* third coord (right bottom corner?) */
			Vector2(v, u)  /* last coord (right top corner?) */
		};

	}
	else
	{
		return
		{
			Vector2(u, v),
			Vector2(0, v),
			Vector2(0, 0),
			Vector2(u, 0)
		};
	}
}

void RBX::Surface::setSurfaceType(SURFACES srfc)
{
	surface = srfc;

	decal = 0;
	getDecal();
}

RBX::Decal* RBX::Surface::getDecal()
{
	if (!decal)
	{
		decal = new RBX::Decal();
		getSurface(decal, surface);
		decal->dfactor = GL_DST_COLOR;
		decal->isDefinedSurfaceDecal = 1;
	}

	return decal;
}