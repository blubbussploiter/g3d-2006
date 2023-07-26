#include "render_shapes.h"
#include "pvenums.h"

TextureRef getSurface(RBX::Decal* d, RBX::SurfaceType s)
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

std::vector<Vector2> RBX::Surface::getTexCoords(RBX::NormalId face, Vector3 sz, bool isDrawingDecal)
{
	float u;
	float v;

	switch (face)
	{
		case TOP:
		case BOTTOM:
		{
			u = -sz.x;
			v = sz.z;
			break;
		}
		case FRONT:
		case BACK:
		{
			u = -sz.x;
			v = sz.y;
			break;
		}
		case LEFT:
		case RIGHT:
		{
			u = sz.z;
			v = sz.y;
			break;
		}
	}

	return
	{

		Vector2(u, v), /* first coord (left top corner?)*/
		Vector2(0, v), /* second coord (left bottom corner?) */
		Vector2(0, 0), /* third coord (right bottom corner?) */
		Vector2(u, 0)  /* last coord (right top corner?) */
	};
}

void RBX::Surface::setSurfaceType(SurfaceType srfc)
{
	surface = srfc;

	decal = 0;
	getDecal();
}

CoordinateFrame RBX::Surface::getSurfaceCenter()
{
	CoordinateFrame center = CoordinateFrame();
	Vector3 size, extentsCenter, positionCenter;
	NormalId face;

	face = getDecal()->getFace();

	size = parent->getSize() / 2;
	extentsCenter = parent->getLocalExtents().getCenter();

	switch (face)
	{
		case NormalId::FRONT:
		{
			positionCenter = extentsCenter + Vector3(0,0, size.z);
			break;
		}
		case NormalId::BACK:
		{
			positionCenter = extentsCenter - Vector3(0, 0, size.z);
			break;
		}
		case NormalId::LEFT:
		{
			positionCenter = extentsCenter + Vector3(-size.x, 0, 0);
			break;
		}
		case NormalId::RIGHT:
		{
			positionCenter = extentsCenter + Vector3(size.x, 0, 0);
			break;
		}
		case NormalId::TOP:
		{
			positionCenter = extentsCenter + Vector3(0, size.y, 0);
			break;
		}
		case NormalId::BOTTOM:
		{
			positionCenter = extentsCenter + Vector3(0, -size.y, 0);
			break;
		}
	}

	CoordinateFrame lookAt;
	lookAt = extentsCenter;
	lookAt.lookAt(positionCenter);
	return CoordinateFrame(lookAt.rotation, positionCenter);
}

RBX::Decal* RBX::Surface::getDecal()
{
	if (!decal)
	{
		decal = new RBX::Decal();
		getSurface(decal, surface);
		decal->sfactor = GL_SRC_ALPHA;
		decal->dfactor = GL_DST_COLOR;
		decal->transparency = 0.0f;
		decal->isDefinedSurfaceDecal = 1;
	}

	return decal;
}