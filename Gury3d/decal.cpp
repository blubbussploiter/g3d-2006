#include "decal.h"
#include "render_shapes.h"

void bindDecal(RBX::PVInstance* p, RBX::Decal* d)
{
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    if (!d->decalColor)
        d->decalColor = p->getColor();
    
    glColor(d->decalColor);
    glBlendFunc(GL_SRC_ALPHA, d->dfactor);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, d->getGLid());
}

void unbindDecal()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void RBX::Decal::fromFile(std::string file, int interpolate)
{
	if (texture.isNull())
		texture = Texture::fromFile(file, TextureFormat::AUTO, Texture::TILE, (Texture::InterpolateMode)interpolate);
}

void RBX::Decal::render(RBX::PVInstance* p)
{

    bindDecal(p, this);

    p->renderFace(face, 1, !isDefinedSurfaceDecal);

    unbindDecal();

}
