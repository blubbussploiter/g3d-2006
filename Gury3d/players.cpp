#include "players.h"
#include "workspace.h"

#include "camera.h"
#include "part.h"

#include "mesh.h"

#include "humanoid.h"
#include "jointservice.h"
#include "serializer.h"
#include "datamodel.h"

void RBX::Network::Player::loadCharacter()
{
	RBX::Instances* load;
	RBX::Humanoid* h = 0;

	if (character) return;

	load = RBX::Serializer::loadInstances(GetFileInPath("/content/font/character.rbxm"));

	if (load && load->size() > 0)
	{
		RBX::Instance* possibleCharacter = load->at(0);
		if (possibleCharacter->getClassName() == "Model")
		{
			h = RBX::Humanoid::modelIsCharacter(possibleCharacter);
			if(h)
				character = (RBX::ModelInstance*)possibleCharacter;
		}
	}
	
	if (!character) return;

	RBX::PartInstance* head, *humanoidRootPart;

	head = (RBX::PartInstance*)character->findFirstChild("Head");
	humanoidRootPart = (RBX::PartInstance*)character->findFirstChild("Torso");

	if (!head || !humanoidRootPart) return;

	if (head->getClassName() != "PVInstance" || humanoidRootPart->getClassName() != "PVInstance") return;

	controller = new PlayerController();
	controller->init(this);

	Camera::singleton()->focusPart = head;
	Camera::singleton()->cameraType = Follow;

	character->setName(getName());

	character->buildJoints();
	h->buildJoints();

	character->setParent(RBX::Workspace::singleton());

}

void RBX::Network::Player::disposeActiveBin()
{
	if (!activeBin)
		return;

	activeBin->remove();
	activeBin = 0;
}

void RBX::Network::Player::setAsController()
{
	if (!RBX::RunService::singleton()->isRunning) return;
	//RBX::ControllerService::singleton()->addController(controller);
	RBX::Camera::singleton()->disable(1);
}

void RBX::Network::Players::createLocalPlayer(int userId)
{
	Player* player;

	if (localPlayer) return; /* 'localplayer already exists!' */
	player = new Player();

	if (userId == 0)
		userId++;

	player->userId = userId;
	player->setName("Player");

	localPlayer = player;
	addPlayer(player);
}

void RBX::Network::Players::createGuiName(Player* plr)
{
	RBX::Gui::GuiLabel* lbl;

	lbl = new RBX::Gui::GuiLabel();

	lbl->textColor = Color3::wheelRandom();
	lbl->title = plr->getName();
	lbl->sz = 10;

	plr->setGuiName(lbl);
	playerList->addChild(lbl);
}

void RBX::Network::Players::destroyPlayer(Player* plr)
{
	auto idx2 = std::remove(players.begin(), players.end(), plr);

	playerList->removeChild(plr->getGuiName());
	players.erase(idx2);

	delete& plr;
}

void RBX::Network::Players::setPlayerList(RBX::Gui::GuiList* __playerList)
{
	if (!playerList)
		playerList = __playerList;
}

void RBX::Network::Players::addPlayer(Player* player)
{
	if (!players.size())
		playerList->visible = true;

	players.push_back(player);
	createGuiName(player);
}

void RBX::Network::Players::updatePlayerList()
{
	for (unsigned int i = 0; i < players.size(); i++)
	{
		Player* p = players.at(i);
		RBX::Gui::GuiLabel* lbl = p->getGuiName();

		if (lbl && lbl->title != p->getName())
		{
			lbl->title = p->getName();
		}
	}
}

RBX::ModelInstance* RBX::Network::Players::findLocalCharacter()
{
	RBX::Network::Players* players = RBX::Network::getPlayers();
	RBX::Network::Player* player;

	player = players->localPlayer;

	if (player && (player->character))
		return player->character;

	return 0;
}

RBX::Network::Players* RBX::Network::getPlayers()
{
	return RBX::Datamodel::getDatamodel()->players;
}
