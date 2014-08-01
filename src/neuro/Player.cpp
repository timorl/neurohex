#include"neuro/Player.hpp"

namespace neuro {

	Player::Player(int id, ArmyP army, int health) : id(id), army(army), health(health) {
		army->setOwner(id);
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

}
