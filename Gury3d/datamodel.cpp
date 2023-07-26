#include <thread>

#include "datamodel.h"
#include "serializer.h"
#include "stdout.h"

RBX::Datamodel* datamodel;

RBX::Datamodel* RBX::Datamodel::getDatamodel()
{
    if (!datamodel) datamodel = new RBX::Datamodel();
    return datamodel;
}

void RBX::Datamodel::loadContent(std::string contentId)
{
    RBX::StandardOut::print(MESSAGE_INFO, "DataModel loading %s", contentId.c_str());
    RBX::Serializer::load(contentId);
    runService->reset();
}

void RBX::Datamodel::close()
{
    RBX::StandardOut::print(MESSAGE_INFO, "DataModel::close()");
    RBX::RunService::singleton()->getPhysics()->close();
}

void RBX::Datamodel::step()
{
    for (int i = 0; i < 4; i++)
    {

        runService->update();
        jointService->update();
        workspace->update();

    }

}
