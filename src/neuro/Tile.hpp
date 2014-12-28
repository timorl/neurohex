#ifndef NEURO_TILE_HPP
#define NEURO_TILE_HPP

#include<string>
#include<memory>
#include<set>
#include<list>
#include<vector>
#include<queue>
#include"ui/Observable.hpp"
#include"utility/DFStyleReader.hpp"
#include"utility/DFStyleCreator.hpp"

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
	const std::map< TileType, std::string > stringTileTypeMap = {
		{TileType::INSTANT_ACTION, "INSTANT_ACTION"},
		{TileType::HQ, "HQ"},
		{TileType::MODULE, "MODULE"},
		{TileType::UNIT, "UNIT"},
		{TileType::FOUNDATION, "FOUNDATION"} };

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
	const std::map< TargettingType, std::string > stringTargettingTypeMap = {
		{TargettingType::LOCAL, "LOCAL"},
		{TargettingType::FREE, "FREE"},
		{TargettingType::LINE, "LINE"},
		{TargettingType::BLOB, "BLOB"},
		{TargettingType::ADJECENT, "ADJECENT"},
		{TargettingType::PATH, "PATH"},
		{TargettingType::AWAY, "AWAY"},
		{TargettingType::HAND, "HAND"} };

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
			* @todo Not the best way of identifying tiles, since it's hard to communicate
			* it over a network or something.
			*/
		std::list< TileP > importantTiles;

		/**
			* @brief Make the targetting represent the one described.
			* @details If this fails, the targetting might be in an illegal state, so this
			* should be immiediately called again, before doing anything else.
			* @param[in] input The description.
			* @return True if this operation was successfull, false otherwise.
			* @todo This probably should actually be in a different class representing
			* the game for the client. Really needs work, but... after The Eduardo.
			* @todo This won't really work now, because we have a pointer here. Maybe we
			* should identify tiles in some other way...?
			*/
		bool fillFromDFStyle(utility::DFStyleReader & input);

		/**
			* @brief Encode the targetting as DFStyle.
			* @param[out] output The encoder to which to write.
			*/
		void encodeAsDFStyle(utility::DFStyleCreator & output);
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
		* @brief A structure exactly identifying a ability or another part of a tile.
		*/
	struct AbilityIdentifier {
		/**
			* @brief The tile owning the ability.
			*/
		TileP tile;

		/**
			* @brief The group containing the ability.
			*/
		AbilityGroup	group;

		/**
			* @brief The id of the ability, -1 if not applicable.
			*/
		int id;

		/**
			* @brief Provides an ordering on AbilityIdentifier objects, so they can be in
			* sets.
			*/
		bool operator<( const AbilityIdentifier & other ) const;
	};

	/**
		* @brief A tile to be created in an army and later played.
		* @todo Fill in functions while creating armies.
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

					/**
						* @brief Make the life represent the one described.
						* @details If this fails, the life might be in an illegal state, so this
						* should be immiediately called again, before doing anything else.
						* @param[in] input The description.
						* @return True if this operation was successfull, false otherwise.
						* @todo This probably should actually be in a different class representing
						* the game for the client. Really needs work, but... after The Eduardo.
						*/
					bool fillFromDFStyle(utility::DFStyleReader & input);

					/**
						* @brief Encode the life as DFStyle.
						* @param[out] output The encoder to which to write.
						*/
					void encodeAsDFStyle(utility::DFStyleCreator & output);
				private:
					int health;
					int damage;
			};

			/**
				* @brief A class representing an activated ability of a tile.
				* @todo Medic code should consider more than one medic and actually create
				* the proper defensive ability.
				* @todo Webbing is implemented only partially.
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
						* @param[in] id The id of the ability within the tile.
						* @param[in] group The group of the ability within the tile.
						* chars.
						*/
					Ability( std::string name, std::string description, int direction, Targetting targetting, int strength, std::string abilityActions, int id, AbilityGroup group ) :
						name(name),
						description(description),
						direction(direction),
						targetting(targetting),
						strength(strength),
						abilityActions(abilityActions),
						id(id),
						group(group) {}

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
						* @brief Returns an identifier for this ability.
						*/
					AbilityIdentifier getIdentifier() const;

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
						* @brief Remove modifications from the given tiles.
						* @param[in] targets The tiles to modify.
						*/
					void demodifyTiles( std::list< TileP > targets );

					/**
						* @brief Sets the parent of this tile.
						*/
					void setParent( TileP par ) { parent = par; }

					/**
						* @brief Modify the strength of the ability.
						* @param[in] amount The amount by which to modify.
						*/
					void modifyStrength( int amount ) { strength += amount; }

					/**
						* @brief Make the ability do nothing.
						*/
					void neutralize() { abilityActions = ""; }

					/**
						* @brief Make the ability represent the one described.
						* @details If this fails, the ability might be in an illegal state, so this
						* should be immiediately called again, before doing anything else.
						* @param[in] input The description.
						* @param[in] grp The ability group of the ability.
						* @return True if this operation was successfull, false otherwise.
						* @todo This probably should actually be in a different class representing
						* the game for the client. Really needs work, but... after The Eduardo.
						*/
					bool fillFromDFStyle(utility::DFStyleReader & input, AbilityGroup grp);

					/**
						* @brief Encode the ability as DFStyle.
						* @param[out] output The encoder to which to write.
						*/
					void encodeAsDFStyle(utility::DFStyleCreator & output);
				private:
					std::string name;
					std::string description;
					int direction;
					Targetting	targetting;
					int strength;
					std::string abilityActions;
					std::weak_ptr< Tile > parent;
					int id;
					AbilityGroup	group;
			};

			/**
				* @brief A vector of abilities.
				*/
			using Abilities = std::vector< Ability >;

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
						* @brief Remove the effects of motivation.
						*/
					void demotivate();

					/**
						* @brief Whether the initiative can be modified.
						*/
					bool isModifiable() const { return modifiable; }

					/**
						* Returns the highest initiative, -1 if none.
						*/
					int getHighestInitiative() const;

					/**
						* @brief Make the initiative represent the one described.
						* @details If this fails, the initiative might be in an illegal state, so this
						* should be immiediately called again, before doing anything else.
						* @param[in] input The description.
						* @return True if this operation was successfull, false otherwise.
						* @todo This probably should actually be in a different class representing
						* the game for the client. Really needs work, but... after The Eduardo.
						*/
					bool fillFromDFStyle(utility::DFStyleReader & input);

					/**
						* @brief Encode the initiative as DFStyle.
						* @param[out] output The encoder to which to write.
						*/
					void encodeAsDFStyle(utility::DFStyleCreator & output);
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
					std::set< int > initiative, Abilities onBattleStart,
					Abilities attacks, Abilities modifiers,
					Abilities activeAbilities, Abilities defensiveAbilities) :
				placing(placing),
				onBattleStart(onBattleStart),
				attacks(attacks),
				modifiers(modifiers),
				activeAbilities(activeAbilities),
				defensiveAbilities(defensiveAbilities),
				life(health),
				initiative(initiative),
				name(name),
				type(type),
				webbed(0) {}

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
			void setOwner(int player) { owner = player; controller = player; }

			/**
				* @brief Whether the tile is currently webbed.
				*/
			bool isWebbed() const { return (webbed > 0); }

			/**
				* @brief Whether the tile is alive.
				*/
			bool isAlive() const { return life.isAlive(); }

			/**
				* @brief Whether the tile has any currently activated abilities.
				*/
			bool isActivated() const { return !activatedAbilities.empty(); }

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
			const Abilities & getOnBattleStart() const;

			/**
				* @brief Get a specific ability from the ones to run at the start of a
				* battle.
				* @param[in] id The id of the ability to return.
				*/
			Ability & getOnBattleStart( int id ) { return onBattleStart[id]; }

			/**
				* @brief Get a vector of attacks to launch at every own initiative.
				*/
			const Abilities & getAttacks() const;

			/**
				* @brief Get a specific attack.
				* @param[in] id The id of the modifier to return.
				*/
			Ability & getAttack( int id ) { return attacks[id]; }

			/**
				* @brief Get a vector of modifiers to tiles anywhere.
				*/
			const Abilities & getModifiers() const;

			/**
				* @brief Get a specific modifier.
				* @param[in] id The id of the modifier to return.
				*/
			Ability & getModifier( int id ) { return modifiers[id]; }

			/**
				* @brief Get a vector of abilities the controller may use every turn.
				*/
			const Abilities & getActiveAbilities() const;

			/**
				* @brief Get a specific active ability.
				* @param[in] id The id of the ability to return.
				*/
			Ability & getActiveAbility( int id ) { return activeAbilities[id]; }

			/**
				* @brief Get a vector of abilities that might affect incoming attacks.
				*/
			const Abilities & getDefensiveAbilities() const;

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
				* @brief Returns the ability that has been triggered first.
				*/
			AbilityIdentifier	getActivatedAbility();

			/**
				* @brief Remove all the modifications to which this tile has been subject.
				*/
			void clearModifications();

			/**
				* @brief Remove all the modifications this tile has made.
				*/
			void stopModifying();

			/**
				* @brief Make the tile represent the one described.
				* @details If this fails, the tile might be in an illegal state, so this
				* should be immiediately called again, before doing anything else.
				* @param[in] input The description.
				* @return True if this operation was successfull, false otherwise.
				* @todo This probably should actually be in a different class representing
				* the game for the client. Really needs work, but... after The Eduardo.
				*/
			bool fillFromDFStyle(utility::DFStyleReader & input);

			/**
				* @brief Encode the tile as DFStyle.
				* @param[out] output The encoder to which to write.
				*/
			void encodeAsDFStyle(utility::DFStyleCreator & output);

			/**
				* @brief Returns a pointer to a dummy Tile, only useful when filled later.
				* @todo Again, this probably means there should be two tiles -- server and
				* client.
				*/
			static TileP getDummy();

			/**
				* @brief At which player's turn should terror end. No terror if -1.
				*/
			static int terrorEndOnPlayer;

			/**
				* @brief Whether to start a battle because of tile effects.
				*/
			static bool battle;
		private:
			Ability placing;
			Abilities onBattleStart;
			Abilities attacks;
			Abilities modifiers;
			Abilities activeAbilities;
			Abilities defensiveAbilities;
			Life life;
			Initiative initiative;
			std::string name;
			TileType type;
			int owner;
			int controller;
			std::weak_ptr< Tile > thisP;

			std::map< AbilityIdentifier, std::set< AbilityIdentifier > > modifications;
			std::set< TileP > modifieds;

			std::queue< AbilityIdentifier > activatedAbilities;

			int webbed;

			void addModified( TileP modified );
			void delModified( TileP modified );

			void dealDamage( int strength, int direction = -1, bool ranged = false );
			void destroy( bool noRedirect = false );
			void move();
			void push( TileP source );
			void castle();
			void web( AbilityIdentifier ai );
			void deweb( AbilityIdentifier ai );
			void motivate( AbilityIdentifier ai );
			void demotivate( AbilityIdentifier ai );
			void changeInitiative( AbilityIdentifier ai, int amount, bool fix = false );
			void dechangeInitiative( AbilityIdentifier ai, int amount, bool fix = false );
			void changeMelee( AbilityIdentifier ai, int amount );
			void dechangeMelee( AbilityIdentifier ai, int amount );
			void changeRanged( AbilityIdentifier ai, int amount );
			void dechangeRanged( AbilityIdentifier ai, int amount );

			void addDefensiveAbility( AbilityIdentifier ai, Ability def );
			void delDefensiveAbility( AbilityIdentifier ai );

			static void terrorize( int terrorist );
			static void startBattle();
	};

	using TileP = std::shared_ptr< Tile >;

}

#endif
