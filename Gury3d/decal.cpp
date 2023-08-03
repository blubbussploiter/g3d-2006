#include "decal.h"
#include "render_shapes.h"

//Reflection::PropertyDescriptor<RBX::Decal, RBX::Content> RBX::Decal::prop_texture("Texture", Reflection::TYPE_Content, &RBX::Decal::getTextureContent, &RBX::Decal::setTextureContent, RBX::Decal::properties);
//Reflection::PropertyDescriptor<RBX::Decal, RBX::NormalId> RBX::Decal::prop_face("Face", Reflection::TYPE_Number, &RBX::Decal::getFace, &RBX::Decal::setFace, RBX::Decal::properties);

void bindDecal(RBX::PVInstance* p, RBX::Decal* d)
{
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glBlendFunc(d->sfactor, d->dfactor);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, d->getGLid());
}

void unbindDecal(RBX::Decal* d)
{
    glBlendFunc(d->sfactor, GL_ZERO);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void RBX::Decal::setTextureContent(Content c)
{
    std::string contentPath;

    RBX::ContentProvider::singleton()->downloadContent(c, contentPath);
    if (contentPath.empty()) return;

    texture = Texture::fromFile(contentPath);

    RBX::ContentProvider::singleton()->cleanupContent(c);
}

void RBX::Decal::fromFile(std::string file, Texture::WrapMode wrap, Texture::InterpolateMode interpolate)
{
	if (texture.isNull())
		texture = Texture::fromFile(file, TextureFormat::AUTO, wrap, interpolate);
}

void RBX::Decal::render(RBX::PVInstance* p)
{

    if (!decalColor)
        decalColor = p->getColor();

    bindDecal(p, this);

    glColor(decalColor);
    p->renderFace(face, 1, !isDefinedSurfaceDecal);

    unbindDecal(this);

}
