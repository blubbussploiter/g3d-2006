#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "scene.h"
#include "workspace.h"
#include "jointservice.h"
#include "controller.h"
#include "lighting.h"
#include "players.h"

#include "GuiRoot.h"

namespace RBX
{
	class Datamodel : public RBX::Instance
	{
	public:

		Workspace* workspace;
		RunService* runService;
		JointService* jointService;
		ControllerService* controllerService;
		Lighting* lighting;
		Scene* scene;

		Gui::GuiRoot* guiRoot;
		RBX::Network::Players* players;
	public:

		static Datamodel* getDatamodel();
		void loadContent(std::string contentId);

		void open();
		void close();

		void step();
		Datamodel()
		{
			setClassName("Datamodel");
			setName("<<<ROOT>>>");
			isParentLocked = 1;
		}
	};
}

#endif