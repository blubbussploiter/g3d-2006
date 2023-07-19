#include "players.h"
#include "workspace.h"

#include "camera.h"
#include "part.h"

#include "welds.h"
#include "mesh.h"

#include "humanoid.h"

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

	RBX::Physics::Weld* rightShoulder;
	RBX::Physics::Weld* leftShoulder;
	RBX::Physics::Weld* rightHip;
	RBX::Physics::Weld* leftHip;
	RBX::Physics::Weld* neck;

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

	rightShoulder = new RBX::Physics::Weld();
	leftShoulder = new RBX::Physics::Weld();
	rightHip = new RBX::Physics::Weld();
	leftHip = new RBX::Physics::Weld();
	neck = new RBX::Physics::Weld();

	headMesh->fromFile(GetFileInPath("/content/font/head.mesh"));

	torso->setSize(Vector3(2.0f, 2.2f, 1.0f));
	rArm->setSize(Vector3(1, 2.2f, 1));
	lArm->setSize(Vector3(1, 2.2f, 1));
	rLeg->setSize(Vector3(1, 2.2f, 1));
	rLeg->setSize(Vector3(1, 2.2f, 1));
	lLeg->setSize(Vector3(1, 2.2f, 1));

	torso->setPosition(Vector3(0, 80, 0));

	rArm->setPosition(Vector3(1.5f,  torso->getPosition().y+0.06f, 0));
	lArm->setPosition(Vector3(-1.5f, torso->getPosition().y+0.06f, 0));
	lLeg->setPosition(Vector3(0.49f,  torso->getPosition().y - 1.6f, 0));
	rLeg->setPosition(Vector3(-0.5f, torso->getPosition().y - 1.6f, 0));
	head->setPosition(Vector3(0, torso->getPosition().y + 1.25f, 0));

	rArm->setBrickColor(24);
	lArm->setBrickColor(24);
	rLeg->setBrickColor(119);
	lLeg->setBrickColor(119);
	head->setBrickColor(24);

	character->primaryPart = head;

	torso->setName("Torso");
	rArm->setName("Right Arm");
	lArm->setName("Left Arm");
	rLeg->setName("Right Leg");
	lLeg->setName("Left Leg");
	head->setName("Head");

	rightShoulder->setName("Right Shoulder");
	leftShoulder->setName("Left Shoulder");
	rightHip->setName("Right Hip");
	leftHip->setName("Left Hip");
	neck->setName("Neck");

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
	headMesh->setMeshScale(Vector3(2, 2, 2));

	rightShoulder->weld(torso, rArm);
	leftShoulder->weld(torso, lArm);
	rightHip->weld(torso, rLeg);
	leftHip->weld(torso, lLeg);
	neck->weld(torso, head);

	rightShoulder->setParent(torso);
	leftShoulder->setParent(torso);
	rightHip->setParent(torso);
	leftHip->setParent(torso);
	neck->setParent(head);

	face = new RBX::Decal();
	tshirt = new RBX::Decal();

	tshirt->fromFile(GetFileInPath("/content/textures/ROBLOX.png"), Texture::NEAREST_MIPMAP);
	tshirt->setFace(BACK);
	tshirt->setParent(torso);
	tshirt->decalColor = Color3::white();
	tshirt->dfactor = GL_ONE_MINUS_SRC_ALPHA;

	face->fromFile(GetFileInPath("/content/textures/face.png"), Texture::NEAREST_MIPMAP);
	face->setFace(BACK);
	face->setParent(head);

	torso->setFace(LEFT, Weld);
	torso->setFace(RIGHT, Weld);

	Camera::singleton()->focusPart = head;
	Camera::singleton()->cameraType = Follow;
	character->setParent(RBX::Workspace::singleton());

	Movement::setCurrentController(controller);

}

void RBX::Network::Player::disposeActiveBin()
{
	if (!activeBin)
		return;

	activeBin->remove();
	activeBin = 0;
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
	for (size_t i = 0; i < players.size(); i++)
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
