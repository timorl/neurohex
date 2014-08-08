#ifndef NEURO_TILE_HPP
#define NEURO_TILE_HPP

#include<memory>
#include<set>
#include<list>
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
		* @brief The possible types of targetting.
		*/
	enum class TargettingType {
		FREE,
		BLOB,
		ADJECENT,
		PATH,
		AWAY,
		HAND
	};

	/**
		* @brief A class controlling the placement of a Tile.
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
		* @brief A class controlling the health, damage and destruction of a Tile.
		*/
	class Life {
		public:
			/**
				* @brief Whether the tile is still alive.
				*/
			bool isAlive() const;

			/**
				* @brief Returns the full health of the tile.
				*/
			int getHealth() const;

			/**
				* @brief Returns the amount of damage dealt to the tile.
				*/
			int getDamage() const;

			/**
				* @brief Tries to deal the specified amount of damage to the tile.
				* @param[in] dmg The amount of damage to deal.
				* @param[in] ignoreRedirect Whether to ignore redirections.
				*/
			void dealDamage(int dmg, bool ignoreRedirect = false);

			/**
				* @brief Tries to destroy the tile.
				* @param[in] ignoreRedirect Whether to ignore redirections.
				*/
			void destroy( bool ignoreRedirect = false );

			/**
				* @brief Registers an entity to which to redirect incoming attacks.
				* @param[in] redir A pointer to the tile handling the damage.
				*/
			void registerRedirectior( TileP redir );
		private:
			bool alive;
			int health;
			int damage;
			std::list< TileP > redirectors;
	};

	/**
		* @brief A class representing an activated ability of a tile.
		*/
	class Ability {
		public:
			/**
				* @brief Get the name of the ability.
				* @return A std::string containing the name of the ability.
				*/
			std::string getName() const { return name; }

			/**
				* @brief Get a description of the ability.
				* @return A std::string containing the description of the ability.
				*/
			std::string getDescription() const { return description; }

			/**
				* @brief Get a description of the possible shape of the targetted fields.
				* @return A std::pair of an integer equal to the number of fields that will
				* be targetted and a TargettingType defining the shape the fields can be in.
				*/
			std::pair< int, TargettingType > getTargettingDescription() const { return std::make_pair( targetsNumber, targettingType ); }

			/**
				* @brief Whether the supplied list of arguments is suitable for this ability.
				* @param[in] args A std::list of pointers to tiles which should be affected by the
				* abililty.
				* @return Whether the given arguments are acceptable for this ability.
				*/
			bool verifyArguments( std::list< TileP > args );

			/**
				* @brief Use the ability on the specified tiles.
				* @param[in] targets A std::list of pointers to tiles to be affected by the
				* ability.
				*/
			void useAbility( std::list< TileP > targets );
		private:
			std::string name;
			std::string description;
			TargettingType	targettingType;
			int targetsNumber;
			std::string abilityActions;

			void push( TileP tile );
			void substitute( TileP tile );
	}

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
			Life life;
			std::list< Ability > abilities;
			std::set< int > initiative;
	};

	Tile::terrorEndOnPlayer = -1;

	using TileP = std::shared_ptr< Tile >;

}

#endif
