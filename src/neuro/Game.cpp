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
						if ( players[currentPlayer]->getHand().containsTile( move.tile ) ) {
							if ( move.discard ) {
								players[currentPlayer]->discardTile( move.tile );
							} else if ( move.abilityGroup == AbilityGroup::PLACING ) {
								tilePlacing( move.tile );
							}
						} else {
							abilityUsing( move.tile, move.abilityGroup, move.abilityId );
						}
					}
				} while ( !move.endTurn );
			}
			currentPlayer++;
			currentPlayer	%= getNumberOfPlayers();
		}
		runBattle(); //The final battle.
	}

	void Game::tilePlacing( TileP tile ) {
		int tilePlacer = tile->getController();
		Targets targets = arbiters[tilePlacer].getTargets( tilePlacer, players, board, noArmy, *tile, AbilityGroup::PLACING, 0 );
		std::list< TileP > affectedTiles;
		Coordinates coords;
		Orientation orientation;
		for ( auto tpl : targets ) {
			std::list< TileP > tls;
			std::tie( coords, orientation, tls ) = tpl;
			affectedTiles.splice( affectedTiles.end(), tls );
		}
		Tile::Placing & placing = tile->getPlacing();
		if ( placing.placeTile( affectedTiles ) ) {
			board.placeTile( coords, orientation, tile );
		}
		//TODO: Check for the possibility of battles.
	}

	void Game::executeAbility( TileP tile, AbilityGroup abilityGroup, int abilityId, Targets & targets ) {
		std::list< TileP > affectedTiles;
		Coordinates coords;
		Orientation orientation;
		for ( auto tpl : targets ) {
			std::list< TileP > tls;
			std::tie( coords, orientation, tls ) = tpl;
			affectedTiles.splice( affectedTiles.end(), tls );
		}
		switch ( abilityGroup ) {
			case AbilityGroup::PLACING:
				tile->getPlacing().placeTile( affectedTiles );
				break;
			case AbilityGroup::BATTLE_START:
				tile->getOnBattleStart( abilityId ).useAbility( affectedTiles );
				break;
			case AbilityGroup::ATTACK:
				tile->getAttack( abilityId ).executeAttack( affectedTiles );
				break;
			case AbilityGroup::MODIFIER:
				tile->getModifier( abilityId ).modifyTiles( affectedTiles );
				break;
			case AbilityGroup::ACTIVE:
				tile->getActiveAbility( abilityId ).useAbility( affectedTiles );
				break;
			case AbilityGroup::DEFENSIVE:
				tile->getDefensiveAbility( abilityId ).useAbility( affectedTiles );
				break;
		}
	}

	void Game::abilityUsing( TileP tile, AbilityGroup abilityGroup, int abilityId ) {
		int abilityUser = tile->getController();
		Targets targets = arbiters[abilityUser].getTargets( abilityUser, players, board, noArmy, *tile, abilityGroup, abilityId );
		executeAbility( tile, abilityGroup, abilityId, targets );
	}

	void Game::battleStart() {
		const Tiles & tiles = board.getTiles();
		std::vector< std::vector< std::tuple< TileP, AbilityGroup, int > > > toRun;
		toRun.resize( getNumberOfPlayers() );
		for ( auto tileColumn : tiles ) {
			for ( auto tileList : tileColumn ) {
				for ( auto tileOnBoard : tileList ) {
					TileP tile = tileOnBoard.first;
					int numOnBattleStart = tile->getOnBattleStart().size();
					int controller = tile->getController();
					for ( int i = 0; i < numOnBattleStart; i++ ) {
						toRun[controller].push_back( std::make_tuple( tile, AbilityGroup::BATTLE_START, i ) );
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
				TileP tile;
				AbilityGroup	abilityGroup;
				int abilityId;
				std::tie( tile, abilityGroup, abilityId ) = toRun[i][j];
				executeAbility( tile, abilityGroup, abilityId, targets );
			}
		}
	}

	void Game::battlePhase( int curInitiative ) {
		const std::list< TileP > tiles = board.getTilesWithInitiative( curInitiative );
		std::vector< std::vector< std::tuple< TileP, AbilityGroup, int > > > toRun;
		toRun.resize( getNumberOfPlayers() );
		for ( TileP tile : tiles ) {
			int numAttacks = tile->getAttacks().size();
			int controller = tile->getController();
			for ( int i = 0; i < numAttacks; i++ ) {
				toRun[controller].push_back( std::make_tuple( tile, AbilityGroup::ATTACK, i ) );
			}
		}
		for ( int i = 0; i < getNumberOfPlayers(); i++ ) {
			arbiters[i].requestTargetsForAbilities( i, players, board, noArmy, toRun[i] );
		}
		for ( int i = 0; i < getNumberOfPlayers(); i++ ) {
			for ( int j = 0; j < static_cast<int>( toRun[i].size() ); j++ ) {
				Targets targets = arbiters[i].getTargetsForAbility( j );
				TileP tile;
				AbilityGroup	abilityGroup;
				int abilityId;
				std::tie( tile, abilityGroup, abilityId ) = toRun[i][j];
				executeAbility( tile, abilityGroup, abilityId, targets );
			}
		}
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

}
