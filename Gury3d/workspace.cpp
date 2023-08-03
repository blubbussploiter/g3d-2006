#include "workspace.h"
#include "runservice.h"

#include "part.h"
#include "model.h"

#include "stdout.h"

RBX::Workspace* workspace;

RBX::Extents RBX::Workspace::getGameExtents()
{
    Vector3 high = Vector3::zero();

    for (unsigned int i = 0; i < pvs.size(); i++)
    {
        RBX::PVInstance* pv;
        pv = (RBX::PVInstance*)(pvs.at(i));
        high += pv->getLocalExtents().high;
    }

    return RBX::Extents(-high, high);
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

void RBX::Workspace::renderPVInstances(RenderDevice* d, bool renderOpaque)
{
    for (unsigned int i = 0; i < pvs.size(); i++)
    {
        RBX::Instance* child = pvs.at(i);

        if (child->isRenderable)
        {
            if (child->getClassName() == "PVInstance")
            {
                RBX::PVInstance* inst = static_cast<RBX::PVInstance*>(child);
                if (renderOpaque && inst->getTransparency() <= 0)
                {
                    inst->render(d);
                }
                if(!renderOpaque)
                {
                    if (inst->getTransparency() < 1)
                    {
                        inst->render(d);
                    }
                }
            }
        }

    }
}

void RBX::getPVInstances(RBX::Instances* instances, RBX::Instances* pvs)
{
    for (unsigned int i = 0; i < instances->size(); i++)
    {
        RBX::Instance* child = instances->at(i);

        if (child->getClassName() == "PVInstance" && !contains((*pvs), 
            child))
            pvs->push_back(child);

        getPVInstances(child->getChildren(), pvs);
    }
}

void RBX::Workspace::render(RenderDevice* d)
{
    renderPVInstances(d, 1); /* render opaque objects first */
    renderPVInstances(d, 0); /* then transparent objects */
}

void RBX::Workspace::update()
{
    if (!RBX::RunService::singleton()->isRunning) return;
    updatePVInstances(&pvs);
}

RBX::Workspace* RBX::Workspace::singleton()
{
    if (!workspace) workspace = new RBX::Workspace();
    return workspace;
}

RBX::Instances* RBX::Workspace::getPvs()
{
    return &pvs;
}

void RBX::Workspace::removePV(RBX::PVInstance* pv)
{
    pvs.erase(std::remove(pvs.begin(), pvs.end(), pv));
}

void RBX::Workspace::wakeUpPVS()
{
    getPVInstances(getChildren(), &pvs);
}