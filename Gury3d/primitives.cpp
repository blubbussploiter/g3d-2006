#include "render_shapes.h"

#define FRONT_CROSS Color3::gray() * 0.7f
#define BACK_CROSS  Color3::white()

void RBX::Primitives::drawLine(Vector2 pos, RenderDevice* d, Color3 color, float width, float height)
{
    Rect2D rect = Rect2D::xywh(pos.x, pos.y, width, height);
    glTranslatef(0.0f, 0.0f, 0.01f);
    Draw::rect2D(rect, d, color);
    glTranslatef(0.0f, 0.0f, -0.01f);
}

void RBX::Primitives::rawCylinderAlongX(Color4 color, float radius, float axis)
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

void RBX::Primitives::drawBall(RenderDevice* d, RBX::PVInstance* base)
{
    Draw::sphere(Sphere(Vector3(0, 0, 0), base->getSize().y / 2), d, base->color, Color4::clear());
}

void RBX::Primitives::drawCylinder(RenderDevice* d, RBX::PVInstance* base)
{
    Vector2 originX, originY;
    float axis, radius, scale;

    axis = base->getSize().y * getAffectedFormFactor(base);
    radius = base->getSize().x / 2;

    scale = radius * 0.1;
    originX = Vector2(0, scale / 2);
    originY = Vector2(scale / 2, 0);

    glPushMatrix();

    glRotatef(90.0, 0.0, 1.0, 0.0);

    glTranslatef(0.0, 0.0, -axis * 0.5);
    glTranslatef(0.0, 0.0, axis);

    drawLine(originX, d, FRONT_CROSS, radius / 1.5f, -scale);
    drawLine(originX, d, FRONT_CROSS, -radius / 1.5f, -scale);
    drawLine(originY, d, FRONT_CROSS, -scale, radius / 1.5f);
    drawLine(originY, d, FRONT_CROSS, -scale, -radius / 1.5f);

    glRotatef(180.0, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, axis);

    drawLine(originX, d, BACK_CROSS, radius / 1.5f, -scale);
    drawLine(originX, d, BACK_CROSS, -radius / 1.5f, -scale);
    drawLine(originY, d, BACK_CROSS, -scale, radius / 1.5f);
    drawLine(originY, d, BACK_CROSS, -scale, -radius / 1.5f);

    glPopMatrix();

    rawCylinderAlongX(base->color, radius, axis);
}
