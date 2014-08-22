#include"neuro/Player.hpp"

namespace neuro {

	Player::Player(int id, ArmyP army, int health) : id(id), army(army), health(health) {
		army->initialize(id);
	}

	void Player::drawTiles(int toDraw) {
		for ( int i = 0; i < toDraw; i++ ) {
			TileP drawn = army->drawTile();
			if ( drawn ) {
				hand.insertTile(drawn);
			} else {
				break;
			}
		}
		sigModified(*this);
	}

	void Player::startTurn(int & firstTurns) {
		int toDraw = 3 - getNumberOfTilesInHand();
		if ( firstTurns ) {
			toDraw -= firstTurns;
			firstTurns--;
		}
		drawTiles( toDraw );
		//Only require discarding a tile if the player had 3 in his hand.
		discardedTile = ( getNumberOfTilesInHand() < 3 );
	}

}
