#include"neuro/Game.hpp"

namespace neuro {

	bool Move::fillFromDFStyle(utility::DFStyleReader & input) {
		while ( input.hasNextToken() ) {
			std::vector< std::string > info = input.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				return false;
			}
			std::string type = info[0];
			if ( type == "MOVEEND" ) {
				return true;
			} else if ( type == "END_TURN" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				if ( info[1] == "true" ) {
					endTurn = true;
				} else if ( info[1] == "false" ) {
					endTurn = false;
				} else {
					return false;
				}
			} else if ( type == "DISCARD" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				if ( info[1] == "true" ) {
					discard = true;
				} else if ( info[1] == "false" ) {
					discard = false;
				} else {
					return false;
				}
			} else if ( type == "ABILITYIDENTIFIERBEGIN" ) {
				if ( !abilityIdentifier.fillFromDFStyle(input) ) {
					return false;
				}
			} else {
				return false;
			}
		}
		return false;
	}

	void Move::encodeAsDFStyle(utility::DFStyleCreator & output) const {
		output.startToken("END_MOVE");
		output.addToToken(endTurn);
		output.endToken();
		output.startToken("DISCARD");
		output.addToToken(discard);
		output.endToken();
		output.startToken("ABILITYIDENTIFIERBEGIN");
		output.endToken();
		abilityIdentifier.encodeAsDFStyle(output);
		output.startToken("MOVEEND");
		output.endToken();
	}

	bool Target::fillFromDFStyle(utility::DFStyleReader & input) {
		tiles.clear();
		while ( input.hasNextToken() ) {
			std::vector< std::string > info = input.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				return false;
			}
			std::string type = info[0];
			if ( type == "TARGETEND" ) {
				return true;
			} else if ( type == "COORDINATES" ) {
				if (  static_cast<int>( info.size() ) < 3 ) {
					return false;
				}
				coords.first = std::stoi(info[1]);
				coords.second = std::stoi(info[2]);
			} else if ( type == "ORIENTATION" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				orientation = std::stoi(info[1]);
			} else if ( type == "TILES" ) {
				for ( int i = 1; i < static_cast<int>( info.size() ); i++ ) {
					tiles.push_back(std::stoi(info[i]));
				}
			} else {
				return false;
			}
		}
		return false;
	}

	void Target::encodeAsDFStyle(utility::DFStyleCreator & output) const {
		output.startToken("COORDINATES");
		output.addToToken(coords.first);
		output.addToToken(coords.second);
		output.endToken();
		output.startToken("ORIENTATION");
		output.addToToken(orientation);
		output.endToken();
		output.startToken("TILES");
		for ( const int & t : tiles ) {
			output.addToToken(t);
		}
		output.endToken();
		output.startToken("TARGETEND");
		output.endToken();
	}

	Game::Game(BoardDescription board, std::vector< std::vector<neuro::Tile> > armies, int initialHealth) :
		board(board),
		currentPlayer(0),
		noArmy(false),
		firstTurns(2) {
		Tile::allTiles.clear();
		for ( int i = 0; i < static_cast<int>( armies.size() ); i++ ) {
			std::vector< int > army;
			for ( neuro::Tile tile : armies[i] ) {
				int curTileId = Tile::allTiles.size();
				tile.setId(curTileId);
				army.push_back(curTileId);
				Tile::allTiles.push_back(tile);
			}
			players.emplace_back(i, army, initialHealth);
		}
	}

	int Game::getNumberOfLivingPlayers() const {
		int result = 0;
		for ( const Player & player : players ) {
			if ( player.isAlive() ) {
				result++;
			}
		}
		return result;
	}

	bool Game::isFinished() const {
		if ( (noArmy && players[currentPlayer].getArmy().isEmpty()) || 
				( getNumberOfLivingPlayers() <= 1 ) ) {
			return true;
		}
		return false;
	}

	Players & Game::getPlayers() {
		return players;
	}

	int Game::startTurn() {
		players[currentPlayer].startTurn( firstTurns );
		noArmy = noArmy || players[currentPlayer].armyEmpty();
		return currentPlayer;
	}

	void Game::endTurn() {
		if ( !isFinished() ) {
			do {
				currentPlayer++;
				currentPlayer	%= getNumberOfPlayers();
			} while ( !players[currentPlayer].isAlive() );
		}
	}

	void Game::discardTile(int tile) {
		players[currentPlayer].discardTile(tile);
	}

	void Game::clearDead() {
		const Tiles & tiles = board.getTiles();
		for ( auto tileColumn : tiles ) {
			for ( auto tileList : tileColumn ) {
				for ( auto tileOnBoard : tileList ) {
					if ( !Tile::allTiles[tileOnBoard.first].isAlive() ) {
						removeFromBoard( tileOnBoard.first );
					}
				}
			}
		}
	}

	void Game::updateTile( Coordinates coords, Orientation orientation, int tile ) {
		Tile::allTiles[tile].clearModifications();
		Targets theTarget;
		Target trgt;
		trgt.tiles.push_back( tile );
		trgt.coords = coords;
		trgt.orientation = orientation;
		theTarget.push_back( trgt );
		const Tiles & tiles = board.getTiles();
		for ( auto tileColumn : tiles ) {
			for ( auto tileList : tileColumn ) {
				for ( auto tileOnBoard : tileList ) {
					int tile = tileOnBoard.first;
					if ( Tile::allTiles[tile].isWebbed() ) {
						continue;
					}
					const Tile::Abilities & mdfrs = Tile::allTiles[tile].getModifiers();
					for ( const Tile::Ability & mdfr : mdfrs ) {
						AbilityIdentifier curModIdent = mdfr.getIdentifier();
						if ( purgeTargets( theTarget, board, curModIdent ) ) {
							executeAbility( curModIdent, theTarget );
						}
					}
				}
			}
		}
	}

	std::vector< std::vector< AbilityIdentifier > > Game::prebattleTargetsRequired() {
		const Tiles & tiles = board.getTiles();
		std::vector< std::vector< AbilityIdentifier > > toRun;
		toRun.resize( getNumberOfPlayers() );
		for ( auto tileColumn : tiles ) {
			for ( auto tileList : tileColumn ) {
				for ( auto tileOnBoard : tileList ) {
					AbilityIdentifier	ai;
					ai.tile = tileOnBoard.first;
					if ( Tile::allTiles[ai.tile].isWebbed() ) {
						continue;
					}
					ai.group = AbilityGroup::BATTLE_START;
					int numOnBattleStart = Tile::allTiles[ai.tile].getOnBattleStart().size();
					int controller = Tile::allTiles[ai.tile].getController();
					for ( ai.id = 0; ai.id < numOnBattleStart; ai.id++ ) {
						toRun[controller].push_back( ai );
					}
				}
			}
		}
		return toRun;
	}

	std::vector< std::vector< AbilityIdentifier > > Game::battlePhaseTargetsRequired( int curInitiative ) {
		const std::list< int > tiles = board.getTilesWithInitiative( curInitiative );
		std::vector< std::vector< AbilityIdentifier > > toRun;
		toRun.resize( getNumberOfPlayers() );
		for ( int tile : tiles ) {
			if ( Tile::allTiles[tile].isWebbed() ) {
				continue;
			}
			AbilityIdentifier ai;
			ai.tile = tile;
			ai.group = AbilityGroup::ATTACK;
			int numAttacks = Tile::allTiles[tile].getAttacks().size();
			int controller = Tile::allTiles[tile].getController();
			for ( ai.id = 0; ai.id < numAttacks; ai.id++ ) {
				toRun[controller].push_back( ai );
			}
		}
		return toRun;
	}

	void Game::executeAbility( AbilityIdentifier & abilityIdentifier, Targets & targets ) {
		std::list< int > affectedTiles;
		Coordinates coords;
		Orientation orientation;
		for ( auto tpl : targets ) {
			affectedTiles.splice( affectedTiles.end(), tpl.tiles );
			coords = tpl.coords;
			orientation = tpl.orientation;
		}
		switch ( abilityIdentifier.group ) {
			case AbilityGroup::PLACING:
				Tile::allTiles[abilityIdentifier.tile].getPlacing().useAbility( affectedTiles );
				break;
			case AbilityGroup::BATTLE_START:
				Tile::allTiles[abilityIdentifier.tile].getOnBattleStart( abilityIdentifier.id ).useAbility( affectedTiles );
				break;
			case AbilityGroup::ATTACK:
				Tile::allTiles[abilityIdentifier.tile].getAttack( abilityIdentifier.id ).executeAttack( affectedTiles );
				break;
			case AbilityGroup::MODIFIER:
				Tile::allTiles[abilityIdentifier.tile].getModifier( abilityIdentifier.id ).modifyTiles( affectedTiles );
				break;
			case AbilityGroup::ACTIVE:
				if ( Tile::allTiles[abilityIdentifier.tile].getActiveAbility( abilityIdentifier.id ).useAbility( affectedTiles ) ) {
					removeFromBoard( abilityIdentifier.tile );
					placeOnBoard( abilityIdentifier.tile, coords, orientation );
				}
				break;
			case AbilityGroup::DEFENSIVE:
				Tile::allTiles[abilityIdentifier.tile].getDefensiveAbility( abilityIdentifier.id ).useAbility( affectedTiles );
				break;
		}
	}

	bool Game::fillFromDFStyle(utility::DFStyleReader & input) {
		while ( input.hasNextToken() ) {
			std::vector< std::string > info = input.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				return false;
			}
			std::string type = info[0];
			if ( type == "GAMEEND" ) {
				return true;
			} else if ( type == "BOARDBEGIN" ) {
				if ( !board.fillFromDFStyle(input) ) {
					return false;
				}
			} else if ( type == "CURRENTPLAYER" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				currentPlayer = std::stoi(info[1]);
				if ( currentPlayer < 0 || currentPlayer >= static_cast<int>( players.size() ) ) {
					return false;
				}
			} else if ( type == "NOARMY" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				if ( info[1] == "true" ) {
					noArmy = true;
				} else if ( info[1] == "false" ) {
					noArmy = false;
				} else {
					return false;
				}
			} else if ( type == "PLAYERBEGIN" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				int playerNum = std::stoi(info[1]);
				if ( playerNum < 0 || playerNum >= static_cast<int>( players.size() ) ) {
					return false;
				}
				if ( !players[playerNum].fillFromDFStyle(input) ) {
					return false;
				}
			} else if ( type == "TILEBEGIN" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				int tileNum = std::stoi(info[1]);
				if ( tileNum < 0 || tileNum >= static_cast<int>( Tile::allTiles.size() ) ) {
					return false;
				}
				if ( !Tile::allTiles[tileNum].fillFromDFStyle(input) ) {
					return false;
				}
			} else {
				return false;
			}
		}
		return false;
	}

	void Game::encodeAsDFStyle(utility::DFStyleCreator & output) const {
		output.startToken("BOARDBEGIN");
		output.endToken();
		board.encodeAsDFStyle(output);
		output.startToken("CURRENTPLAYER");
		output.addToToken(currentPlayer);
		output.endToken();
		output.startToken("NOARMY");
		output.addToToken(noArmy);
		output.endToken();
		for ( int i = 0; i < static_cast<int>( players.size() ); i++ ) {
			output.startToken("PLAYERBEGIN");
			output.addToToken(i);
			output.endToken();
			players[i].encodeAsDFStyle(output);
		}
		for ( int i = 0; i < static_cast<int>( Tile::allTiles.size() ); i++ ) {
			output.startToken("TILEBEGIN");
			output.addToToken(i);
			output.endToken();
			Tile::allTiles[i].encodeAsDFStyle(output);
		}
		output.startToken("GAMEEND");
		output.endToken();
	}

	Targets Game::generateTargets( const neuro::Board & board, neuro::AbilityIdentifier & abilityIdentifier ) {
		Targets result;
		return result;
	}

	bool Game::purgeTargets( Targets & targets, const neuro::Board & board, neuro::AbilityIdentifier identifier ) {
		//TODO: This is a stub.
		return true;
	}

	void Game::removeFromBoard( int tile ) {
		Tile::allTiles[tile].clearModifications();
		Tile::allTiles[tile].stopModifying();
		board.removeTile( tile );
	}

	void Game::placeOnBoard( int tile, Coordinates coords, Orientation orientation ) {
		board.placeTile( coords, orientation, tile );
		updateTile( coords, orientation, tile );
		useModifications( tile );
		if ( board.isFull() ) {
			Tile::battle = true;
		}
	}

	void Game::useModifications( int tile ) {
		Tile::Abilities mods = Tile::allTiles[tile].getModifiers();
		for ( const Tile::Ability & mod : mods ) {
			AbilityIdentifier ai = mod.getIdentifier();
			Targets trgts = generateTargets( board, ai );
			executeAbility( ai, trgts );
		}
	}

}
