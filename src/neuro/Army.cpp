#include"neuro/Army.hpp"

namespace neuro {

	Army::Army(std::vector<int> tiles, int owner) : owner(owner), tiles(tiles) {
		for ( int tileID : tiles ) {
			Tile::allTiles[tileID].setOwner( owner );
		}
	}

	void Army::shuffle() {
		std::shuffle(std::begin(tiles), std::end(tiles), std::default_random_engine());
	}

	int Army::drawTile() {
		int result = -1;
		if ( !isEmpty() ) {
			result = tiles.back();
			tiles.pop_back();
		}
		return result;
	}

	bool Army::fillFromDFStyle(utility::DFStyleReader & input) {
		tiles.clear();
		while ( input.hasNextToken() ) {
			std::vector< std::string > info = input.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				return false;
			}
			std::string type = info[0];
			if ( type == "ARMYEND" ) {
				return true;
			} else if ( type == "OWNER" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				owner = std::stoi(info[1]);
			} else if ( type == "TILES" ) {
				for ( int i = 1; i < static_cast<int>( info.size() ); i++ ) {
					tiles.push_back(std::stoi(info[i]));
				}
			} else {
				return false;
			}
		}
		return false;
	}

	void Army::encodeAsDFStyle(utility::DFStyleCreator & output) const {
		output.startToken("OWNER");
		output.addToToken(owner);
		output.endToken();
		output.startToken("TILES");
		for ( int tile : tiles ) {
			output.addToToken(tile);
		}
		output.endToken();
		output.startToken("ARMYEND");
		output.endToken();
	}

}
