#include"neuro/Player.hpp"

namespace neuro {

	void Player::drawTiles(int toDraw) {
		for ( int i = 0; i < toDraw; i++ ) {
			int drawn = army.drawTile();
			if ( drawn != -1 ) {
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

	bool Player::fillFromDFStyle(utility::DFStyleReader & input) {
		while ( input.hasNextToken() ) {
			std::vector< std::string > info = input.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				return false;
			}
			std::string type = info[0];
			if ( type == "PLAYEREND" ) {
				return true;
			} else if ( type == "ID" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				id = std::stoi(info[1]);
			} else if ( type == "HANDBEGIN" ) {
				if ( !hand.fillFromDFStyle(input) ) {
					return false;
				}
			} else if ( type == "ARMYBEGIN" ) {
				if ( !army.fillFromDFStyle(input) ) {
					return false;
				}
			} else if ( type == "HEALTH" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				health = std::stoi(info[1]);
			} else if ( type == "DISCARDEDTILE" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				if ( info[1] == "true" ) {
					discardedTile = true;
				} else if ( info[1] == "false" ) {
					discardedTile = false;
				} else {
					return false;
				}
			} else {
				return false;
			}
		}
		return false;
	}

	void Player::encodeAsDFStyle(utility::DFStyleCreator & output) const {
		output.startToken("ID");
		output.addToToken(id);
		output.endToken();
		output.startToken("HANDBEGIN");
		output.endToken();
		hand.encodeAsDFStyle(output);
		output.startToken("ARMYBEGIN");
		output.endToken();
		army.encodeAsDFStyle(output);
		output.startToken("HEALTH");
		output.addToToken(health);
		output.endToken();
		output.startToken("DISCARDEDTILE");
		output.addToToken(discardedTile);
		output.endToken();
		output.startToken("PLAYEREND");
		output.endToken();
	}

    void Player::printPlayer() const {
        std::cout << "Players id: " << id << std::endl;
        std::cout << "Players hand:\n";
        hand.printHand();
        //TODO print army
        std::cout << "Players health: " << health << std::endl;
        std::cout << "Discarded Tile = " << (discardedTile ? "true" : "false") << std::endl; 
    }

}
