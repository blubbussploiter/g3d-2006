#include "stdout.h"

#include "model.h"
#include "datamodel.h"

void RBX::RunService::run()
{
    isRunning = true;
    reset();
}

void RBX::RunService::stop()
{
    isRunning = false;
}

void RBX::RunService::reset()
{
    RBX::Scene::singleton()->updatePhysicsObjects();
    //RBX::Workspace::singleton()->wakeUpModels();
}

void RBX::RunService::update()
{
    for (int i = 0; i < 6; i++)
    {
        physics->update();
    }
}

void RBX::RunService::heartbeat()
{
   update();
   //updateSteppers();
}

void RBX::RunService::updateSteppers()
{
    for (unsigned int i = 0; i < steppers->size(); i++)
    {
        steppers->at(i)->onStep();
    }
}

void RBX::RunService::workspaceOnDescendentAdded(RBX::Instance* descendent)
{
    if (descendent->isSteppable)
    {
        steppers->push_back(descendent);
    }
}

RBX::RunService* RBX::RunService::singleton()
{
    return RBX::Datamodel::getDatamodel()->runService;
}