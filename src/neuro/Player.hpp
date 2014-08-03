#ifndef NEURO_PLAYER_HPP
#define NEURO_PLAYER_HPP

#include<memory>
#include"neuro/Hand.hpp"
#include"neuro/Army.hpp"
#include"ui/Observable.hpp"

namespace neuro {

	using TileP = std::shared_ptr<Tile>;
	using ArmyP = std::shared_ptr<Army>;

	/**
		* @brief A player with his Hand, Army and amount of health.
		*/
	class Player : public ui::Observable<Player> {
		public:
			/**
				* @brief Constructs a player with the specified army and amount of health.
				* @param[in] id The id of the player.
				* @param[in] army The Army the player will own.
				* @param[in] health The initial amount of health the player will have.
				* Defaults to 20.
				*/
			Player(int id, ArmyP army, int health = 20);

			/**
				* @brief Returns the id of the player.
				*/
			int getID() const { return id; }

			/**
				* @brief Returns the hand of the player.
				*/
			const Hand & getHand() const { return hand; }

			/**
				* @brief Returns the army of the player.
				*/
			const Army & getArmy() const { return *army; }

			/**
				* @brief Returns the health of the player.
				*/
			int getHealth() const { return health; }

			/**
				* @brief Returns the number of tiles in the players hand.
				*/
			int getNumberOfTilesInHand() const { return hand.getNumberOfTiles(); }

			/**
				* @brief Returns the number of tiles left in the players army.
				*/
			int getNumberOfTilesInArmy() const { return army->tilesLeft(); }

			/**
				* @brief Whether the players army is empty.
				*/
			bool armyEmpty() const { return army->isEmpty(); }

			/**
				* @brief Modifies health by the specified amount.
				* @param[in] mod The amount to be added to health.
				*/
			void modifyHealth(int mod) { health += mod; sigModified(*this); }

			/**
				* @brief Draw the specified amount of tiles.
				* @param[in] toDraw the amount of tiles to draw.
				*/
			void drawTiles(int toDraw);

			/**
				* @brief Remove the specified tile from your hand.
				* @param[in] tile The tile to remove.
				*/
			void removeTile(TileP tile) { hand.removeTile(tile); sigModified(*this); }
		private:
			int id;
			Hand hand;
			ArmyP army;
			int health;
	};

}

#endif
