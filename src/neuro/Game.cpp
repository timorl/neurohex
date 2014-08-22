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

	void Game::abilityUsing( TileP tile, AbilityGroup abilityGroup, int abilityId ) {
	}

}
