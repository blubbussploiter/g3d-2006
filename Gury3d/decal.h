#ifndef DECAL_H
#define DECAL_H

#include <G3DAll.h>

#include "strings.h"
#include "instance.h"

#include "pvenums.h"

namespace RBX
{
	class PVInstance;

	class Decal : public RBX::Instance
	{
	private:
		TextureRef texture;
		NormalId face;
		unsigned int openGLid;
	public:
		GLenum sfactor, dfactor;
		Color4 decalColor;
		float transparency;
		bool isDefinedSurfaceDecal; /* whether its a hardcoded engine decal or user defined decal. (should be distilled by part color) */
		TextureRef getTexture() { return texture; }
		void setTexture(TextureRef r) { texture = r; openGLid = 0; }
		void fromFile(std::string file, Texture::WrapMode wrap = Texture::TILE, Texture::InterpolateMode interpolate=Texture::BILINEAR_NO_MIPMAP);
		unsigned int getGLid() { if (!openGLid) openGLid = texture->openGLID(); return openGLid; }
		NormalId getFace() { return face; }
		void setFace(NormalId _face) { face = _face; }
		void render(PVInstance* p);
		Decal() {
			openGLid = 0; 
			face = NormalId::TOP; 
			sfactor = GL_SRC_ALPHA;
			dfactor = GL_ONE_MINUS_SRC_ALPHA;
			transparency = 0.f;
			setClassName("Decal");
			setName("Decal");
		}
		~Decal() { delete& texture; }
	};
}

#endif