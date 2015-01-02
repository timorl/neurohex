#include"neuro/Game.hpp"

namespace neuro {

	Game::Game(GameOptions options) :
		board(options.board),
		currentPlayer(0),
		noArmy(false) {
			Tile::allTiles.clear();
			for ( int i = 0; i < static_cast<int>( options.contestants.size() ); i++ ) {
				arbiters.emplace_back(options.contestants[i]);
			}
			for ( int i = 0; i < static_cast<int>( options.armies.size() ); i++ ) {
				std::vector< int > army;
				for ( Tile tile : options.armies[i] ) {
					int curTileId = Tile::allTiles.size();
					tile.setId(curTileId);
					army.push_back(curTileId);
					Tile::allTiles.push_back(tile);
				}
				players.emplace_back(i, army, options.initialHealth);
			}
		}

	int Game::getNumberOfLivingPlayers() const {
		int result = 0;
		for ( const Player & player : players ) {
			if ( player.getHealth() > 0 ) {
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

	void Game::play() {
		int firstTurns = 2; //How many turns drawing less tiles.
		while ( !isFinished() ) {
			if ( players[currentPlayer].getHealth() > 0 ) {
				players[currentPlayer].startTurn( firstTurns );
				noArmy = noArmy || players[currentPlayer].armyEmpty();
				Move move;
				do {
					move = arbiters[currentPlayer].getMove( currentPlayer, players, board, noArmy );
					if ( !move.endTurn ) {
						if ( players[currentPlayer].getHand().containsTile( move.abilityIdentifier.tile ) ) {
							if ( move.discard ) {
								players[currentPlayer].discardTile( move.abilityIdentifier.tile );
							} else if ( move.abilityIdentifier.group == AbilityGroup::PLACING ) {
								tilePlacing( move.abilityIdentifier.tile );
							}
						} else {
							abilityUsing( move.abilityIdentifier );
						}
						resolveActivated();
					}
					if ( Tile::battle ) {
						runBattle();
						Tile::battle = false;
					}
				} while ( !move.endTurn );
			}
			currentPlayer++;
			currentPlayer	%= getNumberOfPlayers();
		}
		runBattle(); //The final battle.
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
						if ( Arbiter::purgeTargets( theTarget, board, curModIdent ) ) {
							executeAbility( curModIdent, theTarget );
						}
					}
				}
			}
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

	void Game::encodeAsDFStyle(utility::DFStyleCreator & output) {
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

	void Game::removeFromBoard( int tile ) {
		Tile::allTiles[tile].clearModifications();
		Tile::allTiles[tile].stopModifying();
		board.removeTile( tile );
	}

	void Game::placeOnBoard( int tile, Coordinates coords, Orientation orientation ) {
		board.placeTile( coords, orientation, tile );
		updateTile( coords, orientation, tile );
		useModifications( tile );
		while ( board.isFull() ) {
			runBattle();
		}
	}

	void Game::tilePlacing( int tile ) {
		int tilePlacer = Tile::allTiles[tile].getController();
		AbilityIdentifier ai;
		ai.tile = tile;
		ai.group = AbilityGroup::PLACING;
		ai.id = 0;
		Targets targets = arbiters[tilePlacer].getTargets( tilePlacer, players, board, noArmy, ai );
		std::list< int > affectedTiles;
		Target curTarget;
		for ( auto tpl : targets ) {
			curTarget = tpl;
			affectedTiles.splice( affectedTiles.end(), curTarget.tiles );
		}
		Tile::Ability & placing = Tile::allTiles[tile].getPlacing();
		if ( placing.placeTile( affectedTiles ) ) {
			placeOnBoard( tile, curTarget.coords, curTarget.orientation );
		}
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

	void Game::abilityUsing( AbilityIdentifier & abilityIdentifier ) {
		int abilityUser = Tile::allTiles[abilityIdentifier.tile].getController();
		Targets targets = arbiters[abilityUser].getTargets( abilityUser, players, board, noArmy, abilityIdentifier );
		executeAbility( abilityIdentifier, targets );
	}

	void Game::useModifications( int tile ) {
		Tile::Abilities mods = Tile::allTiles[tile].getModifiers();
		for ( const Tile::Ability & mod : mods ) {
			AbilityIdentifier ai = mod.getIdentifier();
			Targets trgts = Arbiter::generateTargets( board, ai );
			executeAbility( ai, trgts );
		}
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

	void Game::battleStart() {
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
		for ( int i = 0; i < getNumberOfPlayers(); i++ ) {
			arbiters[i].requestTargetsForAbilities( i, players, board, noArmy, toRun[i] );
		}
		for ( int i = 0; i < getNumberOfPlayers(); i++ ) {
			for ( int j = 0; j < static_cast<int>( toRun[i].size() ); j++ ) {
				Targets targets = arbiters[i].getTargetsForAbility( j );
				executeAbility( toRun[i][j], targets );
			}
		}
		clearDead();
	}

	void Game::battlePhase( int curInitiative ) {
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
		for ( int i = 0; i < getNumberOfPlayers(); i++ ) {
			arbiters[i].requestTargetsForAbilities( i, players, board, noArmy, toRun[i] );
		}
		for ( int i = 0; i < getNumberOfPlayers(); i++ ) {
			for ( int j = 0; j < static_cast<int>( toRun[i].size() ); j++ ) {
				Targets targets = arbiters[i].getTargetsForAbility( j );
				executeAbility( toRun[i][j], targets );
			}
		}
		clearDead();
	}

	void Game::runBattle() {
		if ( !players[currentPlayer].hasDiscardedTile() ) {
			int toDiscard = arbiters[currentPlayer].requestDiscard( currentPlayer, players, board, noArmy );
			players[currentPlayer].discardTile( toDiscard );
		}
		battleStart();
		int curInitiative = board.getMaxInitiative();
		while ( curInitiative >= 0 ) {
			battlePhase( curInitiative );
			curInitiative--;
		}
	}

	void Game::resolveActivated() {
		int tile = board.getActivatedTile();
		while ( tile != -1 ) {
			AbilityIdentifier ai = Tile::allTiles[tile].getActivatedAbility();
			abilityUsing( ai );
			tile = board.getActivatedTile();
		}
	}

}
