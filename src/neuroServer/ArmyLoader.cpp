#include<iostream>
#include"neuroServer/ArmyLoader.hpp"

namespace neuroServer {

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

	bool parseTargetting( utility::DFStyleReader & parser, neuro::Targetting & targetting ) {
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

	bool setAbilityActions( std::vector< std::string > & info, std::string & actions ) {
		const std::string abortMessage = "Aborting setting ability actions: ";
		for ( int i = 1; i < static_cast<int>( info.size() ); i++ ) {
			if ( info[i] == "ranged" ) {
				actions += "r";
			} else if ( info[i] == "melee" ) {
				actions += "m";
			} else if ( info[i] == "damage" ) {
				actions += "d";
			} else if ( info[i] == "gauss" ) {
				actions += "g";
			} else if ( info[i] == "place" ) {
				actions += "P";
			} else if ( info[i] == "move" ) {
				actions += "m";
			} else if ( info[i] == "push" ) {
				actions += "p";
			} else if ( info[i] == "battle" ) {
				actions += "b";
			} else if ( info[i] == "autodestroy" ) {
				actions += "a";
			} else if ( info[i] == "save" ) {
				actions += "s";
			} else if ( info[i] == "motivate" ) {
				actions += "M";
			} else if ( info[i] == "initiative" ) {
				actions += "i";
			} else if ( info[i] == "web" ) {
				actions += "w";
			} else {
				std::cerr << abortMessage << unrecognizedArgumentMessage << "ACTIONS" << " : " << info[i] << std::endl;
				return false;
			}
		}
		return true;
	}

	bool parseAbility( utility::DFStyleReader & parser, neuro::Tile::Abilities & abilities, neuro::AbilityGroup group ) {
		const std::string abortMessage = "Aborting ability parse: ";
		const std::string abilityActionsFailedMessage = "Reading ability actions failed.";
		std::string name;
		std::string description;
		int direction;
		neuro::Targetting targetting;
		int strength;
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
			} else if ( type == "STRENGTH" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				strength = std::stoi( info[1] );
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
		abilities.emplace_back( name, description, direction, targetting, strength, actions, abilities.size(), group );
		return true;
	}

	bool parseTile( utility::DFStyleReader & parser, std::vector< neuro::Tile > & tiles, int amount ) {
		const std::string abortMessage = "Aborting tile parse: ";
		const std::string abilityFailedMessage = "Loading of ability failed.";
		std::string name;
		neuro::TileType tileType;
		std::set< int > initiative;
		neuro::Tile::Abilities placing;
		neuro::Tile::Abilities onBattleStart;
		neuro::Tile::Abilities activeAbilities;
		neuro::Tile::Abilities defensiveAbilities;
		neuro::Tile::Abilities attacks;
		neuro::Tile::Abilities modifiers;
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
			} else if ( type == "ABILITYBEGIN" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				if ( info[1] == "battleStart" ) {
					if ( !parseAbility( parser, onBattleStart, neuro::AbilityGroup::BATTLE_START ) ) {
						std::cerr << abortMessage << abilityFailedMessage << std::endl;
						return false;
					}
				} else if ( info[1] == "active" ) {
					if ( !parseAbility( parser, activeAbilities, neuro::AbilityGroup::ACTIVE ) ) {
						std::cerr << abortMessage << abilityFailedMessage << std::endl;
						return false;
					}
				} else if ( info[1] == "defensive" ) {
					if ( !parseAbility( parser, defensiveAbilities, neuro::AbilityGroup::DEFENSIVE ) ) {
						std::cerr << abortMessage << abilityFailedMessage << std::endl;
						return false;
					}
				} else if ( info[1] == "placing" ) {
					if ( !parseAbility( parser, placing, neuro::AbilityGroup::PLACING ) ) {
						std::cerr << abortMessage << abilityFailedMessage << std::endl;
						return false;
					}
				} else if ( info[1] == "attack" ) {
					if ( !parseAbility( parser, attacks, neuro::AbilityGroup::ATTACK ) ) {
						std::cerr << abortMessage << abilityFailedMessage << std::endl;
						return false;
					}
				} else if ( info[1] == "modifier" ) {
					if ( !parseAbility( parser, modifiers, neuro::AbilityGroup::MODIFIER ) ) {
						std::cerr << abortMessage << abilityFailedMessage << std::endl;
						return false;
					}
				} else {
					std::cerr << abortMessage << unrecognizedArgumentMessage << type << " : " << info[1] << std::endl;
					return false;
				}
			} else {
				std::cerr << unrecognizedTokenMessage << type << std::endl;
			}
		}
		if ( name.empty() || placing.empty() ) {
			std::cerr << abortMessage << missingInformationMessage << std::endl;
			return false;
		}
		for ( int i = 0; i < amount; i++ ) {
			tiles.emplace_back( name, tileType, *placing.begin(), health, initiative, onBattleStart, attacks, modifiers, activeAbilities, defensiveAbilities );
		}
		return true;
	}

	bool parseArmy( utility::DFStyleReader & parser, std::string & armyName, std::string & armyDescription, std::vector< neuro::Tile > & tiles ) {
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
		utility::DFDirectoryReader reader( directory );
		int armyCount = 0;
		while ( reader.nextFile() ) {
			utility::DFStyleReader parser = reader.getCurrentFileReader();
			std::string armyName;
			std::string armyDescription;
			std::vector< neuro::Tile > tiles;
			if ( parseArmy( parser, armyName, armyDescription, tiles ) && armies.count(armyName) == 0 ) {
				armyCount++;
				armies[armyName] = std::move(tiles);
				descriptions[armyName] = armyDescription;
				sigModified(*this);
			}
		}
		if ( armyCount == 0 ) {
			std::clog << "No new armies in directory " << directory << std::endl;
		} else {
			std::clog << "Loaded " << armyCount << " new arm" << ( (armyCount > 1)?"ies":"y" ) << " from directory " << directory << std::endl;
		}
	}

}
