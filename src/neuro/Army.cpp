#include"neuro/Army.hpp"

namespace neuro {

	Army::Army( const Army & other ) {
		for ( auto tile : other.tiles ) {
			tiles.emplace_back( new Tile( *tile ) );
		}
	}

	void Army::shuffle() {
		std::shuffle(std::begin(tiles), std::end(tiles), std::default_random_engine());
	}

	void Army::initialize(int player) {
		owner = player;
		for ( TileP tile : tiles ) {
			tile->setOwner( player );
			tile->setParents( tile );
		}
		sigModified(*this);
	}

	TileP Army::drawTile() {
		TileP result;
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
			} else if ( type == "TILEBEGIN" ) {
				TileP tile = Tile::getDummy();
				if ( !tile->fillFromDFStyle(input) ) {
					return false;
				}
				tile->setParents(tile);
				tiles.push_back(tile);
			} else {
				return false;
			}
		}
		return false;
	}

	void Army::encodeAsDFStyle(utility::DFStyleCreator & output) {
		output.startToken("OWNER");
		output.addToToken(owner);
		output.endToken();
		for ( TileP tile : tiles ) {
			output.startToken("TILEBEGIN");
			output.endToken();
			tile->encodeAsDFStyle(output);
		}
		output.startToken("ARMYEND");
		output.endToken();
	}

}
