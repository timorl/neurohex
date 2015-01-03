#include"neuroClient/Game.hpp"

namespace neuroClient {

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
				if ( tileNum < 0 || tileNum >= static_cast<int>( neuro::Tile::allTiles.size() ) ) {
					return false;
				}
				if ( !neuro::Tile::allTiles[tileNum].fillFromDFStyle(input) ) {
					return false;
				}
			} else {
				return false;
			}
		}
		return false;
	}

}
