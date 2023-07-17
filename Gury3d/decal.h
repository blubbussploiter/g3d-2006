#ifndef DECAL_H
#define DECAL_H

#include <G3DAll.h>

#include "strings.h"
#include "instance.h"

namespace RBX
{
	enum FACES
	{
		TOP,
		BOTTOM,
		FRONT,
		BACK,
		LEFT,
		RIGHT
	};

	class PVInstance;

	class Decal : public RBX::Instance
	{
	private:
		TextureRef texture;
		FACES face;
		unsigned int openGLid;
	public:
		GLenum dfactor;
		Color4 decalColor;
		bool isDefinedSurfaceDecal; /* whether its a hardcoded engine decal or user defined decal. (should be distilled by part color) */
		TextureRef getTexture() { return texture; }
		void setTexture(TextureRef r) { texture = r; openGLid = 0; }
		void fromFile(std::string file, int interpolate=Texture::BILINEAR_NO_MIPMAP);
		unsigned int getGLid() { if (!openGLid) openGLid = texture->openGLID(); return openGLid; }
		void setFace(FACES _face) { face = _face; }
		void render(PVInstance* p);
		Decal() {
			openGLid = 0; 
			face = FACES::TOP; 
			dfactor = GL_ONE_MINUS_SRC_ALPHA;
			setClassName("Decal");
			setName("Decal");
		}
		~Decal() { delete& texture; }
	};
}

#endif