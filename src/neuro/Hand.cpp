#include"neuro/Hand.hpp"

namespace neuro {

	bool Hand::containsTile( TileP tile ) const {
		for ( TileP tp : tiles ) {
			if ( tp == tile ) {
				return true;
			}
		}
		return false;
	}

	bool Hand::fillFromDFStyle(utility::DFStyleReader & input) {
		tiles.clear();
		while ( input.hasNextToken() ) {
			std::vector< std::string > info = input.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				return false;
			}
			std::string type = info[0];
			if ( type == "HANDEND" ) {
				return true;
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

	void Hand::encodeAsDFStyle(utility::DFStyleCreator & output) {
		for ( TileP & tile : tiles ) {
			output.startToken("TILEBEGIN");
			output.endToken();
			tile->encodeAsDFStyle(output);
		}
		output.startToken("HANDEND");
		output.endToken();
	}

}
