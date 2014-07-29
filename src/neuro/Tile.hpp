#ifndef NEURO_TILE_HPP
#define NEURO_TILE_HPP

namespace neuro {

using Player=int;
using Orientation=int;

	enum class TileType {
		INSTANT_ACTION,
		HQ,
		MODULE,
		UNIT,
		FOUNDATION
	};

	class Tile : public ui::Observable<Tile> {
		public:
			Tile(TileType type, int health = 0, int initiative = -1) : type(type), health(health), initiative(initiative), orientation(0) {}

			TileType getType() const { return type; }

			Player getOwner() const { return owner; }

			Player getController() const { return controller; }

			Orientation getOrientation() const { return orientation; }

			int getHealth() const { return health; }

			int getInitiative() const { return (initiative > 0) ? initiative : 0; }

			bool isSolid() const { return (type == TileType::INSTANT_ACTION) || (type == TileType::FOUNDATION); }

			void setOwner(Player player) {
				owner = player;
				controller = player;
			}

			void rotate(Orientation orient) {
				orientation = orient;
			}

			void changeController(Player player) {
				controller = player;
			}

			void modifyHealth(int mod) {
				health += mod;
			}

			void modifyInitiative(int mod) {
				initiative += mod;
			}
		private:
			TileType type;
			Player owner;
			Player controller;
			Orientation orientation; // An integer between 0 and 5.
			int health;
			int initiative;
			/* TODO -- Lists of abilities:
				*  -playing
				*  -on board
				*  -permanent edge x6
				*  -battle edge x6
				*/
	};

}

#endif
