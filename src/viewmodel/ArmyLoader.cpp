#include<iostream>
#include"viewmodel/ArmyLoader.hpp"

namespace viewmodel {

	const std::string unrecognizedTokenMessage = "Ignoring unrecognized token: ";
	const std::string emptyTokenMessage = "Empty token.";
	const std::string tooFewArgumentsMessage = "Too few arguments for token: ";
	const std::string missingInformationMessage = "Missing information.";
	const std::string unrecognizedArgumentMessage = "Unrecognized argument: ";
	const std::string targettingFailedMessage = "Loading of targetting failed.";

	bool setTargetTiles( std::vector< std::string > & info, bool & targetTiles ) {
		const std::string abortMessage = "Aborting determining what to target: ";
		if (  static_cast<int>( info.size() ) < 2 ) {
			std::cerr << abortMessage << tooFewArgumentsMessage << "TARGETS" << std::endl;
			return false;
		}
		if ( info[1] == "tiles" ) {
			targetTiles = true;
		} else if ( info[1] == "fields" ) {
			targetTiles = false;
		} else {
			std::cerr << abortMessage << unrecognizedArgumentMessage << "TARGETS" << " : " << info[1] << std::endl;
			return false;
		}
		return true;
	}

	bool setValidTargetTypes( std::vector< std::string > & info, std::set< neuro::TileType > & validTargetTypes ) {
		const std::string abortMessage = "Aborting setting valid target types: ";
		for ( int i = 1; i < static_cast<int>( info.size() ); i++ ) {
			if ( info[i] == "solid" ) {
				validTargetTypes.insert( neuro::TileType::HQ );
				validTargetTypes.insert( neuro::TileType::MODULE );
				validTargetTypes.insert( neuro::TileType::UNIT );
			} else if ( info[i] == "nonsolid" ) {
				validTargetTypes.insert( neuro::TileType::INSTANT_ACTION );
				validTargetTypes.insert( neuro::TileType::FOUNDATION );
			} else if ( info[i] == "unit" ) {
				validTargetTypes.insert( neuro::TileType::UNIT );
			} else if ( info[i] == "module" ) {
				validTargetTypes.insert( neuro::TileType::MODULE );
			} else {
				std::cerr << abortMessage << unrecognizedArgumentMessage << "VALID_TARGET_TYPES" << " : " << info[i] << std::endl;
				return false;
			}
		}
		return true;
	}

	bool setValidAffiliations( std::vector< std::string > & info, neuro::Targetting & targetting ) {
		const std::string abortMessage = "Aborting setting valid target affiliations: ";
		for ( int i = 1; i < static_cast<int>( info.size() ); i++ ) {
			if ( info[i] == "enemy" ) {
				targetting.enemy = true;
			} else if ( info[i] == "own" ) {
				targetting.own = true;
			} else {
				std::cerr << abortMessage << unrecognizedArgumentMessage << "VALID_TARGET_AFFILIATION" << " : " << info[i] << std::endl;
				return false;
			}
		}
		return true;
	}

	bool parseTargetting( utility::DFStyleParser & parser, neuro::Targetting & targetting ) {
		const std::string abortMessage = "Aborting targetting parse: ";
		const std::string targetTilesFailedMessage = "Reading what to target failed.";
		const std::string targetTypesFailedMessage = "Reading target types failed.";
		const std::string targetAffiliationsFailedMessage = "Reading target affiliations failed.";
		while ( parser.hasNextToken() ) {
			std::vector< std::string > info = parser.getNextToken();
			if ( static_cast<int>( info.size() ) < 1 ) {
				std::cerr << abortMessage << emptyTokenMessage << std::endl;
				return false;
			}
			std::string type = info[0];
			if ( type == "TARGETTINGEND" ) {
				break;
			} else if ( type == "TYPE" ) {
				if ( static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				targetting.type = neuro::targettingTypeStringMap.at(info[1]);
			} else if ( type == "ACTUAL_TARGETS" ) {
				if ( static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				targetting.actualTargets = std::stoi( info[1] );
			} else if ( type == "REQUIRED_TARGETS" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				targetting.requiredTargets = std::stoi( info[1] );
			} else if ( type == "RANGE" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				targetting.range = std::stoi( info[1] );
			} else if ( type == "TARGETS" ) {
				if ( !setTargetTiles( info, targetting.targetTiles ) ) {
					std::cerr << abortMessage << targetTilesFailedMessage << std::endl;
					return false;
				}
			} else if ( type == "VALID_TARGET_TYPES" ) {
				if ( !setValidTargetTypes( info, targetting.validTargetTypes ) ) {
					std::cerr << abortMessage << targetTypesFailedMessage << std::endl;
					return false;
				}
			} else if ( type == "VALID_TARGET_AFFILIATION" ) {
				if ( !setValidAffiliations( info, targetting ) ) {
					std::cerr << abortMessage << targetAffiliationsFailedMessage << std::endl;
					return false;
				}
			} else {
				std::cerr << unrecognizedTokenMessage << type << std::endl;
			}
		}
		return true;
	}

	bool setPlacingActions( std::vector< std::string > & info, std::string & actions ) {
		const std::string abortMessage = "Aborting setting placing actions: ";
		for ( int i = 1; i < static_cast<int>( info.size() ); i++ ) {
			if ( info[i] == "place" ) {
				actions += "P";
			} else if ( info[i] == "damage" ) {
				actions += "d";
			} else if ( info[i] == "move" ) {
				actions += "m";
			} else if ( info[i] == "push" ) {
				actions += "p";
			} else if ( info[i] == "battle" ) {
				actions += "b";
			} else {
				std::cerr << abortMessage << unrecognizedArgumentMessage << "ACTIONS" << " : " << info[i] << std::endl;
				return false;
			}
		}
		return true;
	}

	bool parsePlacing( utility::DFStyleParser & parser, std::unique_ptr< neuro::Tile::Placing > & placingP ) {
		const std::string abortMessage = "Aborting placing parse: ";
		const std::string placingActionsFailedMessage = "Reading placing actions failed.";
		std::string actions;
		neuro::Targetting targetting;
		while ( parser.hasNextToken() ) {
			std::vector< std::string > info = parser.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				std::cerr << abortMessage << emptyTokenMessage << std::endl;
				return false;
			}
			std::string type = info[0];
			if ( type == "PLACINGEND" ) {
				break;
			} else if ( type == "ACTIONS" ) {
				if ( !setPlacingActions( info, actions ) ) {
					std::cerr << abortMessage << placingActionsFailedMessage << std::endl;
					return false;
				}
			} else if ( type == "TARGETTINGBEGIN" ) {
				if ( !parseTargetting( parser, targetting ) ) {
					std::cerr << abortMessage << targettingFailedMessage << std::endl;
					return false;
				}
			} else {
				std::cerr << unrecognizedTokenMessage << type << std::endl;
			}
		}
		if ( actions.empty() ) {
			std::cerr << abortMessage << missingInformationMessage << std::endl;
			return false;
		}
		placingP.reset( new neuro::Tile::Placing( actions, targetting ) );
		return true;
	}

	bool setAttackActions( std::vector< std::string > & info, std::string & actions ) {
		const std::string abortMessage = "Aborting setting attack actions: ";
		for ( int i = 1; i < static_cast<int>( info.size() ); i++ ) {
			if ( info[i] == "damage" ) {
				actions += "d";
			} else if ( info[i] == "become" ) {
				if (  static_cast<int>( info.size() ) < i + 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << "become" << std::endl;
					return false;
				}
				actions += "b";
				i++;
				if ( info[i] == "ranged" ) {
					actions += "r";
				} else if ( info[i] == "nonranged" ) {
					actions += "R";
				} else {
					std::cerr << abortMessage << unrecognizedArgumentMessage << "become" << " : " << info[i] << std::endl;
					return false;
				}
			} else if ( info[i] == "autodestroy" ) {
				actions += "a";
			} else {
				std::cerr << abortMessage << unrecognizedArgumentMessage << "ACTIONS" << " : " << info[i] << std::endl;
				return false;
			}
		}
		return true;
	}

	bool setAttackTypes( std::vector< std::string > & info, bool & melee, bool & ranged ) {
		const std::string abortMessage = "Aborting setting attack types: ";
		for ( int i = 1; i < static_cast<int>( info.size() ); i++ ) {
			if ( info[i] == "melee" ) {
				melee = true;
			} else if ( info[i] == "ranged" ) {
				ranged = true;
			} else if ( info[i] == "none" ) {
				melee = false;
				ranged = false;
			} else {
				std::cerr << abortMessage << unrecognizedArgumentMessage << "TYPE" << " : " << info[i] << std::endl;
				return false;
			}
		}
		return true;
	}

	bool parseAttack( utility::DFStyleParser & parser, std::vector< neuro::Tile::Attack > & attacks ) {
		const std::string abortMessage = "Aborting attack parse: ";
		const std::string attackActionsFailedMessage = "Reading attack actions failed.";
		const std::string attackTypesFailedMessage = "Reading attack types failed.";
		int direction = -1;
		neuro::Targetting targetting;
		bool melee = false;
		bool ranged = false;
		int strength;
		std::string actions;
		while ( parser.hasNextToken() ) {
			std::vector< std::string > info = parser.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				std::cerr << abortMessage << emptyTokenMessage << std::endl;
				return false;
			}
			std::string type = info[0];
			if ( type == "ATTACKEND" ) {
				break;
			} else if ( type == "TYPE" ) {
				if ( !setAttackTypes( info, melee, ranged ) ) {
					std::cerr << abortMessage << attackTypesFailedMessage << std::endl;
					return false;
				}
			} else if ( type == "ACTIONS" ) {
				if ( !setAttackActions( info, actions ) ) {
					std::cerr << abortMessage << attackActionsFailedMessage << std::endl;
					return false;
				}
			} else if ( type == "STRENGTH" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				strength	= std::stoi( info[1] );
			} else if ( type == "DIRECTION" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				direction	= std::stoi( info[1] );
			} else if ( type == "TARGETTINGBEGIN" ) {
				if ( !parseTargetting( parser, targetting ) ) {
					std::cerr << abortMessage << targettingFailedMessage << std::endl;
					return false;
				}
			} else {
				std::cerr << unrecognizedTokenMessage << type << std::endl;
			}
		}
		if ( actions.empty() ) {
			std::cerr << abortMessage << missingInformationMessage << std::endl;
			return false;
		}
		attacks.emplace_back( direction, targetting, melee, ranged, strength, actions );
		return true;
	}

	bool setAbilityActions( std::vector< std::string > & info, std::string & actions ) {
		const std::string abortMessage = "Aborting setting ability actions: ";
		for ( int i = 1; i < static_cast<int>( info.size() ); i++ ) {
			if ( info[i] == "prevent" ) {
				if (  static_cast<int>( info.size() ) < i + 3 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << "prevent" << std::endl;
					return false;
				}
				actions += "p";
				i++;
				actions += info[i];
				i++;
				if ( info[i] == "ranged" ) {
					actions += "r";
				} else {
					std::cerr << abortMessage << unrecognizedArgumentMessage << "prevent" << " : " << info[i] << std::endl;
					return false;
				}
			} else {
				std::cerr << abortMessage << unrecognizedArgumentMessage << "ACTIONS" << " : " << info[i] << std::endl;
				return false;
			}
		}
		return true;
	}

	bool parseAbility( utility::DFStyleParser & parser, std::vector< neuro::Tile::Ability > & abilities ) {
		const std::string abortMessage = "Aborting ability parse: ";
		const std::string abilityActionsFailedMessage = "Reading ability actions failed.";
		std::string name;
		std::string description;
		int direction;
		neuro::Targetting targetting;
		std::string actions;
		while ( parser.hasNextToken() ) {
			std::vector< std::string > info = parser.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				std::cerr << abortMessage << emptyTokenMessage << std::endl;
				return false;
			}
			std::string type = info[0];
			if ( type == "ABILITYEND" ) {
				break;
			} else if ( type == "NAME" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				name = info[1];
			} else if ( type == "DESCRIPTION" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				description = info[1];
			} else if ( type == "DIRECTION" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				direction	= std::stoi( info[1] );
			} else if ( type == "ACTIONS" ) {
				if ( !setAbilityActions( info, actions ) ) {
					std::cerr << abortMessage << abilityActionsFailedMessage << std::endl;
					return false;
				}
			} else if ( type == "TARGETTINGBEGIN" ) {
				if ( !parseTargetting( parser, targetting ) ) {
					std::cerr << abortMessage << targettingFailedMessage << std::endl;
					return false;
				}
			} else {
				std::cerr << unrecognizedTokenMessage << type << std::endl;
			}
		}
		if ( name.empty() || description.empty() || actions.empty() ) {
			std::cerr << abortMessage << missingInformationMessage << std::endl;
			return false;
		}
		abilities.emplace_back( name, description, direction, targetting, actions );
		return true;
	}

	bool setModifierActions( std::vector< std::string > & info, std::string & actions ) {
		const std::string abortMessage = "Aborting setting modifier actions: ";
		for ( int i = 1; i < static_cast<int>( info.size() ); i++ ) {
			if ( info[i] == "web" ) {
				actions += "w";
			} else if ( info[i] == "increase" ) {
				if (  static_cast<int>( info.size() ) < i + 3 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << "increase" << std::endl;
					return false;
				}
				actions += "i";
				i++;
				actions += info[i];
				i++;
				if ( info[i] == "initiative" ) {
					actions += "i";
				} else if ( info[i] == "melee" ) {
					actions += "m";
				} else if ( info[i] == "ranged" ) {
					actions += "r";
				} else {
					std::cerr << abortMessage << unrecognizedArgumentMessage << "increase" << " : " << info[i] << std::endl;
					return false;
				}
			} else if ( info[i] == "save" ) {
				actions += "s";
			} else if ( info[i] == "motivate" ) {
				actions += "m";
			} else {
				std::cerr << abortMessage << unrecognizedArgumentMessage << "ACTIONS" << " : " << info[i] << std::endl;
				return false;
			}
		}
		return true;
	}

	bool parseModifier( utility::DFStyleParser & parser, std::vector< neuro::Tile::Modifier > & modifiers ) {
		const std::string abortMessage = "Aborting modifier parse: ";
		const std::string modifierActionsFailedMessage = "Reading modifier actions failed.";
		int direction;
		neuro::Targetting targetting;
		std::string actions;
		while ( parser.hasNextToken() ) {
			std::vector< std::string > info = parser.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				std::cerr << abortMessage << emptyTokenMessage << std::endl;
				return false;
			}
			std::string type = info[0];
			if ( type == "MODIFIEREND" ) {
				break;
			} else if ( type == "DIRECTION" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				direction	= std::stoi( info[1] );
			} else if ( type == "ACTIONS" ) {
				if ( !setModifierActions( info, actions ) ) {
					std::cerr << abortMessage << modifierActionsFailedMessage << std::endl;
					return false;
				}
			} else if ( type == "TARGETTINGBEGIN" ) {
				if ( !parseTargetting( parser, targetting ) ) {
					std::cerr << abortMessage << targettingFailedMessage << std::endl;
					return false;
				}
			} else {
				std::cerr << unrecognizedTokenMessage << type << std::endl;
			}
		}
		if ( actions.empty() ) {
			std::cerr << abortMessage << missingInformationMessage << std::endl;
			return false;
		}
		modifiers.emplace_back( direction, targetting, actions );
		return true;
	}

	bool parseTile( utility::DFStyleParser & parser, std::vector< neuro::TileP > & tiles, int amount ) {
		const std::string abortMessage = "Aborting tile parse: ";
		const std::string placingFailedMessage = "Loading of placing failed.";
		const std::string attackFailedMessage = "Loading of attack failed.";
		const std::string abilityFailedMessage = "Loading of ability failed.";
		const std::string modifierFailedMessage = "Loading of modifier failed.";
		std::string name;
		neuro::TileType tileType;
		std::set< int > initiative;
		std::unique_ptr< neuro::Tile::Placing > placingP;
		std::vector< neuro::Tile::Ability > onBattleStart;
		std::vector< neuro::Tile::Ability > activeAbilities;
		std::vector< neuro::Tile::Ability > defensiveAbilities;
		std::vector< neuro::Tile::Attack> attacks;
		std::vector< neuro::Tile::Modifier > modifiers;
		int health = 1;
		while ( parser.hasNextToken() ) {
			std::vector< std::string > info = parser.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				std::cerr << abortMessage << emptyTokenMessage << std::endl;
				return false;
			}
			std::string type = info[0];
			if ( type == "TILEEND" ) {
				break;
			} else if ( type == "NAME" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				name = info[1];
			} else if ( type == "TYPE" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				tileType = neuro::tileTypeStringMap.at(info[1]);
			} else if ( type == "INITIATIVE" ) {
				for ( int i = 1; i < static_cast<int>( info.size() ); i++ ) {
					initiative.insert( std::stoi( info[i] ) );
				}
			} else if ( type == "HEALTH" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				health = std::stoi( info[1] );
			} else if ( type == "PLACINGBEGIN" ) {
				if ( !parsePlacing( parser, placingP ) ) {
					std::cerr << abortMessage << placingFailedMessage << std::endl;
					return false;
				}
			} else if ( type == "ATTACKBEGIN" ) {
				if ( !parseAttack( parser, attacks ) ) {
					std::cerr << abortMessage << attackFailedMessage << std::endl;
					return false;
				}
			} else if ( type == "ABILITYBEGIN" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				if ( info[1] == "battleStart" ) {
					if ( !parseAbility( parser, onBattleStart ) ) {
						std::cerr << abortMessage << abilityFailedMessage << std::endl;
						return false;
					}
				} else if ( info[1] == "active" ) {
					if ( !parseAbility( parser, activeAbilities ) ) {
						std::cerr << abortMessage << abilityFailedMessage << std::endl;
						return false;
					}
				} else if ( info[1] == "defensive" ) {
					if ( !parseAbility( parser, defensiveAbilities ) ) {
						std::cerr << abortMessage << abilityFailedMessage << std::endl;
						return false;
					}
				} else {
					std::cerr << abortMessage << unrecognizedArgumentMessage << type << " : " << info[1] << std::endl;
					return false;
				}
			} else if ( type == "MODIFIERBEGIN" ) {
				if ( !parseModifier( parser, modifiers ) ) {
					std::cerr << abortMessage << modifierFailedMessage << std::endl;
					return false;
				}
			} else {
				std::cerr << unrecognizedTokenMessage << type << std::endl;
			}
		}
		if ( name.empty() || !placingP ) {
			std::cerr << abortMessage << missingInformationMessage << std::endl;
			return false;
		}
		for ( int i = 0; i < amount; i++ ) {
			tiles.emplace_back( new neuro::Tile( name, tileType, *placingP, health, initiative, onBattleStart, attacks, modifiers, activeAbilities, defensiveAbilities ) );
		}
		return true;
	}

	bool parseArmy( utility::DFStyleParser & parser, std::string & armyName, std::string & armyDescription, std::vector< neuro::TileP > & tiles ) {
		const std::string abortMessage = "Aborting army load: ";
		const std::string tileFailedMessage = "Loading of tile failed.";
		while ( parser.hasNextToken() ) {
			std::vector< std::string > info = parser.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				std::cerr << abortMessage << emptyTokenMessage << std::endl;
				return false;
			}
			std::string type = info[0];
			if ( type == "NAME" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				armyName = info[1];
			} else if ( type == "DESCRIPTION" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				armyDescription	= info[1];
			} else if ( type == "TILEBEGIN" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				int amount = std::stoi( info[1] );
				if ( !parseTile( parser, tiles, amount ) ) {
					std::cerr << abortMessage << tileFailedMessage << std::endl;
					return false;
				}
			} else {
				std::cerr << unrecognizedTokenMessage << type << std::endl;
			}
		}
		if ( armyName.empty() || armyDescription.empty() || tiles.empty() ) {
			std::cerr << abortMessage << missingInformationMessage << std::endl;
			return false;
		}
		return true;
	}

	void ArmyLoader::loadArmies( std::string directory ) {
		utility::DFStyleParser parser( directory );
		int armyCount = 0;
		while ( parser.nextFile() ) {
			std::string armyName;
			std::string armyDescription;
			std::vector< neuro::TileP > tiles;
			if ( parseArmy( parser, armyName, armyDescription, tiles ) && armies.count(armyName) == 0 ) {
				armyCount++;
				armies[armyName] = neuro::ArmyP( new neuro::Army(tiles) );
				descriptions[armyName] = armyDescription;
			}
		}
		if ( armyCount == 0 ) {
			std::clog << "No new armies in directory " << directory << std::endl;
		} else {
			std::clog << "Loaded " << armyCount << " new arm" << ( (armyCount > 1)?"ies":"y" ) << " from directory " << directory << std::endl;
		}
	}

	neuro::ArmyP ArmyLoader::getArmy( std::string name ) const {
		return neuro::ArmyP( new neuro::Army( *armies.at(name) ) );
	}

}
