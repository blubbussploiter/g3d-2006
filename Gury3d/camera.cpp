
#include <thread>
#include "camera.h"
#include "sounds.h"

RBX::Camera* Csingleton;
RenderDevice* RBX::Camera::rd = 0;

POINT oldMouse;
POINT mouse;

bool isInitialized;

void RBX::Camera::tiltUp(double deg, bool enactedByZoom)
{
    CoordinateFrame frame = CoordinateFrame(camera->getCoordinateFrame().rotation, camera->getCoordinateFrame().translation);
    pan(&frame, 0, toRadians(deg), enactedByZoom, 0.09999988f);
    setFrame(frame);
}

void RBX::Camera::tiltDown(double deg, bool enactedByZoom)
{
    CoordinateFrame frame = CoordinateFrame(camera->getCoordinateFrame().rotation, camera->getCoordinateFrame().translation);
    pan(&frame, 0, toRadians(-deg), enactedByZoom, 0.09999988f);
    setFrame(frame);
}

void RBX::Camera::cam_zoom(bool inout)
{
    isInFirstPerson = (zoom <= 1.5f);

    if (inout)
    {
        Zoom(1.0);
        return;
    }

    Zoom(-1);
}

void RBX::Camera::update(Rendering::G3DApp* app)
{
    UserInput* inpt;
    Vector3 pos;

    inpt = app->userInput;
    focusPosition = cframe.lookVector();

    if (cameraType == Follow)
    {
        if (focusPart)
        {
            focusPosition = focusPart->getPosition();
        }
    }

    if (isUsingRightMouse || isInFirstPerson)
    {
        GetCursorPos(&mouse);
        pan(&cframe, (mouse.x - oldMouse.x) / 100.f, (mouse.y - oldMouse.y) / 100.f, 1);
        SetCursorPos(oldMouse.x, oldMouse.y);
    }

    if (inpt->keyDown(SDL_RIGHT_MOUSE_KEY))
    {
        isUsingRightMouse = true;
    }
    else
        isUsingRightMouse = false;

    GetCursorPos(&oldMouse);

    if(!isInFirstPerson)
        Camera::singleton()->pan(&cframe, 0, 0);
    camera->setCoordinateFrame(cframe);
}

void RBX::Camera::move()
{
    if (!moving())
        return;

    CoordinateFrame _cf;
    _cf = cframe;

    switch (dir())
    {
        case Movement::Forward:
        {
            _cf.translation += _cf.lookVector() * getSpeed();
            break;
        }
        case Movement::Backwards:
        {
            _cf.translation -= _cf.lookVector() * getSpeed();
            break;
        }
        case Movement::Right:
        {
            _cf.translation += _cf.rightVector() * getSpeed();
            break;
        }
        case Movement::Left:
        {
            _cf.translation -= _cf.rightVector() * getSpeed();
            break;
        }
    }

    setFrame(_cf);
    Camera::singleton()->pan(&cframe, 0, 0, 1);
}

void RBX::Camera::cameraInit(GCamera* __camera, RenderDevice* d)
{
    if (Csingleton)
        return;

    rd = d;

    Csingleton = new RBX::Camera();
    Csingleton->setCamera(__camera);
}

/* same as `workspace.CurrentCamera` */
RBX::Camera* RBX::Camera::singleton()
{
    return Csingleton;
}
