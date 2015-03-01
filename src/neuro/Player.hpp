#ifndef NEURO_PLAYER_HPP
#define NEURO_PLAYER_HPP

#include<memory>
#include"neuro/Hand.hpp"
#include"neuro/Army.hpp"
#include"ui/Observable.hpp"
#include"utility/DFStyleReader.hpp"
#include"utility/DFStyleCreator.hpp"
#include"iostream"

namespace neuro {

	/**
		* @brief A player with his Hand, Army and amount of health.
		*/
	class Player : public ui::Observable<Player> {
		public:
			/**
				* @brief Constructs a player with the specified army and amount of health.
				* @param[in] id The id of the player.
				* @param[in] army The tiles in the army the player will own.
				* @param[in] health The initial amount of health the player will have.
				*/
			Player(int id, std::vector<int> army, int health) :
				id(id),
				army(army, id),
				health(health) {}

			/**
				* @brief Move a player.
				*/
			Player(Player && orig) :
				id(orig.id),
				hand(std::move(orig.hand)),
				army(std::move(orig.army)),
				health(orig.health),
				discardedTile(orig.discardedTile) {}

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
			const Army & getArmy() const { return army; }

			/**
				* @brief Returns the health of the player.
				*/
			int getHealth() const { return health; }

			/**
				* @brief Whether the player is alive.
				*/
			bool isAlive() const { return (health > 0); }

			/**
				* @brief Returns the number of tiles in the players hand.
				*/
			int getNumberOfTilesInHand() const { return hand.getNumberOfTiles(); }

			/**
				* @brief Returns the number of tiles left in the players army.
				*/
			int getNumberOfTilesInArmy() const { return army.tilesLeft(); }

			/**
				* @brief Whether the players army is empty.
				*/
			bool armyEmpty() const { return army.isEmpty(); }

			/**
				* @brief Whether the player has discarded the required tile this turn.
				*/
			bool hasDiscardedTile() const { return discardedTile; }

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
			void removeTile(int tile) { hand.removeTile(tile); sigModified(*this); }

			/**
				* @brief Discard the specified tile from your hand.
				* @param[in] tile The tile to discard.
				*/
			void discardTile(int tile) { discardedTile = true; removeTile(tile); }

			/**
				* @brief Do everything that should be done with the player on turn start.
				* @param[in] firstTurns The parameter for drawing less tiles in the first
				* two turns.
				*/
			void startTurn(int & firstTurns);

			/**
				* @brief Make the player represent the one described.
				* @details If this fails, the player might be in an illegal state, so this
				* should be immiediately called again, before doing anything else.
				* @param[in] input The description.
				* @return True if this operation was successfull, false otherwise.
				*/
			bool fillFromDFStyle(utility::DFStyleReader & input);

			/**
				* @brief Encode the player as DFStyle.
				* @param[out] output The encoder to which to write.
				*/
			void encodeAsDFStyle(utility::DFStyleCreator & output) const;

			/**
				* @brief Print the current player state.
                * @todo Make the printed things more meaningful.
				*/
            void printPlayer() const;
		private:
			int id;
			Hand hand;
			Army army;
			int health;
			bool discardedTile;
	};

}

#endif
