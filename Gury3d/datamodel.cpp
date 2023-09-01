
#include "appmanager.h"
#include "serializer.h"
#include "stdout.h"

RBX::Datamodel* RBX::Datamodel::getDatamodel()
{
    RBX::Experimental::Application* application = RBX::AppManager::singleton()->getApplication();
    if (application)
    {
        return application->getDatamodel();
    }
    return 0;
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

void RBX::Datamodel::open()
{
    workspace = new Workspace();
    runService = new RunService();
    jointService = new JointService();
    lighting = new Lighting();
    scene = new Scene();
    controllerService = new ControllerService();
    players = new RBX::Network::Players();
    guiRoot = Gui::singleton();
}

void RBX::Datamodel::step()
{
    if (runService->isRunning)
    {
        runService->heartbeat();
        jointService->update();
        RBX::Scene::singleton()->updatePhysicsObjects();
    }

}
