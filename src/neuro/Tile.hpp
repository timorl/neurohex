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
		* @todo Code moving.
		*/
	class Tile : public ui::Observable<Tile> {
		public:
			/**
				* @brief A class controlling the health, damage and destruction of a Tile.
				*/
			class Life {
				public:
					/**
						* @brief Construct a life object.
						* @param[in] health The initial health of the object.
						*/
					Life( int health ) : health(health), damage(0) {}

					/**
						* @brief Whether the tile is still alive.
						*/
					bool isAlive() const { return ( health > damage ); }

					/**
						* @brief Returns the full health of the tile.
						*/
					int getHealth() const { return health; }

					/**
						* @brief Returns the amount of damage dealt to the tile.
						*/
					int getDamage() const { return damage; }

					/**
						* @brief Deals the specified amount of damage to the tile.
						* @param[in] dmg The amount of damage to deal.
						*/
					void dealDamage(int dmg) { damage += dmg; }
				private:
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
						* @param[in] strength The strength of the ability, ignored when unneeded.
						* @param[in] abilityActions What the ability does, coded as a sequence of
						* chars.
						*/
					Ability( std::string name, std::string description, int direction, Targetting targetting, int strength, std::string abilityActions ) :
						name(name),
						description(description),
						direction(direction),
						targetting(targetting),
						strength(strength),
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
						* @brief Returns the direction in which this ability works, -1 if not applicable.
						*/
					int getDirection() const { return direction; }

					/**
						* @brief Returns a sequence of chars describing what the ability does.
						*/
					const std::string & getActionString() const { return abilityActions; }

					/**
						* @brief Get a description of the possible shape of the targetted fields.
						* @return A Targetting object describing the method of targetting used.
						*/
					Targetting getTargettingDescription() const { return targetting; }

					/**
						* @brief Use the ability on the specified tiles.
						* @param[in] targets A std::list of pointers to tiles to be affected by the
						* ability.
						* @returns Whether to place this tile at the last coordinate provided.
						*/
					bool useAbility( std::list< TileP > targets );

					/**
						* @brief Use the ability on the specified tiles.
						* @param[in] targets A std::list of pointers to tiles to be affected by the
						* ability.
						* @returns Whether to place this tile at the last coordinate provided.
						*/
					bool placeTile( std::list< TileP > targets );

					/**
						* @brief Use the ability as a defence against an attack.
						* @param[in] damage A reference to the amount of damage the attack would
						* do barring our intervention.
						* @param[in] ranged Whether the attack was ranged.
						*/
					void useDefensiveAbility( int & damage, bool ranged );

					/**
						* @brief Execute the attack against the specified targets.
						* @param[in] targets A std::list of pointers to tiles to be affected by the
						* attack.
						*/
					void executeAttack( std::list< TileP > targets );

					/**
						* @brief Modify the given tiles.
						* @param[in] targets The tiles to modify.
						*/
					void modifyTiles( std::list< TileP > targets );

					/**
						* @brief Sets the parent of this tile.
						*/
					void setParent( TileP par ) { parent = par; }

					/**
						* @brief Modify the strength of the ability.
						* @param[in] amount The amount by which to modify.
						*/
					void modifyStrength( int amount ) { strength += amount; }
				private:
					std::string name;
					std::string description;
					int direction;
					Targetting	targetting;
					int strength;
					std::string abilityActions;
					std::weak_ptr< Tile > parent;

					void push( TileP tile );
					void substitute( TileP tile );
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
					Initiative( std::set< int > initiative ) : modifiable(true), initiative(initiative) {}

					/**
						* @brief Whether the tile has initiative at the given battle stage.
						*/
					bool hasInitiative( int battleStage ) const { return ( initiative.count( battleStage ) > 0 );  }

					/**
						* @brief Modify all the initiatives.
						* @param[in] change The amount by which to modify the initiative.
						* @param[in] fix Whether to make the initiative unmodifiable. Defaults to
						* false.
						*/
					void changeInitiative( int change, bool fix = false );

					/**
						* @brief Add an additional initiative after the last one the initiative
						* contains.
						*/
					void motivate();

					/**
						* @brief Whether the initiative can be modified.
						*/
					bool isModifiable() const { return modifiable; }

					/**
						* Returns the highest initiative, -1 if none.
						*/
					int getHighestInitiative() const;
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
			Tile( std::string name, TileType type, Ability placing, int health,
					std::set< int > initiative, std::vector< Ability > onBattleStart,
					std::vector< Ability > attacks, std::vector< Ability > modifiers,
					std::vector< Ability > activeAbilities, std::vector< Ability > defensiveAbilities) :
				placingO(placing),
				onBattleStartO(onBattleStart),
				attacksO(attacks),
				modifiersO(modifiers),
				activeAbilitiesO(activeAbilities),
				defensiveAbilitiesO(defensiveAbilities),
				lifeO(health),
				initiativeO(initiative),
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
				placingO(other.placing),
				onBattleStartO(other.onBattleStart),
				attacksO(other.attacks),
				modifiersO(other.modifiers),
				activeAbilitiesO(other.activeAbilities),
				defensiveAbilitiesO(other.defensiveAbilities),
				lifeO(other.life),
				initiativeO(other.initiative),
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
			void setOwner(int player) { owner = player; controller = player; }

			/**
				* @brief Whether the tile is currently webbed.
				*/
			bool isWebbed() const { return webbed; }

			/**
				* @brief Returns the highest initiative this tile has, -1 if none.
				*/
			int getHighestInitiative() const;

			/**
				* @brief Whether this tile acts at the given battle stage.
				*/
			bool hasInitiative( int battleStage ) const { return initiative.hasInitiative(battleStage); }

			/**
				* @brief Set parents for this tile's abilities.
				* @param thisTile A shared_ptr to this tile.
				*/
			void setParents( TileP thisTile );

			/**
				* @brief Get the object responsible for placing this tile.
				*/
			Ability & getPlacing() { return placing; }

			/**
				* @brief Get a vector of abilities to use at the start of every battle.
				*/
			const std::vector< Ability > & getOnBattleStart() const;

			/**
				* @brief Get a specific ability from the ones to run at the start of a
				* battle.
				* @param[in] id The id of the ability to return.
				*/
			Ability & getOnBattleStart( int id ) { return onBattleStart[id]; }

			/**
				* @brief Get a vector of attacks to launch at every own initiative.
				*/
			const std::vector< Ability > & getAttacks() const;

			/**
				* @brief Get a specific attack.
				* @param[in] id The id of the modifier to return.
				*/
			Ability & getAttack( int id ) { return attacks[id]; }

			/**
				* @brief Get a vector of modifiers to tiles anywhere.
				*/
			const std::vector< Ability > & getModifiers() const;

			/**
				* @brief Get a specific modifier.
				* @param[in] id The id of the modifier to return.
				*/
			Ability & getModifier( int id ) { return modifiers[id]; }

			/**
				* @brief Get a vector of abilities the controller may use every turn.
				*/
			const std::vector< Ability > & getActiveAbilities() const;

			/**
				* @brief Get a specific active ability.
				* @param[in] id The id of the ability to return.
				*/
			Ability & getActiveAbility( int id ) { return activeAbilities[id]; }

			/**
				* @brief Get a vector of abilities that might affect incoming attacks.
				*/
			const std::vector< Ability > & getDefensiveAbilities() const;

			/**
				* @brief Get a specific defensive ability.
				* @param[in] id The id of the ability to return.
				*/
			Ability & getDefensiveAbility( int id ) { return defensiveAbilities[id]; }

			/**
				* @brief Get the initiative of the tile.
				*/
			Initiative & getInitiative() { return initiative; }

			/**
				* @brief At which player's turn should terror end. No terror if -1.
				*/
			static int terrorEndOnPlayer;

			/**
				* @brief Whether to start a battle because of tile effects.
				*/
			static bool battle;
		private:
			Ability placingO;
			std::vector< Ability > onBattleStartO;
			std::vector< Ability > attacksO;
			std::vector< Ability > modifiersO;
			std::vector< Ability > activeAbilitiesO;
			std::vector< Ability > defensiveAbilitiesO;
			Life lifeO;
			Initiative initiativeO;
			Ability placing;
			std::vector< Ability > onBattleStart;
			std::vector< Ability > attacks;
			std::vector< Ability > modifiers;
			std::vector< Ability > activeAbilities;
			std::vector< Ability > defensiveAbilities;
			Life life;
			Initiative initiative;
			std::string name;
			TileType type;
			int owner;
			int controller;
			std::weak_ptr< Tile > thisP;

			bool webbed;

			void dealDamage( int strength, int direction = -1, bool ranged = false );
			void destroy( bool noRedirect = false );
			void move();
			void push( TileP source );
			void castle();
			void web();
			void motivate();
			void changeInitiative( int amount, bool fix = false );
			void changeMelee( int amount );
			void changeRanged( int amount );

			void addDefensiveAbility( Ability def );

			static void terrorize( int terrorist );
			static void startBattle();
	};

	using TileP = std::shared_ptr< Tile >;

}

#endif
