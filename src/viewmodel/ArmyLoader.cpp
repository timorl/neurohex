#include"viewmodel/ArmyLoader.hpp"

namespace viewmodel {

	void setValidTargetTypes( std::vector< std::string > & targettingInfo, std::set< neuro::TileType > & validTargetTypes ) {
		for ( int i = 1; i < static_cast<int>( targettingInfo.size() ); i++ ) {
			if ( targettingInfo[i] == "solid" ) {
				validTargetTypes.insert( neuro::TileType::HQ );
				validTargetTypes.insert( neuro::TileType::MODULE );
				validTargetTypes.insert( neuro::TileType::UNIT );
			} else if ( targettingInfo[i] == "nonsolid" ) {
				validTargetTypes.insert( neuro::TileType::INSTANT_ACTION );
				validTargetTypes.insert( neuro::TileType::FOUNDATION );
			}
		}
	}

	void setValidAffiliations( std::vector< std::string > & targettingInfo, neuro::Targetting & targetting ) {
		for ( int i = 1; i < static_cast<int>( targettingInfo.size() ); i++ ) {
			if ( targettingInfo[i] == "enemy" ) {
				targetting.enemy = true;
			} else if ( targettingInfo[i] == "own" ) {
				targetting.own = true;
			}
		}
	}

	void parseTargetting( utility::DFStyleParser & parser, neuro::Targetting & targetting ) {
		while ( parser.hasNextToken() ) {
			std::vector< std::string > targettingInfo = parser.getNextToken();
			std::string type = targettingInfo[0];
			if ( type == "TARGETTINGEND" ) {
				break;
			} else if ( type == "TYPE" ) {
				targetting.type = neuro::targettingTypeStringMap.at(targettingInfo[1]);
			} else if ( type == "ACTUAL_TARGETS" ) {
				targetting.actualTargets = std::stoi( targettingInfo[1] );
			} else if ( type == "REQUIRED_TARGETS" ) {
				targetting.requiredTargets = std::stoi( targettingInfo[1] );
			} else if ( type == "RANGE" ) {
				targetting.range = std::stoi( targettingInfo[1] );
			} else if ( type == "VALID_TARGET_TYPES" ) {
				setValidTargetTypes( targettingInfo, targetting.validTargetTypes );
			} else if ( type == "VALID_TARGET_AFFILIATION" ) {
				setValidAffiliations( targettingInfo, targetting );
			}
		}
	}

	void setPlacingActions( std::vector< std::string > & placingInfo, std::string & actions ) {
		for ( int i = 1; i < static_cast<int>( placingInfo.size() ); i++ ) {
			if ( placingInfo[i] == "place" ) {
				actions += "p";
			}
		}
	}

	void parsePlacing( utility::DFStyleParser & parser, std::unique_ptr< neuro::Tile::Placing > & placingP ) {
		std::string actions;
		neuro::Targetting targetting;
		while ( parser.hasNextToken() ) {
			std::vector< std::string > placingInfo = parser.getNextToken();
			std::string type = placingInfo[0];
			if ( type == "PLACINGEND" ) {
				break;
			} else if ( type == "ACTIONS" ) {
				setPlacingActions( placingInfo, actions );
			} else if ( type == "TARGETTINGBEGIN" ) {
				parseTargetting( parser, targetting );
			}
		}
		placingP.reset( new neuro::Tile::Placing( actions, targetting ) );
	}

	void setAttackActions( std::vector< std::string > & attackInfo, std::string & actions ) {
		for ( int i = 1; i < static_cast<int>( attackInfo.size() ); i++ ) {
			if ( attackInfo[i] == "damage" ) {
				actions += "d";
			}
		}
	}

	void setAttackTypes( std::vector< std::string > & attackInfo, bool & melee, bool & ranged ) {
		for ( int i = 1; i < static_cast<int>( attackInfo.size() ); i++ ) {
			if ( attackInfo[i] == "melee" ) {
				melee = true;
			} else if ( attackInfo[i] == "ranged" ) {
				ranged = true;
			}
		}
	}

	void parseAttack( utility::DFStyleParser & parser, std::list< neuro::Tile::Attack > & attacks ) {
		int direction = -1;
		neuro::Targetting targetting;
		bool melee = false;
		bool ranged = false;
		int strength;
		std::string actions;
		while ( parser.hasNextToken() ) {
			std::vector< std::string > attackInfo = parser.getNextToken();
			std::string type = attackInfo[0];
			if ( type == "ATTACKEND" ) {
				break;
			} else if ( type == "TYPE" ) {
				setAttackTypes( attackInfo, melee, ranged );
			} else if ( type == "ACTIONS" ) {
				setAttackActions( attackInfo, actions );
			} else if ( type == "STRENGTH" ) {
				strength	= std::stoi( attackInfo[1] );
			} else if ( type == "DIRECTION" ) {
				direction	= std::stoi( attackInfo[1] );
			} else if ( type == "TARGETTINGBEGIN" ) {
				parseTargetting( parser, targetting );
			}
		}
		attacks.emplace_back( direction, targetting, melee, ranged, strength, actions );
	}

	void parseAbility( utility::DFStyleParser & parser, std::list< neuro::Tile::Ability > & abilities ) {
		//FIXME: This is a stub.
	}

	void parseModifier( utility::DFStyleParser & parser, std::list< neuro::Tile::Modifier > & abilities ) {
		//FIXME: This is a stub.
	}

	void parseTile( utility::DFStyleParser & parser, std::vector< neuro::TileP > & tiles, int amount ) {
		std::string name;
		neuro::TileType tileType;
		std::set< int > initiative;
		std::unique_ptr< neuro::Tile::Placing > placingP;
		std::list< neuro::Tile::Ability > onBattleStart;
		std::list< neuro::Tile::Ability > activeAbilities;
		std::list< neuro::Tile::Ability > defensiveAbilities;
		std::list< neuro::Tile::Attack> attacks;
		std::list< neuro::Tile::Modifier > modifiers;
		int health = 1;
		while ( parser.hasNextToken() ) {
			std::vector< std::string > tileInfo = parser.getNextToken();
			std::string type = tileInfo[0];
			if ( type == "TILEEND" ) {
				break;
			} else if ( type == "NAME" ) {
				name = tileInfo[1];
			} else if ( type == "TYPE" ) {
				tileType = neuro::tileTypeStringMap.at(tileInfo[1]);
			} else if ( type == "INITIATIVE" ) {
				for ( int i = 1; i < static_cast<int>( tileInfo.size() ); i++ ) {
					initiative.insert( std::stoi( tileInfo[i] ) );
				}
			} else if ( type == "HEALTH" ) {
				health = std::stoi( tileInfo[1] );
			} else if ( type == "PLACINGBEGIN" ) {
				parsePlacing( parser, placingP );
			} else if ( type == "ATTACKBEGIN" ) {
				parseAttack( parser, attacks );
			} else if ( type == "BATTLESTARTBEGIN" ) {
				parseAbility( parser, onBattleStart );
			} else if ( type == "ACTIVEBEGIN" ) {
				parseAbility( parser, activeAbilities );
			} else if ( type == "DEFENSIVEBEGIN" ) {
				parseAbility( parser, defensiveAbilities );
			} else if ( type == "MODIFIERBEGIN" ) {
				parseModifier( parser, modifiers );
			}
		}
		for ( int i = 0; i < amount; i++ ) {
			tiles.push_back( neuro::TileP( new neuro::Tile( name, tileType, *placingP, health, initiative, onBattleStart, attacks, modifiers, activeAbilities, defensiveAbilities ) ) );
		}
	}

	void parseArmy( utility::DFStyleParser & parser, std::string & armyName, std::string & armyDescription, std::vector< neuro::TileP > & tiles ) {
		while ( parser.hasNextToken() ) {
			std::vector< std::string > armyInfo = parser.getNextToken();
			std::string type = armyInfo[0];
			if ( type == "NAME" ) {
				armyName = armyInfo[1];
			} else if ( type == "DESCRIPTION" ) {
				armyDescription	= armyInfo[1];
			} else if ( type == "TILEBEGIN" ) {
				int amount = std::stoi( armyInfo[1] );
				parseTile( parser, tiles, amount );
			}
		}
	}

	void ArmyLoader::loadArmies( std::string directory ) {
		utility::DFStyleParser parser( directory );
		while ( parser.nextFile() ) {
			std::string armyName;
			std::string armyDescription;
			std::vector< neuro::TileP > tiles;
			parseArmy( parser, armyName, armyDescription, tiles );
			if ( armies.count(armyName) == 0 ) {
				armies[armyName] = neuro::ArmyP( new neuro::Army(tiles) );
				descriptions[armyName] = armyDescription;
			}
		}
	}

	neuro::ArmyP ArmyLoader::getArmy( std::string name ) const {
		return armies.at(name);
	}

}
