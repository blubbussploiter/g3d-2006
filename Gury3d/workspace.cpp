
#include "part.h"
#include "model.h"

#include "stdout.h"
#include "datamodel.h"

#include "camera.h"

RBX::Extents RBX::Workspace::getGameExtents()
{
    Vector3 high = Vector3::zero();
    std::vector<RBX::Render::Renderable*> pvs = RBX::Scene::singleton()->getArrayOfObjects();

    for (unsigned int i = 0; i < pvs.size(); i++)
    {
        RBX::PVInstance* pv;
        pv = (RBX::PVInstance*)(pvs.at(i));
        if (pv && pv->body)
        {
            high += pv->getLocalExtents().high;
        }
    }

    return RBX::Extents(-high / 2, high / 2);
}

void RBX::Workspace::wakeUpModels()
{
    RBX::Instances* children;
    children = getChildren();

    for (unsigned int i = 0; i < children->size(); i++)
    {
        RBX::Instance* child;
        child = children->at(i);
        if (child->getClassName() == "Model")
        {
            static_cast<RBX::ModelInstance*>(child)->buildJoints();
            static_cast<RBX::ModelInstance*>(child)->createController();
        }
    }
}

void updatePVInstances(RBX::Instances* PVInstances)
{
    RBX::PVInstance* part;
    for (size_t i = 0; i < PVInstances->size(); i++)
    {
        RBX::Instance* child = PVInstances->at(i);
        if (child->isRenderable || child->isAffectedByPhysics)
        {
            if (child->getClassName() == "PVInstance")
            {
                part = (RBX::PVInstance*)child;
                RBX::RunService::singleton()->getPhysics()->createBody(part);
            }
        }
    }
}

void RBX::getPVInstances(RBX::Instances* instances, RBX::Instances* pvs)
{
    for (unsigned int i = 0; i < instances->size(); i++)
    {
        RBX::Instance* child = instances->at(i);

        if (child->getClassName() == "PVInstance")
            pvs->push_back(child);

        getPVInstances(child->getChildren(), pvs);
    }
}

void RBX::Workspace::onDescendentAdded(RBX::Instance* descendent)
{
    RBX::View::singleton()->onWorkspaceDescendentAdded(descendent);
}

RBX::Workspace* RBX::Workspace::singleton()
{
    return RBX::Datamodel::getDatamodel()->workspace;
}

/* hacky method */

RBX::Camera* RBX::Workspace::getCurrentCamera()
{
    return currentCamera;
}

void RBX::Workspace::setCurrentCamera(Camera* camera)
{
    currentCamera = camera;
}