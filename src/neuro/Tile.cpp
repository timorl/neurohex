#include"neuro/Tile.hpp"

namespace neuro {

	int Tile::terrorEndOnPlayer = -1;
	bool Tile::battle = false;
	std::vector<Tile> Tile::allTiles;

	bool isMelee( Tile::Ability atk ) {
		return ( atk.getActionString()[0] == 'm' );
	}

	bool isRanged( Tile::Ability atk ) {
		return ( atk.getActionString()[0] == 'r' );
	}

	Tile::Ability createSavior( int sacrifice, int recipient ) {
		const std::string name = "Being saved";
		const std::string description = "This tile is being saved from damage. If anything tries to damage it, another will die instead.";
		//TODO: This is a stub.
		Targetting targetting;
		std::string actions;
		int saviorId = Tile::allTiles[recipient].getDefensiveAbilities().size();
		AbilityGroup	group = AbilityGroup::DEFENSIVE;
		return Tile::Ability( name, description, -1, targetting, 0, actions, saviorId, group );
	}

	Tile::Ability createMovement( int recipient ) {
		const std::string name = "Moving";
		const std::string description = "This tile is currently moving.";
		Targetting targetting;
		targetting.targetTiles = false;
		targetting.type = TargettingType::ADJECENT;
		targetting.actualTargets = -1;
		targetting.requiredTargets = 1;
		targetting.range = 1;
		targetting.validTargetTypes.insert( TileType::FOUNDATION );
		targetting.enemy = true;
		targetting.own = true;
		std::string actions("mx");
		int id = Tile::allTiles[recipient].getActiveAbilities().size();
		AbilityGroup	group = AbilityGroup::ACTIVE;
		return Tile::Ability( name, description, -1, targetting, 0, actions, id, group );
	}

	Tile::Ability createPush( int source, int recipient ) {
		const std::string name = "Being pushed";
		const std::string description = "This tile is currently being pushed away.";
		Targetting targetting;
		targetting.targetTiles = false;
		targetting.type = TargettingType::AWAY;
		targetting.actualTargets = -1;
		targetting.requiredTargets = 1;
		targetting.range = 1;
		targetting.validTargetTypes.insert( TileType::FOUNDATION );
		targetting.enemy = true;
		targetting.own = true;
		targetting.importantTiles.push_back( source );
		std::string actions("mx");
		int id = Tile::allTiles[recipient].getActiveAbilities().size();
		AbilityGroup	group = AbilityGroup::ACTIVE;
		return Tile::Ability( name, description, -1, targetting, 0, actions, id, group );
	}

	bool AbilityIdentifier::operator<( const AbilityIdentifier & other ) const {
		if ( tile != other.tile ) {
			return tile < other.tile;
		}
		if ( group != other.group ) {
			return group < other.group;
		}
		if ( id != other.id ) {
			return id < other.id;
		}
		return false;
	}

	const Tile::Abilities & Tile::getOnBattleStart() const {
		return onBattleStart;
	}

	const Tile::Abilities & Tile::getAttacks() const {
		return attacks;
	}

	const Tile::Abilities & Tile::getModifiers() const {
		return modifiers;
	}

	const Tile::Abilities & Tile::getActiveAbilities() const {
		return activeAbilities;
	}

	const Tile::Abilities & Tile::getDefensiveAbilities() const {
		return defensiveAbilities;
	}

	void Tile::Initiative::motivate() {
		if ( modifiable && !initiative.empty() ) {
			initiative.insert( *( initiative.begin() ) - 1 );
		}
	}

	void Tile::Initiative::demotivate() {
		initiative.erase( initiative.begin() );
	}

	void Tile::Initiative::changeInitiative( int amount, bool fix ) {
		if ( modifiable && !initiative.empty() ) {
			std::set< int > newInitiative;
			for ( int i : initiative ) {
				newInitiative.insert( i + amount );
			}
			initiative = newInitiative;
			modifiable = !fix;
		}
	}

	int Tile::Initiative::getHighestInitiative() const {
		if ( initiative.empty() ) {
			return -1;
		}
		return *(initiative.rbegin());
	}

	AbilityIdentifier	Tile::Ability::getIdentifier() const {
		AbilityIdentifier	ai;
		ai.tile = parent;
		ai.id = id;
		ai.group = group;
		return ai;
	}

	bool Tile::Ability::placeTile( std::list< int > targets ) {
		bool stayOnBoard = false;
		for ( auto action = abilityActions.begin(); action != abilityActions.end(); action++ ) {
			switch ( *action ) {
				case 'P':
					stayOnBoard	= true;
					break;
				case 'd':
					for ( int trgt : targets ) {
						allTiles[trgt].dealDamage( 1 );
					}
					break;
				case 'm':
					for ( int trgt : targets ) {
						allTiles[trgt].move();
					}
					break;
				case 'p':
					for ( int trgt : targets ) {
						allTiles[trgt].push(parent);
					}
					break;
				case 'b':
					startBattle();
					break;
			}
		}
		return stayOnBoard;
	}

	bool Tile::Ability::useAbility( std::list< int > targets ) {
		bool placeTile = false;
		for ( auto action = abilityActions.begin(); action != abilityActions.end(); action++ ) {
			switch ( *action ) {
				case 'm':
					placeTile = true;
					break;
				case 'x':
					neutralize();
					break;
			}
		}
		return placeTile;
	}

	void Tile::Ability::useDefensiveAbility( int & damage, bool ranged ) {
		for ( auto action = abilityActions.begin(); action != abilityActions.end(); action++ ) {
			switch ( *action ) {
				case 'r':
					int amount = strength;
					if ( ranged ) {
						damage -= amount;
					}
					break;
			}
		}
	}

	void Tile::Ability::executeAttack( std::list< int > targets ) {
		for ( auto action = abilityActions.begin(); action != abilityActions.end(); action++ ) {
			switch ( *action ) {
				case 'r':
					for ( int trgt : targets ) {
						allTiles[trgt].dealDamage( strength, direction, true );
					}
					break;
				case 'g': //For Gauss gun -- ranged damage, but not ranged attack.
					for ( int trgt : targets ) {
						allTiles[trgt].dealDamage( strength, direction, true );
					}
					break;
				case 'd':
					for ( int trgt : targets ) {
						allTiles[trgt].dealDamage( strength, direction, false );
					}
					break;
				case 'm':
					for ( int trgt : targets ) {
						allTiles[trgt].dealDamage( strength, direction, false );
					}
					break;
				case 'a':
					allTiles[parent].destroy( true );
					break;
			}
		}
	}

	void Tile::Ability::modifyTiles( std::list< int > targets ) {
		AbilityIdentifier ai = getIdentifier();
		for ( auto action = abilityActions.begin(); action != abilityActions.end(); action++ ) {
			switch ( *action ) {
				case 'w':
					for ( int trgt : targets ) {
						allTiles[ai.tile].addModified( trgt );
						allTiles[trgt].web( ai );
					}
					break;
				case 'i':
					for ( int trgt : targets ) {
						allTiles[ai.tile].addModified( trgt );
						allTiles[trgt].changeInitiative( ai, strength );
					}
					break;
				case 'm':
					for ( int trgt : targets ) {
						allTiles[ai.tile].addModified( trgt );
						allTiles[trgt].changeMelee( ai, strength );
					}
					break;
				case 'r':
					for ( int trgt : targets ) {
						allTiles[ai.tile].addModified( trgt );
						allTiles[trgt].changeRanged( ai, strength );
					}
					break;
				case 's':
						for ( int trgt : targets ) {
							allTiles[ai.tile].addModified( trgt );
							allTiles[trgt].addDefensiveAbility( ai, createSavior( parent, trgt ) );
						}
						break;
				case 'M':
					for ( int trgt : targets ) {
						allTiles[ai.tile].addModified( trgt );
						allTiles[trgt].motivate( ai );
					}
					break;
			}
		}
	}

	void Tile::Ability::demodifyTiles( std::list< int > targets ) {
		AbilityIdentifier ai = getIdentifier();
		for ( auto action = abilityActions.begin(); action != abilityActions.end(); action++ ) {
			switch ( *action ) {
				case 'w':
					for ( int trgt : targets ) {
						allTiles[ai.tile].delModified( trgt );
						allTiles[trgt].deweb( ai );
					}
					break;
				case 'i':
					for ( int trgt : targets ) {
						allTiles[ai.tile].delModified( trgt );
						allTiles[trgt].dechangeInitiative( ai, strength );
					}
					break;
				case 'm':
					for ( int trgt : targets ) {
						allTiles[ai.tile].delModified( trgt );
						allTiles[trgt].dechangeMelee( ai, strength );
					}
					break;
				case 'r':
					for ( int trgt : targets ) {
						allTiles[ai.tile].delModified( trgt );
						allTiles[trgt].dechangeRanged( ai, strength );
					}
					break;
				case 's':
					{
						for ( int trgt : targets ) {
							allTiles[ai.tile].delModified( trgt );
							allTiles[trgt].delDefensiveAbility( ai );
						}
						break;
					}
				case 'M':
					for ( int trgt : targets ) {
						allTiles[ai.tile].delModified( trgt );
						allTiles[trgt].demotivate( ai );
					}
					break;
			}
		}
	}

	void Tile::setId(int thisTile) {
		globalID = thisTile;
		placing.setParent( thisTile );
		for ( auto atk : attacks ) {
			atk.setParent( thisTile );
		}
		for ( auto mdfr : modifiers ) {
			mdfr.setParent( thisTile );
		}
	}

	int Tile::getHighestInitiative() const {
		if ( webbed ) {
			return -1;
		}
		return initiative.getHighestInitiative();
	}

	AbilityIdentifier	Tile::getActivatedAbility() {
		AbilityIdentifier	result = activatedAbilities.front();
		activatedAbilities.pop();
		return result;
	}

	void Tile::clearModifications() {
		std::list< int > toDemod;
		toDemod.push_back( globalID );
		for ( auto modif : modifications ) {
			allTiles[modif.first.tile].getModifier( modif.first.id ).demodifyTiles( toDemod );
		}
	}

	void Tile::stopModifying() {
		std::list< int > toDemod( modifieds.begin(), modifieds.end() );
		for ( Ability & mod : modifiers ) {
			mod.demodifyTiles( toDemod );
		}
	}

	bool Tile::fillFromDFStyle(utility::DFStyleReader & input) {
		onBattleStart.clear();
		attacks.clear();
		modifiers.clear();
		activeAbilities.clear();
		defensiveAbilities.clear();
		while ( input.hasNextToken() ) {
			std::vector< std::string > info = input.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				return false;
			}
			std::string tType = info[0];
			if ( tType == "TILEEND" ) {
				return true;
			} else if ( tType == "ABILITYBEGIN" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				std::string & abilityType = info[1];
				if ( abilityType == "placing" ) {
					if ( !placing.fillFromDFStyle(input, AbilityGroup::PLACING) ) {
						return false;
					}
				} else if ( abilityType == "battleStart" ) {
					Ability toAdd = placing;
					if ( !toAdd.fillFromDFStyle(input, AbilityGroup::BATTLE_START) ) {
						return false;
					}
					onBattleStart.push_back(toAdd);
				} else if ( abilityType == "attack" ) {
					Ability toAdd = placing;
					if ( !toAdd.fillFromDFStyle(input, AbilityGroup::ATTACK) ) {
						return false;
					}
					attacks.push_back(toAdd);
				} else if ( abilityType == "modifier" ) {
					Ability toAdd = placing;
					if ( !toAdd.fillFromDFStyle(input, AbilityGroup::MODIFIER) ) {
						return false;
					}
					modifiers.push_back(toAdd);
				} else if ( abilityType == "active" ) {
					Ability toAdd = placing;
					if ( !toAdd.fillFromDFStyle(input, AbilityGroup::ACTIVE) ) {
						return false;
					}
					activeAbilities.push_back(toAdd);
				} else if ( abilityType == "defensive" ) {
					Ability toAdd = placing;
					if ( !toAdd.fillFromDFStyle(input, AbilityGroup::DEFENSIVE) ) {
						return false;
					}
					defensiveAbilities.push_back(toAdd);
				} else {
					return false;
				}
			} else if ( tType == "LIFEBEGIN" ) {
				if ( !life.fillFromDFStyle(input) ) {
					return false;
				}
			} else if ( tType == "INITIATIVEBEGIN" ) {
				if ( !initiative.fillFromDFStyle(input) ) {
					return false;
				}
			} else if ( tType == "NAME" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				name = info[1];
			} else if ( tType == "TYPE" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				type = tileTypeStringMap.at(info[1]);
			} else if ( tType == "OWNER" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				owner = std::stoi(info[1]);
			} else if ( tType == "CONTROLLER" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				controller = std::stoi(info[1]);
			} else if ( tType == "WEBBED" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				webbed = std::stoi(info[1]);
			} else if ( tType == "ID" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				globalID = std::stoi(info[1]);
			} else {
				return false;
			}
		}
		return false;
	}

	void Tile::encodeAsDFStyle(utility::DFStyleCreator & output) const {
		output.startToken("ABILITYBEGIN");
		output.addToToken(std::string("placing"));
		output.endToken();
		placing.encodeAsDFStyle(output);
		for ( const Ability & ablt : onBattleStart ) {
			output.startToken("ABILITYBEGIN");
			output.addToToken(std::string("battleStart"));
			output.endToken();
			ablt.encodeAsDFStyle(output);
		}
		for ( const Ability & ablt : attacks ) {
			output.startToken("ABILITYBEGIN");
			output.addToToken(std::string("attack"));
			output.endToken();
			ablt.encodeAsDFStyle(output);
		}
		for ( const Ability & ablt : modifiers ) {
			output.startToken("ABILITYBEGIN");
			output.addToToken(std::string("modifier"));
			output.endToken();
			ablt.encodeAsDFStyle(output);
		}
		for ( const Ability & ablt : activeAbilities ) {
			output.startToken("ABILITYBEGIN");
			output.addToToken(std::string("active"));
			output.endToken();
			ablt.encodeAsDFStyle(output);
		}
		for ( const Ability & ablt : defensiveAbilities ) {
			output.startToken("ABILITYBEGIN");
			output.addToToken(std::string("defensive"));
			output.endToken();
			ablt.encodeAsDFStyle(output);
		}
		output.startToken("LIFEBEGIN");
		output.endToken();
		life.encodeAsDFStyle(output);
		output.startToken("INITIATIVEBEGIN");
		output.endToken();
		initiative.encodeAsDFStyle(output);
		output.startToken("NAME");
		output.addToToken(name);
		output.endToken();
		output.startToken("TYPE");
		output.addToToken(stringTileTypeMap.at(type));
		output.endToken();
		output.startToken("OWNER");
		output.addToToken(owner);
		output.endToken();
		output.startToken("CONTROLLER");
		output.addToToken(controller);
		output.endToken();
		output.startToken("WEBBED");
		output.addToToken(webbed);
		output.endToken();
		output.startToken("ID");
		output.addToToken(globalID);
		output.endToken();
		output.startToken("TILEEND");
		output.endToken();
	}

	bool Tile::Ability::fillFromDFStyle(utility::DFStyleReader & input, AbilityGroup grp) {
		group = grp;
		while ( input.hasNextToken() ) {
			std::vector< std::string > info = input.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				return false;
			}
			std::string tType = info[0];
			if ( tType == "ABILITYEND" ) {
				return true;
			} else if ( tType == "NAME" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				name = info[1];
			} else if ( tType == "DESCRIPTION" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				description = info[1];
			} else if ( tType == "DIRECTION" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				direction = std::stoi(info[1]);
			} else if ( tType == "TARGETTINGBEGIN" ) {
				if ( !targetting.fillFromDFStyle(input) ) {
					return false;
				}
			} else if ( tType == "STRENGTH" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				strength = std::stoi(info[1]);
			} else if ( tType == "ACTIONS" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				abilityActions = info[1];
			} else if ( tType == "PARENT" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				parent = std::stoi(info[1]);
			} else if ( tType == "ID" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				id = std::stoi(info[1]);
			} else {
				return false;
			}
		}
		return false;
	}

	void Tile::Ability::encodeAsDFStyle(utility::DFStyleCreator & output) const {
		output.startToken("NAME");
		output.addToToken(name);
		output.endToken();
		output.startToken("DESCRIPTION");
		output.addToToken(description);
		output.endToken();
		output.startToken("DIRECTION");
		output.addToToken(direction);
		output.endToken();
		output.startToken("TARGETTINGBEGIN");
		output.endToken();
		targetting.encodeAsDFStyle(output);
		output.startToken("STRENGTH");
		output.addToToken(strength);
		output.endToken();
		output.startToken("ACTIONS");
		output.addToToken(abilityActions);
		output.endToken();
		output.startToken("PARENT");
		output.addToToken(parent);
		output.endToken();
		output.startToken("ID");
		output.addToToken(id);
		output.endToken();
		output.startToken("ABILITYEND");
		output.endToken();
	}

	bool Tile::Life::fillFromDFStyle(utility::DFStyleReader & input) {
		while ( input.hasNextToken() ) {
			std::vector< std::string > info = input.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				return false;
			}
			std::string tType = info[0];
			if ( tType == "LIFEEND" ) {
				return true;
			} else if ( tType == "HEALTH" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				health = std::stoi(info[1]);
			} else if ( tType == "DAMAGE" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				damage = std::stoi(info[1]);
			} else {
				return false;
			}
		}
		return false;
	}

	void Tile::Life::encodeAsDFStyle(utility::DFStyleCreator & output) const {
		output.startToken("HEALTH");
		output.addToToken(health);
		output.endToken();
		output.startToken("DAMAGE");
		output.addToToken(damage);
		output.endToken();
		output.startToken("LIFEEND");
		output.endToken();
	}

	bool Tile::Initiative::fillFromDFStyle(utility::DFStyleReader & input) {
		initiative.clear();
		while ( input.hasNextToken() ) {
			std::vector< std::string > info = input.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				return false;
			}
			std::string tType = info[0];
			if ( tType == "INITIATIVEEND" ) {
				return true;
			} else if ( tType == "MODIFIABLE" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				if ( info[1] == "true" ) {
					modifiable = true;
				} else if ( info[1] == "false" ) {
					modifiable = false;
				} else {
					return false;
				}
			} else if ( tType == "INITIATIVE" ) {
				for ( int i = 1; i < static_cast<int>( info.size() ); i++ ) {
					initiative.insert(std::stoi(info[i]));
				}
			} else {
				return false;
			}
		}
		return false;
	}

	void Tile::Initiative::encodeAsDFStyle(utility::DFStyleCreator & output) const {
		output.startToken("MODIFIABLE");
		output.addToToken(modifiable);
		output.endToken();
		output.startToken("INITIATIVE");
		for ( const int & initi : initiative ) {
			output.addToToken(initi);
		}
		output.endToken();
		output.startToken("INITIATIVEEND");
		output.endToken();
	}

	bool Targetting::fillFromDFStyle(utility::DFStyleReader & input) {
		validTargetTypes.clear();
		importantTiles.clear();
		while ( input.hasNextToken() ) {
			std::vector< std::string > info = input.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				return false;
			}
			std::string tType = info[0];
			if ( tType == "TARGETTINGEND" ) {
				return true;
			} else if ( tType == "TARGET_TILES" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				if ( info[1] == "true" ) {
					targetTiles = true;
				} else if ( info[1] == "false" ) {
					targetTiles = false;
				} else {
					return false;
				}
			} else if ( tType == "TYPE" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				type = targettingTypeStringMap.at(info[1]);
			} else if ( tType == "ACTUAL_TARGETS" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				actualTargets = std::stoi(info[1]);
			} else if ( tType == "REQUIRED_TARGETS" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				requiredTargets = std::stoi(info[1]);
			} else if ( tType == "RANGE" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				range = std::stoi(info[1]);
			} else if ( tType == "TARGET_TYPES" ) {
				for ( int i = 1; i < static_cast<int>( info.size() ); i++ ) {
					validTargetTypes.insert(tileTypeStringMap.at(info[i]));
				}
			} else if ( tType == "ENEMY" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				if ( info[1] == "true" ) {
					enemy = true;
				} else if ( info[1] == "false" ) {
					enemy = false;
				} else {
					return false;
				}
			} else if ( tType == "OWN" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				if ( info[1] == "true" ) {
					own = true;
				} else if ( info[1] == "false" ) {
					own = false;
				} else {
					return false;
				}
			} else if ( tType == "IMPORTANT_TILES" ) {
				for ( int i = 1; i < static_cast<int>( info.size() ); i++ ) {
					importantTiles.push_back(std::stoi(info[i]));
				}
			} else {
				return false;
			}
		}
		return false;
	}

	void Targetting::encodeAsDFStyle(utility::DFStyleCreator & output) const {
		output.startToken("TARGET_TILES");
		output.addToToken(targetTiles);
		output.endToken();
		output.startToken("TYPE");
		output.addToToken(stringTargettingTypeMap.at(type));
		output.endToken();
		output.startToken("ACTUAL_TARGETS");
		output.addToToken(actualTargets);
		output.endToken();
		output.startToken("REQUIRED_TARGETS");
		output.addToToken(requiredTargets);
		output.endToken();
		output.startToken("RANGE");
		output.addToToken(range);
		output.endToken();
		output.startToken("TARGET_TYPES");
		for ( const TileType & tt : validTargetTypes ) {
			output.addToToken(stringTileTypeMap.at(tt));
		}
		output.endToken();
		output.startToken("ENEMY");
		output.addToToken(enemy);
		output.endToken();
		output.startToken("OWN");
		output.addToToken(own);
		output.endToken();
		output.startToken("IMPORTANT_TILES");
		for ( const int & impTil : importantTiles ) {
			output.addToToken(impTil);
		}
		output.endToken();
		output.startToken("TARGETTINGEND");
		output.endToken();
	}

	bool AbilityIdentifier::fillFromDFStyle(utility::DFStyleReader & input) {
		while ( input.hasNextToken() ) {
			std::vector< std::string > info = input.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				return false;
			}
			std::string tType = info[0];
			if ( tType == "ABILITYIDENTIFIEREND" ) {
				return true;
			} else if ( tType == "TILE" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				tile = std::stoi(info[1]);
			} else if ( tType == "GROUP" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				group = abilityGroupStringMap.at(info[1]);
			} else if ( tType == "ID" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				id = std::stoi(info[1]);
			} else {
				return false;
			}
		}
		return false;
	}

	void AbilityIdentifier::encodeAsDFStyle(utility::DFStyleCreator & output) const {
		output.startToken("TILE");
		output.addToToken(tile);
		output.endToken();
		output.startToken("GROUP");
		output.addToToken(stringAbilityGroupMap.at(group));
		output.endToken();
		output.startToken("ID");
		output.addToToken(id);
		output.endToken();
		output.startToken("ABILITYIDENTIFIEREND");
		output.endToken();
	}

	void Tile::addModified( int modified ) {
		modifieds.insert( modified );
	}

	void Tile::delModified( int modified ) {
		modifieds.erase( modified );
	}

	void Tile::dealDamage( int strength, int direction, bool ranged ) {
		for ( Ability & def : defensiveAbilities ) {
			if ( def.getDirection() == ( ( direction + 3 ) % 6 ) || def.getDirection() == -1 ) {
				def.useDefensiveAbility( strength, ranged );
			}
		}
		if ( strength > 0 ) {
			life.dealDamage( strength );
		}
	}

	void Tile::destroy( bool noRedirect ) {
		const int infiniteDamage = 100;
		if ( noRedirect ) {
			life.dealDamage( infiniteDamage ); // Pretty much infinity.
		} else {
			dealDamage( infiniteDamage ); // Let the dealing function handle defenses.
		}
	}

	void Tile::move() {
		activeAbilities.push_back( createMovement(globalID) );
		activatedAbilities.push( activeAbilities.back().getIdentifier() );
	}

	void Tile::push( int source ) {
		activeAbilities.push_back( createPush( source, globalID) );
		activatedAbilities.push( activeAbilities.back().getIdentifier() );
	}

	void Tile::startBattle() {
		battle = true;
	}

	void Tile::web( AbilityIdentifier ai ) {
		AbilityIdentifier	target;
		target.tile = globalID;
		target.id = -1;
		modifications[ai].insert(target);
		webbed++;
	}

	void Tile::deweb( AbilityIdentifier ai ) {
		AbilityIdentifier	target;
		target.tile = globalID;
		target.id = -1;
		if ( modifications.count( ai ) > 0 ) {
			modifications[ai].erase( target );
			webbed--;
		}
	}

	void Tile::motivate( AbilityIdentifier ai ) {
		AbilityIdentifier	target;
		target.tile = globalID;
		target.id = -1;
		modifications[ai].insert(target);
		initiative.motivate();
	}

	void Tile::demotivate( AbilityIdentifier ai ) {
		AbilityIdentifier	target;
		target.tile = globalID;
		target.id = -1;
		if ( modifications.count( ai ) > 0 ) {
			modifications[ai].erase( target );
			initiative.demotivate();
		}
	}

	void Tile::changeInitiative( AbilityIdentifier ai, int amount, bool fix ) {
		AbilityIdentifier	target;
		target.tile = globalID;
		target.id = -1;
		modifications[ai].insert(target);
		initiative.changeInitiative( amount, fix );
	}

	void Tile::dechangeInitiative( AbilityIdentifier ai, int amount, bool fix ) {
		AbilityIdentifier	target;
		target.tile = globalID;
		target.id = -1;
		if ( modifications.count( ai ) > 0 ) {
			modifications[ai].erase( target );
			initiative.changeInitiative( -amount, fix );
		}
	}

	void Tile::changeMelee( AbilityIdentifier ai, int amount ) {
		for ( Ability & atk : attacks ) {
			if ( isMelee( atk ) ) {
				AbilityIdentifier	target = atk.getIdentifier();
				modifications[ai].insert(target);
				atk.modifyStrength( amount );
			}
		}
	}

	void Tile::dechangeMelee( AbilityIdentifier ai, int amount ) {
		for ( AbilityIdentifier ident : modifications[ai] ) {
			Ability & atk = getAttack( ident.id );
			if ( isMelee( atk ) ) {
				atk.modifyStrength( -amount );
				modifications[ai].erase( ident );
			}
		}
	}

	void Tile::changeRanged( AbilityIdentifier ai, int amount ) {
		for ( Ability & atk : attacks ) {
			if ( isRanged( atk ) ) {
				AbilityIdentifier	target = atk.getIdentifier();
				modifications[ai].insert(target);
				atk.modifyStrength( amount );
			}
		}
	}

	void Tile::dechangeRanged( AbilityIdentifier ai, int amount ) {
		for ( AbilityIdentifier ident : modifications[ai] ) {
			Ability & atk = getAttack( ident.id );
			if ( isRanged( atk ) ) {
				atk.modifyStrength( -amount );
				modifications[ai].erase( ident );
			}
		}
	}

	void Tile::addDefensiveAbility( AbilityIdentifier ai, Ability def ) {
		AbilityIdentifier	target = def.getIdentifier();
		modifications[ai].insert(target);
		defensiveAbilities.push_back(def);
	}

	void Tile::delDefensiveAbility( AbilityIdentifier ai ) {
		for ( AbilityIdentifier ident : modifications[ai] ) {
			defensiveAbilities[ident.id].neutralize();
		}
		modifications[ai].clear();
	}

}
