#include "stdout.h"
#include "workspace.h"
#include "jointservice.h"
#include "runservice.h"
#include "model.h"

RBX::RunService* runService;

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
    RBX::Workspace::singleton()->wakeUpPVS();
    RBX::Workspace::singleton()->update();
    RBX::Workspace::singleton()->wakeUpModels();
}

void RBX::RunService::update()
{
    if (!isRunning)
        return;

    physics->update();
}

void RBX::RunService::heartbeat()
{
    update();
    updateSteppers(Workspace::singleton()->getChildren());
}

void RBX::RunService::updateSteppers(RBX::Instances* steppers)
{
    for (unsigned int i = 0; i < steppers->size(); i++)
    {
        RBX::Instance* stepper = steppers->at(i);
        if (stepper && stepper->isSteppable)
            stepper->onStep();
        updateSteppers(stepper->getChildren());
    }
}

RBX::RunService* RBX::RunService::singleton()
{
    if (!runService) runService = new RunService();
    return runService;
}