#include"neuro/Game.hpp"

namespace neuro {

	Game::Game(GameOptions options) :
		board(options.board),
		currentPlayer(0),
		noArmy(false) {
			for ( int i = 0; i < static_cast<int>( options.contestants.size() ); i++ ) {
				arbiters.emplace_back( options.contestants[i] );
			}
			for ( int i = 0; i < static_cast<int>( options.armies.size() ); i++ ) {
				players.push_back( PlayerP( new Player(i, options.armies[i], options.initialHealth) ) );
			}
		}

	int Game::getNumberOfLivingPlayers() const {
		int result = 0;
		for ( const PlayerP & player : players ) {
			if ( player->getHealth() > 0 ) {
				result++;
			}
		}
		return result;
	}

	bool Game::isFinished() const {
		if ( (noArmy && players[currentPlayer]->getArmy().isEmpty()) || 
				( getNumberOfLivingPlayers() <= 1 ) ) {
			return true;
		}
		return false;
	}

	void Game::play() {
		int firstTurns = 2; //How many turns drawing less tiles.
		while ( !isFinished() ) {
			if ( players[currentPlayer]->getHealth() > 0 ) {
				players[currentPlayer]->startTurn( firstTurns );
				noArmy = noArmy || players[currentPlayer]->armyEmpty();
				Move move;
				do {
					move = arbiters[currentPlayer].getMove( currentPlayer, players, board, noArmy );
					if ( !move.endTurn ) {
						if ( players[currentPlayer]->getHand().containsTile( move.abilityIdentifier.tile ) ) {
							if ( move.discard ) {
								players[currentPlayer]->discardTile( move.abilityIdentifier.tile );
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

	void Game::updateTile( Coordinates coords, Orientation orientation, TileP tile ) {
		tile->clearModifications();
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
					TileP tile = tileOnBoard.first;
					if ( tile->isWebbed() ) {
						continue;
					}
					const Tile::Abilities & mdfrs = tile->getModifiers();
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

	void Game::removeFromBoard( TileP tile ) {
		tile->clearModifications();
		tile->stopModifying();
		board.removeTile( tile );
	}

	void Game::placeOnBoard( TileP tile, Coordinates coords, Orientation orientation ) {
		board.placeTile( coords, orientation, tile );
		updateTile( coords, orientation, tile );
		useModifications( tile );
		while ( board.isFull() ) {
			runBattle();
		}
	}

	void Game::tilePlacing( TileP tile ) {
		int tilePlacer = tile->getController();
		AbilityIdentifier ai;
		ai.tile = tile;
		ai.group = AbilityGroup::PLACING;
		ai.id = 0;
		Targets targets = arbiters[tilePlacer].getTargets( tilePlacer, players, board, noArmy, ai );
		std::list< TileP > affectedTiles;
		Target curTarget;
		for ( auto tpl : targets ) {
			curTarget = tpl;
			affectedTiles.splice( affectedTiles.end(), curTarget.tiles );
		}
		Tile::Ability & placing = tile->getPlacing();
		if ( placing.placeTile( affectedTiles ) ) {
			placeOnBoard( tile, curTarget.coords, curTarget.orientation );
		}
	}

	void Game::executeAbility( AbilityIdentifier & abilityIdentifier, Targets & targets ) {
		std::list< TileP > affectedTiles;
		Coordinates coords;
		Orientation orientation;
		for ( auto tpl : targets ) {
			affectedTiles.splice( affectedTiles.end(), tpl.tiles );
			coords = tpl.coords;
			orientation = tpl.orientation;
		}
		switch ( abilityIdentifier.group ) {
			case AbilityGroup::PLACING:
				abilityIdentifier.tile->getPlacing().useAbility( affectedTiles );
				break;
			case AbilityGroup::BATTLE_START:
				abilityIdentifier.tile->getOnBattleStart( abilityIdentifier.id ).useAbility( affectedTiles );
				break;
			case AbilityGroup::ATTACK:
				abilityIdentifier.tile->getAttack( abilityIdentifier.id ).executeAttack( affectedTiles );
				break;
			case AbilityGroup::MODIFIER:
				abilityIdentifier.tile->getModifier( abilityIdentifier.id ).modifyTiles( affectedTiles );
				break;
			case AbilityGroup::ACTIVE:
				if ( abilityIdentifier.tile->getActiveAbility( abilityIdentifier.id ).useAbility( affectedTiles ) ) {
					removeFromBoard( abilityIdentifier.tile );
					placeOnBoard( abilityIdentifier.tile, coords, orientation );
				}
				break;
			case AbilityGroup::DEFENSIVE:
				abilityIdentifier.tile->getDefensiveAbility( abilityIdentifier.id ).useAbility( affectedTiles );
				break;
		}
	}

	void Game::abilityUsing( AbilityIdentifier & abilityIdentifier ) {
		int abilityUser = abilityIdentifier.tile->getController();
		Targets targets = arbiters[abilityUser].getTargets( abilityUser, players, board, noArmy, abilityIdentifier );
		executeAbility( abilityIdentifier, targets );
	}

	void Game::useModifications( TileP tile ) {
		Tile::Abilities mods = tile->getModifiers();
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
					if ( !tileOnBoard.first->isAlive() ) {
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
					if ( ai.tile->isWebbed() ) {
						continue;
					}
					ai.group = AbilityGroup::BATTLE_START;
					int numOnBattleStart = ai.tile->getOnBattleStart().size();
					int controller = ai.tile->getController();
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
		const std::list< TileP > tiles = board.getTilesWithInitiative( curInitiative );
		std::vector< std::vector< AbilityIdentifier > > toRun;
		toRun.resize( getNumberOfPlayers() );
		for ( TileP tile : tiles ) {
			if ( tile->isWebbed() ) {
				continue;
			}
			AbilityIdentifier ai;
			ai.tile = tile;
			ai.group = AbilityGroup::ATTACK;
			int numAttacks = tile->getAttacks().size();
			int controller = tile->getController();
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
		if ( !players[currentPlayer]->hasDiscardedTile() ) {
			TileP toDiscard = arbiters[currentPlayer].requestDiscard( currentPlayer, players, board, noArmy );
			players[currentPlayer]->discardTile( toDiscard );
		}
		battleStart();
		int curInitiative = board.getMaxInitiative();
		while ( curInitiative >= 0 ) {
			battlePhase( curInitiative );
			curInitiative--;
		}
	}

	void Game::resolveActivated() {
		TileP tile = board.getActivatedTile();
		while ( tile ) {
			AbilityIdentifier ai = tile->getActivatedAbility();
			abilityUsing( ai );
			tile = board.getActivatedTile();
		}
	}

}
