#ifndef PLAYERS_H
#define PLAYERS_H

#include "GuiRoot.h"

#include "model.h"
#include "controller.h"

#include "backpack.h"

namespace RBX /* change this to RBX::Network (maybe) */
{
	namespace Network
	{
		class Players;
		class Player;

		Players* getPlayers();

		class Players : public RBX::Instance
		{
		private:
			RBX::Gui::GuiList* playerList;
			std::vector<Player*> players;
		public:

			Player* localPlayer;

			void createLocalPlayer(int userId);
			void createGuiName(Player* plr);
			void setPlayerList(RBX::Gui::GuiList* playerList);
			void destroyPlayer(Player* plr);
			void addPlayer(Player* plr);
			void updatePlayerList();
			/* unlike actual function, no arguments, instead dependant on there being a global Players class */
			static RBX::ModelInstance* findLocalCharacter(); 
			Players()
			{
				setClassName("Players");
				setName("Players");
				isParentLocked = 1;
			}
		};

		class PlayerController : public Movement::Controller
		{
		private:
			Player* plr;
			dMass mass;

			float jmpPower;
			float camYaw;

		public:

			void mv_update();
			void move();

			void init(Player* _plr) { if (!plr) plr = _plr; jmpPower = 30; }
		};

		class Player : public RBX::Instance
		{
		private:
			RBX::Gui::GuiLabel* guiName; /* player name in list */
			PlayerController* controller;
		public:

			int userId;

			RBX::Backpack* backpack;
			RBX::HopperBin* activeBin;

			RBX::ModelInstance* character;

			RBX::Gui::GuiLabel* getGuiName() { return guiName; }
			void setGuiName(RBX::Gui::GuiLabel* lbl) { guiName = lbl; }

			void loadCharacter();
			void disposeActiveBin();

			Player()
			{
				setClassName("Player");
				setName("Player");
				backpack = new RBX::Backpack();
			}
			~Player()
			{
				getPlayers()->destroyPlayer(this);
			}
		};

	}
}

#endif PLAYERS_H