
#include <thread>

#include "workspace.h"

#include "camera.h"
#include "sounds.h"

RBX::Camera* currentCamera;
RenderDevice* RBX::Camera::rd = 0;

POINT oldMouse;
POINT mouse;

bool isInitialized;

static RBX::Sound* switch3 = RBX::Sound::fromFile(GetFileInPath("/content/sounds/SWITCH3.wav"));

//Reflection::PropertyDescriptor<RBX::Camera, CoordinateFrame> RBX::Camera::prop_cframe("CoordinateFrame", Reflection::Types::TYPE_CFrame, &RBX::Camera::getCoordinateFrame, &RBX::Camera::setCoordinateFrame, RBX::Camera::properties);

void RBX::Camera::tiltUp(double deg, bool enactedByZoom)
{
    pan(&cframe, 0, toRadians(deg), 1, 0.41f);
    if(!enactedByZoom) switch3->play();
}

void RBX::Camera::tiltDown(double deg, bool enactedByZoom)
{
    pan(&cframe, 0, toRadians(-deg), 1, 0.41f);
    if (!enactedByZoom) switch3->play();
}

void RBX::Camera::cam_zoom(bool inout)
{
    if(cameraType == Follow) isInFirstPerson = (zoom <= 1.5f);

    if (inout)
    {
        Zoom(1);
        return;
    }

    Zoom(-1);
}

void RBX::Camera::update(Rendering::G3DApp* app)
{
    UserInput* inpt;
    Vector3 pos;

    inpt = app->userInput;
    //focusPosition = cframe.lookVector();

    if (cameraType == Follow)
    {
        if (focusPart)
            focusPosition = focusPart->getPosition();
    }

    if (isUsingRightMouse || isInFirstPerson)
    {
        GetCursorPos(&mouse);
        pan(&cframe, (mouse.x - oldMouse.x) / 100.f, (mouse.y - oldMouse.y) / 100.f);
        SetCursorPos(oldMouse.x, oldMouse.y);
    }

    if (inpt->keyDown(SDL_RIGHT_MOUSE_KEY))
    {
        isUsingRightMouse = true;
    }
    else
        isUsingRightMouse = false;

    GetCursorPos(&oldMouse);

    Camera::singleton()->pan(&cframe, 0, 0, 1, 0.41f);
    camera->setCoordinateFrame(cframe);
}

void RBX::Camera::move()
{
    if (!moving())
        return;

    switch (dir())
    {
        case RBX::Forward:
        {
            cframe.translation += cframe.lookVector() * getSpeed();
            break;
        }
        case RBX::Backwards:
        {
            cframe.translation -= cframe.lookVector() * getSpeed();
            break;
        }
        case RBX::Right:
        {
            cframe.translation += cframe.rightVector() * getSpeed();
            break;
        }
        case RBX::Left:
        {
            cframe.translation -= cframe.rightVector() * getSpeed();
            break;
        }
    }

    setFrame(cframe);
}

void RBX::Camera::zoomExtents()
{
    zoomExtents(RBX::Workspace::singleton()->getGameExtents());
}

void RBX::Camera::zoomExtents(RBX::Extents extents)
{
    Extents worldExtents;
    CoordinateFrame coordFrame, extentsFrame;
    Vector3 low, high, pos, size;

    worldExtents = extents.toWorldSpace(Vector3::zero());
    coordFrame = getCoordinateFrame();

    low = worldExtents.low;
    high = worldExtents.high;

    size = high - low;
    size /= 4;

    pos = (coordFrame.translation + size);
    pos.x = -pos.x;
    pos.z = -pos.z;
    
    extentsFrame = CoordinateFrame(coordFrame.rotation, pos / 2);

    setFrame(extentsFrame);
    lookAt(worldExtents.getCenter());
}

void RBX::Camera::cameraInit(GCamera* __camera, RenderDevice* d)
{
    if (currentCamera)
        return;

    rd = d;

    currentCamera = new RBX::Camera();
    currentCamera->setCamera(__camera);
}

/* same as `workspace.CurrentCamera` */

RBX::Camera* RBX::Camera::singleton()
{
    return currentCamera;
}
