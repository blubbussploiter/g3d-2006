#include "runservice.h"

RBX::RunService* runService;

void RBX::RunService::run()
{
    if (!physics)
        physics = new XplicitNgine();

    physics->init();
    isRunning = true;
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