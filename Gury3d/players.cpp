#include "players.h"
#include "workspace.h"

#include "camera.h"
#include "part.h"


#include "mesh.h"

#include "humanoid.h"
#include "jointservice.h"

RBX::Network::Players* players;

void RBX::Network::Player::loadCharacter()
{
	RBX::PartInstance* torso;
	RBX::PartInstance* rArm;
	RBX::PartInstance* lArm;
	RBX::PartInstance* rLeg;
	RBX::PartInstance* lLeg;
	RBX::PartInstance* head;
	RBX::Humanoid* human;
	RBX::Decal* face;
	RBX::Decal* tshirt;

	RBX::Render::Mesh* headMesh;

	if (character)
	{
		character->remove();
		character = 0;
	}

	controller = new PlayerController();
	controller->init(this);

	character = new RBX::ModelInstance();
	human = new RBX::Humanoid();

	torso = new RBX::PartInstance();
	rArm = new RBX::PartInstance();
	lArm = new RBX::PartInstance();
	rLeg = new RBX::PartInstance();
	lLeg = new RBX::PartInstance();
	head = new RBX::PartInstance();
	headMesh = new RBX::Render::Mesh();

	headMesh->fromMeshType(RBX::Render::MeshType::Head);

	torso->setSize(Vector3(2, 2, 1));
	rArm->setSize(Vector3(1, 2, 1));
	lArm->setSize(Vector3(1, 2, 1));
	rLeg->setSize(Vector3(1, 2, 1));
	rLeg->setSize(Vector3(1, 2, 1));
	lLeg->setSize(Vector3(1, 2, 1));

	torso->setCFM(0.1f);
	//torso->setFriction(0.01);
	rArm->setCFM(0.1f);
	//rArm->setFriction(0.01);
	lArm->setCFM(0.1f);
	//lArm->setFriction(0.01);
	rLeg->setCFM(0.1f);
	//rLeg->setFriction(0.01);
	lLeg->setCFM(0.1f);
	//lLeg->setFriction(0.01);
	head->setCFM(0.1f);
	//head->setFriction(0.01);

	torso->setPosition(Vector3(0, 20, 0));

	//rArm->setPosition(Vector3(1.5,  torso->getPosition().y+0.06, 0));
	//lArm->setPosition(Vector3(-1.5, torso->getPosition().y+0.06, 0));
	rArm->setPosition(Vector3(1.5f, torso->getPosition().y, 0));
	lArm->setPosition(Vector3(-1.5f, torso->getPosition().y, 0));
	//lLeg->setPosition(Vector3(0.49,  torso->getPosition().y - 1.6, 0));
	lLeg->setPosition(Vector3(0.49f,  torso->getPosition().y - 2, 0));
	rLeg->setPosition(Vector3(-0.5f, torso->getPosition().y - 2, 0));
	//rLeg->setPosition(Vector3(-0.5, torso->getPosition().y - 1.6, 0));
	//head->setPosition(Vector3(0, torso->getPosition().y + 1.25, 0));
	head->setPosition(Vector3(0, torso->getPosition().y + 1.42f, 0));

	rArm->setBrickColor(24);
	lArm->setBrickColor(24);
	rLeg->setBrickColor(119);
	lLeg->setBrickColor(119);
	head->setBrickColor(24);

	character->primaryPart = torso;

	torso->setName("Torso");
	rArm->setName("Right Arm");
	lArm->setName("Left Arm");
	rLeg->setName("Right Leg");
	lLeg->setName("Left Leg");
	head->setName("Head");

	torso->setParent(character);
	rArm->setParent(character);
	lArm->setParent(character);
	rLeg->setParent(character);
	lLeg->setParent(character);
	lLeg->setParent(character);
	head->setParent(character);
	human->setParent(character);
	human->setHumanoidAttributes();

	head->locked = 1;
	torso->locked = 1;
	rArm->locked = 1;
	lArm->locked = 1;
	rLeg->locked = 1;
	lLeg->locked = 1;

	headMesh->setParent(head);
	headMesh->setMeshScale(Vector3(1.25f, 1.25f, 1.25f));

	face = new RBX::Decal();
	tshirt = new RBX::Decal();

	tshirt->fromFile(GetFileInPath("/content/textures/ROBLOX.png"), Texture::TILE, Texture::NEAREST_MIPMAP);
	tshirt->setFace(BACK);
	tshirt->setParent(torso);
	tshirt->decalColor = Color3::white();
	tshirt->dfactor = GL_ONE_MINUS_SRC_ALPHA;

	face->fromFile(GetFileInPath("/content/textures/face.png"), Texture::TRANSPARENT_BORDER, Texture::NEAREST_MIPMAP);
	face->setFace(BACK);
	face->setParent(head);

	torso->setFace(LEFT, Weld);
	torso->setFace(RIGHT, Weld);

	character->setName(getName());

	Camera::singleton()->focusPart = head;
	Camera::singleton()->cameraType = Follow;

	character->buildJoints();
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
	RBX::addController(controller);
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

RBX::Network::Players* RBX::Network::getPlayers()
{
	if (!players)
		players = new RBX::Network::Players();
	return players;
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
			lbl->title = p->getName();
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
