#include "render_shapes.h"
#include "lighting.h"
#include "pvenums.h"
#include "rbxmath.h"
#include "stdout.h"
#include "mesh.h"
#include "ray.h"

/* Booleans */

const Reflection::PropertyDescriptor<RBX::PVInstance, bool> RBX::PVInstance::prop_anchored("Anchored", Reflection::Types::TYPE_Bool, &RBX::PVInstance::getAnchored, &RBX::PVInstance::setAnchored, RBX::PVInstance::properties);
const Reflection::PropertyDescriptor<RBX::PVInstance, bool> RBX::PVInstance::prop_locked("Locked", Reflection::Types::TYPE_Bool, &RBX::PVInstance::getLocked, &RBX::PVInstance::setLocked, RBX::PVInstance::properties);

/* Coordinates */

const Reflection::PropertyDescriptor<RBX::PVInstance, CoordinateFrame> RBX::PVInstance::prop_cframe("CFrame", Reflection::Types::TYPE_CFrame, &RBX::PVInstance::getCFrame, &RBX::PVInstance::setCFrame, RBX::PVInstance::properties);
const Reflection::PropertyDescriptor<RBX::PVInstance, Vector3> RBX::PVInstance::prop_pos("Position", Reflection::Types::TYPE_Vector3, &RBX::PVInstance::getPosition, &RBX::PVInstance::setPosition, RBX::PVInstance::properties);
const Reflection::PropertyDescriptor<RBX::PVInstance, Vector3> RBX::PVInstance::prop_sz("size", Reflection::Types::TYPE_Vector3, &RBX::PVInstance::getSize, &RBX::PVInstance::setSize, RBX::PVInstance::properties);

/* Tokens */

const Reflection::PropertyDescriptor<RBX::PVInstance, int> RBX::PVInstance::prop_shape("shape", Reflection::Types::TYPE_Number, &RBX::PVInstance::getShape, &RBX::PVInstance::setShape, RBX::PVInstance::properties);

const Reflection::PropertyDescriptor<RBX::PVInstance, int> RBX::PVInstance::prop_frontSurface("FrontSurface", Reflection::Types::TYPE_Number, &RBX::PVInstance::getFrontSurface, &RBX::PVInstance::setFrontSurface, RBX::PVInstance::properties);
const Reflection::PropertyDescriptor<RBX::PVInstance, int> RBX::PVInstance::prop_backSurface("BackSurface", Reflection::Types::TYPE_Number, &RBX::PVInstance::getBackSurface, &RBX::PVInstance::setBackSurface, RBX::PVInstance::properties);
const Reflection::PropertyDescriptor<RBX::PVInstance, int> RBX::PVInstance::prop_rightSurface("RightSurface", Reflection::Types::TYPE_Number, &RBX::PVInstance::getRightSurface, &RBX::PVInstance::setRightSurface, RBX::PVInstance::properties);
const Reflection::PropertyDescriptor<RBX::PVInstance, int> RBX::PVInstance::prop_leftSurface("LeftSurface", Reflection::Types::TYPE_Number, &RBX::PVInstance::getLeftSurface, &RBX::PVInstance::setLeftSurface, RBX::PVInstance::properties);
const Reflection::PropertyDescriptor<RBX::PVInstance, int> RBX::PVInstance::prop_topSurface("TopSurface", Reflection::Types::TYPE_Number, &RBX::PVInstance::getTopSurface, &RBX::PVInstance::setTopSurface, RBX::PVInstance::properties);
const Reflection::PropertyDescriptor<RBX::PVInstance, int> RBX::PVInstance::prop_bottomSurface("BottomSurface", Reflection::Types::TYPE_Number, &RBX::PVInstance::getBottomSurface, &RBX::PVInstance::setBottomSurface, RBX::PVInstance::properties);
const Reflection::PropertyDescriptor<RBX::PVInstance, int> RBX::PVInstance::prop_rawFormFactor("rawFormFactor", Reflection::Types::TYPE_Number, &RBX::PVInstance::getFormFactor, &RBX::PVInstance::setFormFactor, RBX::PVInstance::properties);
const Reflection::PropertyDescriptor<RBX::PVInstance, int> RBX::PVInstance::prop_formFactor("formFactor", Reflection::Types::TYPE_Number, &RBX::PVInstance::getFormFactor, &RBX::PVInstance::setFormFactor, RBX::PVInstance::properties);
const Reflection::PropertyDescriptor<RBX::PVInstance, float> RBX::PVInstance::prop_transparency("Transparency", Reflection::Types::TYPE_Number, &RBX::PVInstance::getTransparency, &RBX::PVInstance::setTransparency, RBX::PVInstance::properties);

void drawFace(std::vector<Vector2> surfaceTexCoords, Vector3 v0, Vector3 v1, Vector3 v2, Vector3 v3)
{
    glNormal((v1 - v0).cross(v2 - v0).direction());

    glTexCoord2d(surfaceTexCoords[0].x, surfaceTexCoords[0].y);
    glVertex(v0);
    glTexCoord2d(surfaceTexCoords[1].x, surfaceTexCoords[1].y);
    glVertex(v1);
    glTexCoord2d(surfaceTexCoords[2].x, surfaceTexCoords[2].y);
    glVertex(v2);
    glTexCoord2d(surfaceTexCoords[3].x, surfaceTexCoords[3].y);
    glVertex(v3);
}

void RBX::PVInstance::setFace(NormalId f, SurfaceType s)
{

    switch (f)
    {
    case NormalId::TOP:
    {
        top->setSurfaceType(s);
        top->getDecal()->setFace(f);
        break;
    }
    case NormalId::BOTTOM:
    {
        bottom->setSurfaceType(s);
        bottom->getDecal()->setFace(f);
        break;
    }
    case NormalId::LEFT:
    {
        left->setSurfaceType(s);
        left->getDecal()->setFace(f);
        break;
    case NormalId::RIGHT:
    {
        right->setSurfaceType(s);
        right->getDecal()->setFace(f);
        break;
    }
    case NormalId::FRONT:
    {
        front->setSurfaceType(s);
        front->getDecal()->setFace(f);
        break;
    }
    case NormalId::BACK:
    {
        back->setSurfaceType(s);
        back->getDecal()->setFace(f);
        break;
    }
    }
    }
}

void RBX::PVInstance::render(RenderDevice* d)
{

    glEnable(GL_DEPTH_TEST);
    d->setObjectToWorldMatrix(getCFrame());

    RBX::Lighting::singleton()->begin(d, 50.0f);

    switch (shape)
    {
        case part:
        {    

            renderFace(TOP);
            renderFace(BOTTOM);
            renderFace(FRONT);
            renderFace(BACK);
            renderFace(LEFT);
            renderFace(RIGHT);

            /* render Decals */

            for (unsigned int i = 0; i < getChildren()->size(); i++)
            {
                RBX::Instance* child = getChildren()->at(i);
                if (!child)
                    continue;
                if (child->getClassName() == "Decal")
                    ((RBX::Decal*)(child))->render(this);
            }

            break;
        }
        case ball:
        {     

            RBX::Primitives::drawBall(d, this);
            break;
        }
        case cylinder:
        {
            RBX::Primitives::drawCylinder(d, this);
            break;
        }
    }

    RBX::Lighting::singleton()->end(d);

    render3dSurface(d, TOP);
    render3dSurface(d, BOTTOM);
    render3dSurface(d, FRONT);
    render3dSurface(d, BACK);
    render3dSurface(d, LEFT);
    render3dSurface(d, RIGHT);

    glDisable(GL_DEPTH_TEST);

}

void RBX::PVInstance::renderFace(NormalId face, bool isAlpha, bool isDrawingDecal)
{
    Vector3 realSz;
    float alpha = 1;

    realSz = (size) / 2;
    realSz.y *= getAffectedFormFactor(this);

    if (!isAlpha)
    {
        if (transparency <= 1)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        alpha = (1 - transparency);
    }

    if(!isDrawingDecal)
        glColor(color.r, color.g, color.b, alpha);

    glBegin(GL_QUADS);

    switch (face)
    {
        case NormalId::TOP:
        {
            drawFace(top->getTexCoords(face, realSz,isDrawingDecal), Vector3(realSz.x, realSz.y, -realSz.z),
                Vector3(-realSz.x, realSz.y, -realSz.z),
                Vector3(-realSz.x, realSz.y, realSz.z),
                Vector3(realSz.x, realSz.y, realSz.z));
            break;
        }
        case NormalId::BOTTOM:
        {
            drawFace(bottom->getTexCoords(face, realSz, isDrawingDecal), Vector3(realSz.x, -realSz.y, realSz.z),
                Vector3(-realSz.x, -realSz.y, realSz.z),
                Vector3(-realSz.x, -realSz.y, -realSz.z),
                Vector3(realSz.x, -realSz.y, -realSz.z));
            break;
        }
        case NormalId::FRONT:
        {
            drawFace(front->getTexCoords(face, realSz, isDrawingDecal),Vector3(realSz.x, realSz.y, realSz.z),
                Vector3(-realSz.x, realSz.y, realSz.z),
                Vector3(-realSz.x, -realSz.y, realSz.z),
                Vector3(realSz.x, -realSz.y, realSz.z));
        }
        case NormalId::BACK:
        {
            drawFace(back->getTexCoords(face, realSz, isDrawingDecal),Vector3(realSz.x, -realSz.y, -realSz.z),
                Vector3(-realSz.x, -realSz.y, -realSz.z),
                Vector3(-realSz.x, realSz.y, -realSz.z),
                Vector3(realSz.x, realSz.y, -realSz.z));
            break;
        }
        case NormalId::LEFT:
        {
            drawFace(left->getTexCoords(face, realSz, isDrawingDecal),Vector3(-realSz.x, realSz.y, realSz.z),
                Vector3(-realSz.x, realSz.y, -realSz.z),
                Vector3(-realSz.x, -realSz.y, -realSz.z),
                Vector3(-realSz.x, -realSz.y, realSz.z));
            break;
        }
        case NormalId::RIGHT:
        {
            drawFace(right->getTexCoords(face, realSz, isDrawingDecal),Vector3(realSz.x, realSz.y, -realSz.z),
                Vector3(realSz.x, realSz.y, realSz.z),
                Vector3(realSz.x, -realSz.y, realSz.z),
                Vector3(realSz.x, -realSz.y, -realSz.z));
            break;
        }
    }

    glEnd();

    if (!isAlpha)
    {
        if (transparency < 1)
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glDisable(GL_BLEND);
            renderSurface(face);
        }
    }
}

void RBX::PVInstance::onRemove()
{
    RBX::Workspace::singleton()->removePV(this);
}

void RBX::PVInstance::render3dSurface(RenderDevice* d, NormalId face)
{
    Surface* s;
    SurfaceType type;

    s = getSurface(face);
    type = s->getSurfaceType();

    if (!s) return;

    CoordinateFrame center;
    center = s->getSurfaceCenter();

    d->pushState();
    d->setObjectToWorldMatrix(getCFrame());

    switch (type)
    {
        case SurfaceType::Smooth:
        {
            goto end;
        }
        case SurfaceType::Hinge:
        {
            Draw::cylinder(Cylinder(center.translation - center.lookVector() * 0.5f, center.translation + center.lookVector() * 0.5f, 0.2f), d, Color3::yellow(), Color4::clear());
            break;
        }
        case SurfaceType::SteppingMotor:
        case SurfaceType::Motor:
        {

            Draw::cylinder(Cylinder(center.translation - center.lookVector() * 0.5f, center.translation + center.lookVector() * 0.5f, 0.2f), d, Color3::yellow(), Color4::clear());
            Draw::cylinder(Cylinder(center.translation, center.translation + center.lookVector() * 0.1f, 0.5f), d, Color3::gray(), Color4::clear());

            break;
        }
    }

end:

    d->popState();
}

void RBX::PVInstance::renderSurface(NormalId face)
{
    Surface* s;
    s = getSurface(face);

    if (!s) return;

    switch (s->getSurfaceType())
    {
        case SurfaceType::Hinge:
        case SurfaceType::Motor:
        case SurfaceType::Smooth:
        case SurfaceType::SteppingMotor:
        {
            return;
        }
        default:
        {
            s->getDecal()->render(this);
        }
    }
}


RBX::Surface* RBX::PVInstance::getSurface(NormalId face)
{
    switch (face)
    {
    case NormalId::TOP:
    {
        return top;
    }
    case NormalId::BOTTOM:
    {
        return bottom;
    }
    case NormalId::FRONT:
    {
        return front;
    }
    case NormalId::BACK:
    {
        return back;
    }
    case NormalId::LEFT:
    {
        return left;
    }
    case NormalId::RIGHT:
    {
        return right;
    }
    }
    return 0;
}