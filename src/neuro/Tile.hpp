#ifndef NEURO_TILE_HPP
#define NEURO_TILE_HPP

namespace neuro {

using Player=int;
using Orientation=int;

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
			Tile(TileType type, int health = 0, int initiative = 0) : type(type), health(health), initiative(initiative), orientation(0) {}

			/**
				* @brief Returns the type of the tile.
				*/
			TileType getType() const { return type; }

			/**
				* @brief Returns the owner of the tile.
				*/
			Player getOwner() const { return owner; }

			/**
				* @brief Returns the current controller of the tile.
				*/
			Player getController() const { return controller; }

			/**
				* @brief Returns the current orientation of the tile.
				*/
			Orientation getOrientation() const { return orientation; }

			/**
				* @brief Returns the current health of the tile.
				*/
			int getHealth() const { return health; }

			/**
				* @brief Returns the current initiative of the tile if positive, else 0.
				*/
			int getInitiative() const { return (initiative > 0) ? initiative : 0; }

			/**
				* @brief Tells whether the tile is considered a solid object.
				*/
			bool isSolid() const { return (type == TileType::INSTANT_ACTION) || (type == TileType::FOUNDATION); }

			/**
				* @brief Sets the owner and original controller of the tile.
				* @param[in] player The player to become the owner.
				*/
			void setOwner(Player player) {
				owner = player;
				controller = player;
			}

			/**
				* @brief Changes the orientation of the tile.
				* @param[in] orient The new orientation of the tile.
				*/
			void rotate(Orientation orient) {
				orientation = orient;
				sigModified(*this);
			}

			/**
				* @brief Changes the controller of the tile.
				* @param[in] player The new controller of the tile.
				*/
			void changeController(Player player) {
				controller = player;
				sigModified(*this);
			}

			/**
				* @brief Modifies the health of the tile.
				* @param[in] mod The amount of health to be added.
				*/
			void modifyHealth(int mod) {
				health += mod;
				sigModified(*this);
			}

			/**
				* @brief Modifies the initiative of the tile.
				* @param[in] mod The amount of initiative to be added.
				*/
			void modifyInitiative(int mod) {
				initiative += mod;
				sigModified(*this);
			}
		private:
			TileType type;
			Player owner;
			Player controller;
			Orientation orientation; // An integer between 0 and 5.
			int health;
			int initiative;
	};

}

#endif
