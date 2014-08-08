#ifndef NEURO_TILE_HPP
#define NEURO_TILE_HPP

#include<memory>
#include<set>
#include"ui/Observable.hpp"

namespace neuro {

	class Tile;
	using TileP = std::shared_ptr< Tile >;

	/**
		* @brief All the possible types of tiles.
		*/
	enum class TileType {
		INSTANT_ACTION,
		HQ,
		MODULE,
		UNIT,
		FOUNDATION
	};

	/**
		* @brief A class controlling the placement of a tile.
		*/
	class Placing {
		public:
			/**
				* @brief Whether the supplied list of arguments is suitable for this tile.
				* @param[in] A std::list of pointers to tiles which should be affected by the
				* placing.
				* @return Whether the given arguments are acceptable for this tile's
				* placing.
				*/
			bool verifyArguments( std::list< TileP > args );

			/**
				* @brief Place the Tile ad affect the provided targets.
				* @param[in,out] targets The tiles that might be modified by the placed
				* tile.
				* @return Whether the tile should end up on the board after the actions have
				* been performed.
				*/
			bool placeTile( std::list< TileP > targets );

			/**
				* @brief Returns the number of fields the tile should target.
				*/
			int getNumberOfTargets() const { return maxArgs; }

			/**
				* @brief Returns whether the tile targets blobs.
				* @return true if all the targetted fields should be in one blob, false if
				* they can be anywhere.
				*/
			bool blobTargetting() const { return blob; }

			/**
				* @brief The description of actions to take when placing the tile.
				*/
			std::string placeActions;
		private:
			int minArgs;
			int maxArgs;
			bool blob;
			std::set< TileType > acceptableTargets;

			void dealDamage(int amount, TileP target);
			void destroyTile(TileP target);
			void moveTile(TileP target);
			void pushTile(TileP target);
			void castleTiles(TileP first, TileP second);
			void terrorize();
	};

	/**
		* @brief A tile to be created in an army and later played.
		* @todo Lists of abilities:
		*  -playing
		*  -on board
		*  -permanent edge x6
		*  -battle edge x6	
		*/
	class Tile : public ui::Observable<Tile> {
		public:
			/**
				* @brief Construct a tile with a specific type, health and initiative.
				* @param[in] type Type of the tile.
				* @param[in] health Health of the tile. Defaults to 0.
				* @param[in] initiative Initiative of the tile. Defaults to 0.
				*/
			Tile(TileType type, int health = 0) : type(type), health(health) {}

			/**
				* @brief Returns the type of the tile.
				*/
			TileType getType() const { return type; }

			/**
				* @brief Returns the owner of the tile.
				*/
			int getOwner() const { return owner; }

			/**
				* @brief Returns the current controller of the tile.
				*/
			int getController() const { return controller; }

			/**
				* @brief Returns the current health of the tile.
				*/
			int getHealth() const { return health; }

			/**
				* @brief Tells whether the tile is considered a solid object.
				*/
			bool isSolid() const { return !( (type == TileType::INSTANT_ACTION) || (type == TileType::FOUNDATION) ); }

			/**
				* @brief Sets the owner and original controller of the tile.
				* @param[in] player The player to become the owner.
				*/
			void setOwner(int player) {
				owner = player;
				controller = player;
			}

			/**
				* @brief Changes the controller of the tile.
				* @param[in] player The new controller of the tile.
				*/
			void changeController(int player) {
				controller = player;
				sigModified(*this);
			}

			/**
				* @brief At which player's turn should terror end. No terror if -1.
				*/
			static int terrorEndOnPlayer;
		private:
			TileType type;
			int owner;
			int controller;
			Placing placing;
			int health;
			std::set< int > initiative;
	};

	using TileP = std::shared_ptr< Tile >;

}

#endif
