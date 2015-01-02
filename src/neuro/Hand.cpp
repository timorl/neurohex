#include"neuro/Hand.hpp"

namespace neuro {

	bool Hand::containsTile( int tile ) const {
		for ( int tp : tiles ) {
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

	void Hand::encodeAsDFStyle(utility::DFStyleCreator & output) {
		output.startToken("TILES");
		for ( int tile : tiles ) {
			output.addToToken(tile);
		}
		output.endToken();
		output.startToken("HANDEND");
		output.endToken();
	}

}
