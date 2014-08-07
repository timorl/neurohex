#include"neuro/Game.hpp"

namespace neuro {

	Game::Game(GameOptions options) :
		board(options.board),
		currentPlayer(0),
		noArmy(false),
		finished(false) {
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

	void Game::play() {
		while ( !isFinished() ) {
			if ( players[currentPlayer]->getHealth() > 0 ) {
				players[currentPlayer]->drawTiles( 3 - players[currentPlayer]->getNumberOfTilesInHand() );
				//Only require discarding a tile if the player had 3 in his hand.
				bool discardedTile = ( players[currentPlayer]->getNumberOfTilesInHand() < 3 );
				noArmy = noArmy || players[currentPlayer]->armyEmpty();
				Move move;
				do {
					move = arbiters[currentPlayer].getMove( currentPlayer, players, board, noArmy );
					if ( !move.endTurn ) {
						if ( players[currentPlayer]->getHand().containsTile( move.tile ) ) {
							if ( move.discard ) {
								players[currentPlayer]->removeTile( move.tile );
								discardedTile = true;
							} else {
								//FIXME: This just places the tile at the first coordinate given.
								board.placeTile( move.coords.front(), make_pair( move.tile, move.orientation ) );
								players[currentPlayer]->removeTile( move.tile );
							}
						} else { //Assume the tile is on the board.
							Coordinates coord = board.findTile( move.tile );
							if ( coord.first != -1 ) {
								//TODO: Actually moving or otherwise using the tile.
							}
						}
					}
				} while ( !discardedTile || !move.endTurn );
			}
			currentPlayer++;
			currentPlayer	%= getNumberOfPlayers();
			if ( (noArmy && players[currentPlayer]->getArmy().isEmpty()) || 
				( getNumberOfLivingPlayers() <= 1 ) ) {
				finished = true;
			}
		}
	}

}
