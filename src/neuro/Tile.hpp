#ifndef NEURO_TILE_HPP
#define NEURO_TILE_HPP

#include<string>
#include<memory>
#include<set>
#include<list>
#include<vector>
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

	const std::map< std::string, TileType > tileTypeStringMap = {
		{"INSTANT_ACTION", TileType::INSTANT_ACTION},
		{"HQ", TileType::HQ},
		{"MODULE", TileType::MODULE},
		{"UNIT", TileType::UNIT},
		{"FOUNDATION", TileType::FOUNDATION} };

	/**
		* @brief The possible types of targetting.
		*/
	enum class TargettingType {
		LOCAL,
		FREE,
		LINE,
		BLOB,
		ADJECENT,
		PATH,
		AWAY,
		HAND
	};

	const std::map< std::string, TargettingType > targettingTypeStringMap = {
		{"LOCAL", TargettingType::LOCAL},
		{"FREE", TargettingType::FREE},
		{"LINE", TargettingType::LINE},
		{"BLOB", TargettingType::BLOB},
		{"ADJECENT", TargettingType::ADJECENT},
		{"PATH", TargettingType::PATH},
		{"AWAY", TargettingType::AWAY},
		{"HAND", TargettingType::HAND} };

	/**
		* @brief a structure fully describing how to target an ability.
		*/
	struct Targetting {
		/**
			* @brief Whether to target specific tiles, targets whole fields if false.
			*/
		bool targetTiles;

		/**
			* @brief The type of the targetting.
			*/
		TargettingType	type;

		/**
			* @brief The number of fields or tiles that will actually be affected by the ability, -1 means all provided.
			*/
		int actualTargets;

		/**
			* @brief The number of fields or tiles that the player should target.
			*/
		int requiredTargets;

		/**
			* @brief The range of the ability. Ignored if irrelevant, -1 means infinite.
			*/
		int range;

		/**
			* @brief A set containing all the types of tiles that can be targetted.
			*/
		std::set< TileType > validTargetTypes;

		/**
			* @brief Whether the ability can target enemy tiles.
			*/
		bool enemy;

		/**
			* @brief Whether the ability can target own tiles.
			*/
		bool own;

		/**
			* @brief A list of tiles important for some targetting types, e.g. AWAY.
			*/
		std::list< TileP > importantTiles;
	};

	/**
		* @brief The groups of abilities a tile can have.
		*/
	enum class AbilityGroup {
		PLACING,
		BATTLE_START,
		ATTACK,
		MODIFIER,
		ACTIVE,
		DEFENSIVE
	};

	/**
		* @brief A tile to be created in an army and later played.
		* @todo Fill in functions while creating armies.
		*/
	class Tile : public ui::Observable<Tile> {
		public:
			/**
				* @brief A class controlling the placement of a Tile.
				*/
			class Placing {
				public:
					/**
						* @brief Construct a placing with the given actions and targetting.
						* @param[in] actions The stuff that should happen when the tile is placed.
						* @param[in] targetting The way the tile might be placed.
						*/
					Placing( std::string actions, Targetting targetting ) : targetting(targetting), placeActions(actions) {}

					/**
						* @brief Whether the supplied list of arguments is suitable for this tile.
						* @param[in] args A std::list of pointers to tiles which should be affected by the
						* placing.
						* @return Whether the given arguments are acceptable for this tile's
						* placing.
						*/
					bool verifyArguments( std::list< TileP > args );

					/**
						* @brief Place the Tile and affect the provided targets.
						* @param[in,out] targets The tiles that might be modified by the placed
						* tile.
						* @return Whether the tile should end up on the board after the actions have
						* been performed.
						*/
					bool placeTile( std::list< TileP > targets );

					/**
						* @brief Get a description of the possible shape of the targetted fields.
						* @return A Targetting object describing the method of targetting used.
						*/
					Targetting getTargettingDescription() const { return targetting; }
				private:
					Targetting	targetting;
					std::string placeActions;

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
						* @brief Construct a life object.
						* @param[in] health The initial health of the object.
						*/
					Life( int health ) : alive(true), health(health), damage(0) {}

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
			};

			/**
				* @brief A class representing an activated ability of a tile.
				*/
			class Ability {
				public:
					/**
						* @brief Construct an ability object.
						* @param[in] name The name of the ability.
						* @param[in] description A short description of the ability.
						* @param[in] direction The direction in which the ability is supposed to
						* work, -1 if not applicable.
						* @param[in] targetting How to target the ability.
						* @param[in] abilityActions What the ability does, coded as a sequence of
						* chars.
						*/
					Ability( std::string name, std::string description, int direction, Targetting targetting, std::string abilityActions ) :
						name(name),
						description(description),
						direction(direction),
						targetting(targetting),
						abilityActions(abilityActions) {}

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
						* Returns the direction in which this ability works, -1 if not applicable.
						*/
					int getDirection() { return direction; }

					/**
						* @brief Get a description of the possible shape of the targetted fields.
						* @return A Targetting object describing the method of targetting used.
						*/
					Targetting getTargettingDescription() const { return targetting; }

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
					int direction;
					Targetting	targetting;
					std::string abilityActions;

					void push( TileP tile );
					void substitute( TileP tile );
			};

			/**
				* @brief A class representing an attack of a tile, usually in a direction.
				*/
			class Attack {
				public:
					/**
						* @brief Construct an attack with the specified properties.
						* @param[in] direction The direction in which the attack is, -1 means no
						* direction.
						* @param[in] targetting The way the attack is targetted.
						* @param[in] melee Whether the attack is considered melee.
						* @param[in] ranged Whether the attack is considered ranged.
						* @param[in] strength The base amount of damage the attack will do.
						* @param[in] attackActions The actions the attack will cause.
						*/
					Attack( int direction, Targetting targetting, bool melee, bool ranged, int strength, std::string attackActions ) :
						direction(direction),
						targetting(targetting),
						melee(melee),
						ranged(ranged),
						strength(strength),
						attackActions(attackActions) {}

					/**
						* Returns the direction in which this attack works, -1 if not applicable.
						*/
					int getDirection() { return direction; }

					/**
						* @brief Get a description of the possible shape of the targetted fields.
						* @return A Targetting object describing the method of targetting used.
						*/
					Targetting getTargettingDescription() const { return targetting; }

					/**
						* @brief Whether the attack is melee.
						*/
					bool isMelee() const { return melee; }

					/**
						* @brief Whether the attack is ranged.
						*/
					bool isRanged() const { return ranged; }

					/**
						* @brief Execute the attack on the given tiles.
						* @param[in] targets A std::list of affected tiles.
						* @param[in] direction The direction in which the attack is executed.
						* Important for some defensive abilities.
						*/
					void executeAttack( std::list< TileP > targets, int direction );
				private:
					int direction;
					Targetting	targetting;
					bool melee;
					bool ranged;
					int strength;
					std::string attackActions;

					void hit( TileP tile, int direction );
			};

			/**
				* @brief A class representing a passive modifier, usually in a direction.
				*/
			class Modifier {
				public:
					/**
						* @brief Construct a modifier.
						* @param[in] direction The direction in which the modifier will work, -1
						* means everywhere.
						* @param[in] targetting How the modifier will choose its target.
						* @param[in] modifyActions What the modifier will do, encoded as a
						* sequence of chars.
						*/
					Modifier( int direction, Targetting targetting, std::string modifyActions ) :
						direction(direction),
						targetting(targetting),
						modifyActions(modifyActions) {}

					/**
						* Returns the direction in which this modifier works, -1 if not applicable.
						*/
					int getDirection() { return direction; }

					/**
						* @brief Get a description of the possible shape of the targetted fields.
						* @return A Targetting object describing the method of targetting used.
						*/
					Targetting getTargettingDescription() const { return targetting; }

					/**
						* @brief Modify the given tile.
						* @param[in] tile The tile to modify.
						*/
					void modifyTile( TileP tile );
				private:
					int direction;
					Targetting	targetting;
					std::string modifyActions;
			};

			/**
				* @brief A class representing the initiative of a tile.
				*/
			class Initiative {
				public:
					/**
						* @brief Construct a initiative object.
						* @param[in] initiative The set of initiatives the object should initially
						* have.
						*/
					Initiative( std::set< int > initiative ) : initiative(initiative) {}

					/**
						* @brief Whether the tile has initiative at the given battle stage.
						*/
					bool hasInitiative( int battleStage ) { return ( initiative.count( battleStage ) > 0 );  }

					/**
						* @brief Add one initiative after every existing one.
						*/
					void duplicateInitiatives();

					/**
						* @brief Modify all the initiatives.
						* @param[in] change The amount by which to modify the initiative.
						* @param[in] fix Whether to make the initiative unmodifiable. Defaults to
						* false.
						*/
					void modifyInitiatives( int change, bool fix = false );
				private:
					bool modifiable;
					std::set< int > initiative;
			};

			/**
				* @brief Construct a full fledged tile without a owner.
				* @param[in] name Name of the tile.
				* @param[in] type Type of the tile.
				* @param[in] placing The description of the placing of the tile.
				* @param[in] health The initial health of the tile.
				* @param[in] initiative A set of initial initiatives of the tile.
				* @param[in] onBattleStart The abilities to be called at the start of each
				* battle.
				* @param[in] attacks Attacks the tile possesses.
				* @param[in] modifiers Modifiers the tile possesses.
				* @param[in] activeAbilities Active abilities the tile possesses.
				* @param[in] defensiveAbilities Defensive abilities the tile possesses.
				*/
			Tile( std::string name, TileType type, Placing placing, int health,
					std::set< int > initiative, std::vector< Ability > onBattleStart,
					std::vector< Attack > attacks, std::vector< Modifier > modifiers,
					std::vector< Ability > activeAbilities, std::vector< Ability > defensiveAbilities) :
				placing(placing),
				onBattleStart(onBattleStart),
				attacks(attacks),
				modifiers(modifiers),
				activeAbilities(activeAbilities),
				defensiveAbilities(defensiveAbilities),
				life(health),
				initiative(initiative),
				name(name),
				type(type) {}

			/**
				* @brief provides a copy of the tile with no owner, controller or observer.
				* @param[in] other The tile to copy.
				*/
			Tile( const Tile & other ) :
				placing(other.placing),
				onBattleStart(other.onBattleStart),
				attacks(other.attacks),
				modifiers(other.modifiers),
				activeAbilities(other.activeAbilities),
				defensiveAbilities(other.defensiveAbilities),
				life(other.life),
				initiative(other.initiative),
				name(other.name),
				type(other.type) {}


			/**
				* @brief Returns the name of the tile.
				*/
			std::string getName() { return name; }

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
				* @brief Attempt to deal damage to the tile.
				* @param[in] strength The amount of damage.
				* @param[in] direction The direction from which the damage is coming. The
				* default is -1, meaning no direction.
				* @param[in] ranged Whether the attack is ranged, defaults to false.
				*/
			void dealDamage( int strength, int direction = -1, bool ranged = false );

			/**
				* @brief Get the object responsible for placing this tile.
				*/
			Placing & getPlacing() { return placing; }

			/**
				* @brief Get a vector of abilities to use at the start of every battle.
				*/
			const std::vector< Ability > & getOnBattleStart() const { return onBattleStart; }

			/**
				* @brief Get a vector of attacks to launch at every own initiative.
				*/
			const std::vector< Attack > & getAttacks() const { return attacks; }

			/**
				* @brief Get a vector of modifiers to tiles anywhere.
				*/
			const std::vector< Modifier > & getModifiers() const { return modifiers; }

			/**
				* @brief Get a vector of abilities the controller may use every turn.
				*/
			const std::vector< Ability > & getActiveAbilities() const { return activeAbilities; }

			/**
				* @brief Get a vector of abilities that might affect incoming attacks.
				*/
			const std::vector< Ability > & getDefensiveAbilities() const { return defensiveAbilities; }

			/**
				* @brief Get the initiative of the tile.
				*/
			Initiative & getInitiative() { return initiative; }

			/**
				* @brief At which player's turn should terror end. No terror if -1.
				*/
			static int terrorEndOnPlayer;
		private:
			Placing placing;
			std::vector< Ability > onBattleStart;
			std::vector< Attack > attacks;
			std::vector< Modifier > modifiers;
			std::vector< Ability > activeAbilities;
			std::vector< Ability > defensiveAbilities;
			Life life;
			Initiative initiative;
			std::string name;
			TileType type;
			int owner;
			int controller;
	};

	using TileP = std::shared_ptr< Tile >;

}

#endif
