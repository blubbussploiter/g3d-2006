#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "workspace.h"
#include "runservice.h"
#include "jointservice.h"
#include "GuiRoot.h"

namespace RBX
{
	class Datamodel : public RBX::Instance
	{
	public:
		Workspace* workspace;
		RunService* runService;
		JointService* jointService;
		Gui::GuiRoot* guiRoot;
	public:
		static Datamodel* getDatamodel();
		void loadContent(std::string contentId);
		void close();
		void step();
		Datamodel()
		{
			setClassName("Datamodel");
			setName("<<<ROOT>>>");
			workspace = Workspace::singleton();
			runService = RunService::singleton();
			jointService = JointService::singleton();
			guiRoot = Gui::singleton();
			isParentLocked = 1;
		}
	};
}

#endif