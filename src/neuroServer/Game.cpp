#include"neuroServer/Game.hpp"

namespace neuroServer {

	Game::Game(GameOptions options) :
		board(options.board),
		currentPlayer(0),
		noArmy(false) {
			neuro::Tile::allTiles.clear();
			for ( int i = 0; i < static_cast<int>( options.contestants.size() ); i++ ) {
				arbiters.emplace_back(options.contestants[i]);
			}
			for ( int i = 0; i < static_cast<int>( options.armies.size() ); i++ ) {
				std::vector< int > army;
				for ( neuro::Tile tile : options.armies[i] ) {
					int curTileId = neuro::Tile::allTiles.size();
					tile.setId(curTileId);
					army.push_back(curTileId);
					neuro::Tile::allTiles.push_back(tile);
				}
				players.emplace_back(i, army, options.initialHealth);
			}
		}

	int Game::getNumberOfLivingPlayers() const {
		int result = 0;
		for ( const neuro::Player & player : players ) {
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
							} else if ( move.abilityIdentifier.group == neuro::AbilityGroup::PLACING ) {
								tilePlacing( move.abilityIdentifier.tile );
							}
						} else {
							abilityUsing( move.abilityIdentifier );
						}
						resolveActivated();
					}
					if ( neuro::Tile::battle ) {
						runBattle();
						neuro::Tile::battle = false;
					}
				} while ( !move.endTurn );
			}
			currentPlayer++;
			currentPlayer	%= getNumberOfPlayers();
		}
		runBattle(); //The final battle.
	}

	void Game::updateTile( neuro::Coordinates coords, neuro::Orientation orientation, int tile ) {
		neuro::Tile::allTiles[tile].clearModifications();
		Targets theTarget;
		Target trgt;
		trgt.tiles.push_back( tile );
		trgt.coords = coords;
		trgt.orientation = orientation;
		theTarget.push_back( trgt );
		const neuro::Tiles & tiles = board.getTiles();
		for ( auto tileColumn : tiles ) {
			for ( auto tileList : tileColumn ) {
				for ( auto tileOnBoard : tileList ) {
					int tile = tileOnBoard.first;
					if ( neuro::Tile::allTiles[tile].isWebbed() ) {
						continue;
					}
					const neuro::Tile::Abilities & mdfrs = neuro::Tile::allTiles[tile].getModifiers();
					for ( const neuro::Tile::Ability & mdfr : mdfrs ) {
						neuro::AbilityIdentifier curModIdent = mdfr.getIdentifier();
						if ( Arbiter::purgeTargets( theTarget, board, curModIdent ) ) {
							executeAbility( curModIdent, theTarget );
						}
					}
				}
			}
		}
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
		for ( int i = 0; i < static_cast<int>( neuro::Tile::allTiles.size() ); i++ ) {
			output.startToken("TILEBEGIN");
			output.addToToken(i);
			output.endToken();
			neuro::Tile::allTiles[i].encodeAsDFStyle(output);
		}
		output.startToken("GAMEEND");
		output.endToken();
	}

	void Game::removeFromBoard( int tile ) {
		neuro::Tile::allTiles[tile].clearModifications();
		neuro::Tile::allTiles[tile].stopModifying();
		board.removeTile( tile );
	}

	void Game::placeOnBoard( int tile, neuro::Coordinates coords, neuro::Orientation orientation ) {
		board.placeTile( coords, orientation, tile );
		updateTile( coords, orientation, tile );
		useModifications( tile );
		while ( board.isFull() ) {
			runBattle();
		}
	}

	void Game::tilePlacing( int tile ) {
		int tilePlacer = neuro::Tile::allTiles[tile].getController();
		neuro::AbilityIdentifier ai;
		ai.tile = tile;
		ai.group = neuro::AbilityGroup::PLACING;
		ai.id = 0;
		Targets targets = arbiters[tilePlacer].getTargets( tilePlacer, players, board, noArmy, ai );
		std::list< int > affectedTiles;
		Target curTarget;
		for ( auto tpl : targets ) {
			curTarget = tpl;
			affectedTiles.splice( affectedTiles.end(), curTarget.tiles );
		}
		neuro::Tile::Ability & placing = neuro::Tile::allTiles[tile].getPlacing();
		if ( placing.placeTile( affectedTiles ) ) {
			placeOnBoard( tile, curTarget.coords, curTarget.orientation );
		}
	}

	void Game::executeAbility( neuro::AbilityIdentifier & abilityIdentifier, Targets & targets ) {
		std::list< int > affectedTiles;
		neuro::Coordinates coords;
		neuro::Orientation orientation;
		for ( auto tpl : targets ) {
			affectedTiles.splice( affectedTiles.end(), tpl.tiles );
			coords = tpl.coords;
			orientation = tpl.orientation;
		}
		switch ( abilityIdentifier.group ) {
			case neuro::AbilityGroup::PLACING:
				neuro::Tile::allTiles[abilityIdentifier.tile].getPlacing().useAbility( affectedTiles );
				break;
			case neuro::AbilityGroup::BATTLE_START:
				neuro::Tile::allTiles[abilityIdentifier.tile].getOnBattleStart( abilityIdentifier.id ).useAbility( affectedTiles );
				break;
			case neuro::AbilityGroup::ATTACK:
				neuro::Tile::allTiles[abilityIdentifier.tile].getAttack( abilityIdentifier.id ).executeAttack( affectedTiles );
				break;
			case neuro::AbilityGroup::MODIFIER:
				neuro::Tile::allTiles[abilityIdentifier.tile].getModifier( abilityIdentifier.id ).modifyTiles( affectedTiles );
				break;
			case neuro::AbilityGroup::ACTIVE:
				if ( neuro::Tile::allTiles[abilityIdentifier.tile].getActiveAbility( abilityIdentifier.id ).useAbility( affectedTiles ) ) {
					removeFromBoard( abilityIdentifier.tile );
					placeOnBoard( abilityIdentifier.tile, coords, orientation );
				}
				break;
			case neuro::AbilityGroup::DEFENSIVE:
				neuro::Tile::allTiles[abilityIdentifier.tile].getDefensiveAbility( abilityIdentifier.id ).useAbility( affectedTiles );
				break;
		}
	}

	void Game::abilityUsing( neuro::AbilityIdentifier & abilityIdentifier ) {
		int abilityUser = neuro::Tile::allTiles[abilityIdentifier.tile].getController();
		Targets targets = arbiters[abilityUser].getTargets( abilityUser, players, board, noArmy, abilityIdentifier );
		executeAbility( abilityIdentifier, targets );
	}

	void Game::useModifications( int tile ) {
		neuro::Tile::Abilities mods = neuro::Tile::allTiles[tile].getModifiers();
		for ( const neuro::Tile::Ability & mod : mods ) {
			neuro::AbilityIdentifier ai = mod.getIdentifier();
			Targets trgts = Arbiter::generateTargets( board, ai );
			executeAbility( ai, trgts );
		}
	}

	void Game::clearDead() {
		const neuro::Tiles & tiles = board.getTiles();
		for ( auto tileColumn : tiles ) {
			for ( auto tileList : tileColumn ) {
				for ( auto tileOnBoard : tileList ) {
					if ( !neuro::Tile::allTiles[tileOnBoard.first].isAlive() ) {
						removeFromBoard( tileOnBoard.first );
					}
				}
			}
		}
	}

	void Game::battleStart() {
		const neuro::Tiles & tiles = board.getTiles();
		std::vector< std::vector< neuro::AbilityIdentifier > > toRun;
		toRun.resize( getNumberOfPlayers() );
		for ( auto tileColumn : tiles ) {
			for ( auto tileList : tileColumn ) {
				for ( auto tileOnBoard : tileList ) {
					neuro::AbilityIdentifier	ai;
					ai.tile = tileOnBoard.first;
					if ( neuro::Tile::allTiles[ai.tile].isWebbed() ) {
						continue;
					}
					ai.group = neuro::AbilityGroup::BATTLE_START;
					int numOnBattleStart = neuro::Tile::allTiles[ai.tile].getOnBattleStart().size();
					int controller = neuro::Tile::allTiles[ai.tile].getController();
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
		std::vector< std::vector< neuro::AbilityIdentifier > > toRun;
		toRun.resize( getNumberOfPlayers() );
		for ( int tile : tiles ) {
			if ( neuro::Tile::allTiles[tile].isWebbed() ) {
				continue;
			}
			neuro::AbilityIdentifier ai;
			ai.tile = tile;
			ai.group = neuro::AbilityGroup::ATTACK;
			int numAttacks = neuro::Tile::allTiles[tile].getAttacks().size();
			int controller = neuro::Tile::allTiles[tile].getController();
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
			neuro::AbilityIdentifier ai = neuro::Tile::allTiles[tile].getActivatedAbility();
			abilityUsing( ai );
			tile = board.getActivatedTile();
		}
	}

}
