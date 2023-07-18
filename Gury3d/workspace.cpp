#include "workspace.h"
#include "runservice.h"

#include "part.h"
#include "model.h"

#include "welds.h"

RBX::Workspace* workspace;

void RBX::Workspace::updatePVInstances(RBX::Instances* PVInstances)
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
                RBX::RunService::singleton()->getPhysics()->updateBody(part);
            }
            if (child->getClassName() == "Weld")
            {
                ((RBX::Physics::Weld*)(child))->createWeld();
            }
        }
        updatePVInstances(child->getChildren());
    }
}

void RBX::Workspace::renderPVInstances(RenderDevice* d, RBX::Instances* instances, bool renderOpaque)
{
    for (size_t i = 0; i < instances->size(); i++)
    {
        RBX::Instance* child = instances->at(i);

        if (child->isRenderable)
        {
            if (child->getClassName() == "PVInstance")
            {
                RBX::PVInstance* inst = static_cast<RBX::PVInstance*>(child);
                if (renderOpaque && inst->getTransparency() <= 0)
                {
                    inst->render(d);
                }
                if (inst->getTransparency() > 0 && !renderOpaque) 
                {
                    inst->render(d);
                }
            }
        }

        renderPVInstances(d, child->getChildren(), renderOpaque);
    }
}

void RBX::Workspace::getPVInstances(RBX::Instances* instances, RBX::Instances* pvs)
{
    for (size_t i = 0; i < instances->size(); i++)
    {
        RBX::Instance* child = instances->at(i);

        if (child->getClassName() == "PVInstance")
            pvs->push_back(child);

        getPVInstances(child->getChildren(), pvs);
    }
}

void RBX::Workspace::render(RenderDevice* d)
{
    renderPVInstances(d, getChildren(), 1); /* render opaque objects first */
    renderPVInstances(d, getChildren(), 0); /* then transparent objects */
}

void RBX::Workspace::update()
{
    updatePVInstances(getChildren());
}

RBX::Workspace* RBX::Workspace::singleton()
{
    if (!workspace)
        workspace = new RBX::Workspace();
    return workspace;
}
