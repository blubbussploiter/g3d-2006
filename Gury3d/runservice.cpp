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
    RBX::Workspace::singleton()->update();
    RBX::Workspace::singleton()->buildModelJoints();
}

void RBX::RunService::update()
{
    if (!isRunning)
        return;

    physics->update();
}

RBX::RunService* RBX::RunService::singleton()
{
    if (!runService) runService = new RunService();
    return runService;
}