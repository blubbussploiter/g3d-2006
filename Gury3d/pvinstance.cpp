#include "render_shapes.h"
#include "rbxmath.h"
#include "lighting.h"
#include "mesh.h"
#include "ray.h"

#include "welds.h"

void rawCylinderAlongX(Color4 color, float radius, float axis)
{
    GLUquadric* v2; // esi
    GLUquadric* v3; // esi
    GLUquadric* v4; // esi
    GLfloat z; // [esp+98h] [ebp-4h]

    glPushMatrix();
    glColor(color.r, color.g, color.b, color.a);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    z = -axis * 0.5;
    glTranslatef(0.0, 0.0, z);
    v2 = gluNewQuadric();
    gluQuadricDrawStyle(v2, 0x186ACu);
    gluCylinder(v2, radius, radius, axis, 12, 1);
    gluDeleteQuadric(v2);
    glTranslatef(0.0, 0.0, axis);
    v3 = gluNewQuadric();
    gluQuadricDrawStyle(v3, 0x186ACu);
    gluDisk(v3, 0.0, radius, 12, 1);
    gluDeleteQuadric(v3);
    glRotatef(180.0, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, axis);
    v4 = gluNewQuadric();
    gluQuadricDrawStyle(v4, 0x186ACu);
    gluDisk(v4, 0.0, radius, 12, 1);
    gluDeleteQuadric(v4);
    glPopMatrix();
}

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

RBX::PVInstance* getPartConnectedToSurface(RBX::PVInstance* p, RBX::FACES surface)
{
    RBX::PVInstance* part;
    RBX::World::Ray* r = 0;

    CoordinateFrame origin;

    switch (surface)
    {
        case RBX::FRONT:
        {
            r = new RBX::World::Ray(origin.translation, origin.getLookVector()*1.1);
            break;
        }
        case RBX::BACK:
        {
            r = new RBX::World::Ray(origin.translation, -origin.getLookVector()*1.1);
            break;
        }
        case RBX::RIGHT:
        {
            r = new RBX::World::Ray(origin.translation, origin.getRightVector()*1.1);
            break;
        }
        case RBX::LEFT:
        {
            r = new RBX::World::Ray(origin.translation, -origin.getRightVector()*1.1);
            break;
        }
        case RBX::TOP:
        {
            r = new RBX::World::Ray(origin.translation, origin.upVector()*1.1);
            break;
        }
        case RBX::BOTTOM:
        {
            r = new RBX::World::Ray(origin.translation, -origin.upVector()*1.1);
            break;
        }
    }

    if (!r) return 0;

    r->addIgnore(p);
    part = r->getPartFromRay();
    
    printf("part = '%s'\n", part->getName().c_str());

    return part;
}

void applySurface(RBX::PVInstance* part0, RBX::PVInstance* part1, RBX::Surface* surface)
{
    if (!part0 || !part1) return;

    switch (surface->getSurfaceType())
    {
        case RBX::Weld:
        {
            printf("ok '%s' '%s'\n", part0->getName().c_str(), part1->getName().c_str());
            RBX::Physics::Weld* weld;
            weld = new RBX::Physics::Weld();
            weld->weld(part0, part1);
            weld->setParent(part0);
            break;
        }
    }
}

void RBX::PVInstance::render(RenderDevice* d)
{
    Vector3 realSz;

    realSz = (size) / 2;

    glEnable(GL_DEPTH_TEST);

    RBX::Lighting::singleton()->begin(d, cframe.translation + realSz, 200);
    d->setObjectToWorldMatrix(getCFrame());

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

            for (int i = 0; i < getChildren()->size(); i++)
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
            Draw::sphere(Sphere(Vector3(0,0,0), getSize().y/2), d, color, Color4::clear());
            break;
        }
        case cylinder:
        {
            float axis, radius;

            axis = getSize().y / 1.0001;
            radius = getSize().z / 2;

            rawCylinderAlongX(color, radius, axis);

            break;
        }
    }

    glDisable(GL_DEPTH_TEST);
    
    RBX::Lighting::singleton()->end(d);

}

void RBX::PVInstance::renderFace(FACES face, bool isAlpha, bool isDrawingDecal)
{
    Vector3 realSz;
    float alpha = 1;

    realSz = (size) / 2;
    realSz.y /= 1.2;

    if (!isAlpha && transparency <= 1)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        alpha = (1 - transparency);
    }

    if(!isDrawingDecal)
        glColor4f(color.r, color.g, color.b, alpha);

    glBegin(GL_QUADS);

    switch (face)
    {
        case FACES::TOP:
        {
            drawFace(top->getTexCoords(face, realSz,isDrawingDecal), Vector3(realSz.x, realSz.y, -realSz.z),
                Vector3(-realSz.x, realSz.y, -realSz.z),
                Vector3(-realSz.x, realSz.y, realSz.z),
                Vector3(realSz.x, realSz.y, realSz.z));
            break;
        }
        case FACES::BOTTOM:
        {
            drawFace(bottom->getTexCoords(face, realSz, isDrawingDecal), Vector3(realSz.x, -realSz.y, realSz.z),
                Vector3(-realSz.x, -realSz.y, realSz.z),
                Vector3(-realSz.x, -realSz.y, -realSz.z),
                Vector3(realSz.x, -realSz.y, -realSz.z));
            break;
        }
        case FACES::FRONT:
        {
            drawFace(front->getTexCoords(face, realSz, isDrawingDecal),Vector3(realSz.x, realSz.y, realSz.z),
                Vector3(-realSz.x, realSz.y, realSz.z),
                Vector3(-realSz.x, -realSz.y, realSz.z),
                Vector3(realSz.x, -realSz.y, realSz.z));
        }
        case FACES::BACK:
        {
            drawFace(back->getTexCoords(face, realSz, isDrawingDecal),Vector3(realSz.x, -realSz.y, -realSz.z),
                Vector3(-realSz.x, -realSz.y, -realSz.z),
                Vector3(-realSz.x, realSz.y, -realSz.z),
                Vector3(realSz.x, realSz.y, -realSz.z));
            break;
        }
        case FACES::LEFT:
        {
            drawFace(left->getTexCoords(face, realSz, isDrawingDecal),Vector3(-realSz.x, realSz.y, realSz.z),
                Vector3(-realSz.x, realSz.y, -realSz.z),
                Vector3(-realSz.x, -realSz.y, -realSz.z),
                Vector3(-realSz.x, -realSz.y, realSz.z));
            break;
        }
        case FACES::RIGHT:
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
        if (transparency <= 1)
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glDisable(GL_BLEND);
        }

        renderSurface(face);
    }
}

void RBX::PVInstance::think()
{
    return;
    RBX::PVInstance* frontp, * backp, * leftp, * rightp, * topp, * bottomp;

    frontp = getPartConnectedToSurface(this, FRONT);
    backp = getPartConnectedToSurface(this, BACK);
    leftp = getPartConnectedToSurface(this, LEFT);
    rightp = getPartConnectedToSurface(this, RIGHT);
    bottomp = getPartConnectedToSurface(this, BOTTOM);
    topp = getPartConnectedToSurface(this, TOP);

    applySurface(this, frontp, front);
    applySurface(this, backp, back);
    applySurface(this, rightp, right);
    applySurface(this, leftp, left);
    applySurface(this, topp, top);
    applySurface(this, bottomp, bottom);
    
}

void RBX::PVInstance::renderSurface(FACES face)
{
    Surface* s;
    s = getSurface(face);

    if (!s)
        return;

    if (s->getSurfaceType() == SURFACES::Smooth)
        return;

    s->getDecal()->decalColor = getColor();
    s->getDecal()->render(this);
}


RBX::Surface* RBX::PVInstance::getSurface(FACES face)
{
    switch (face)
    {
    case FACES::TOP:
    {
        return top;
    }
    case FACES::BOTTOM:
    {
        return bottom;
    }
    case FACES::FRONT:
    {
        return front;
    }
    case FACES::BACK:
    {
        return back;
    }
    case FACES::LEFT:
    {
        return left;
    }
    case FACES::RIGHT:
    {
        return right;
    }
    }
    return 0;
}