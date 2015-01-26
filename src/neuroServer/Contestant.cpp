#include"neuroServer/Contestant.hpp"

namespace neuroServer {

	bool Move::fillFromDFStyle(utility::DFStyleReader & input) {
		while ( input.hasNextToken() ) {
			std::vector< std::string > info = input.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				return false;
			}
			std::string type = info[0];
			if ( type == "MOVEEND" ) {
				return true;
			} else if ( type == "END_TURN" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				if ( info[1] == "true" ) {
					endTurn = true;
				} else if ( info[1] == "false" ) {
					endTurn = false;
				} else {
					return false;
				}
			} else if ( type == "DISCARD" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				if ( info[1] == "true" ) {
					discard = true;
				} else if ( info[1] == "false" ) {
					discard = false;
				} else {
					return false;
				}
			} else if ( type == "ABILITYIDENTIFIERBEGIN" ) {
				if ( !abilityIdentifier.fillFromDFStyle(input) ) {
					return false;
				}
			} else {
				return false;
			}
		}
		return false;
	}

	void Move::encodeAsDFStyle(utility::DFStyleCreator & output) const {
		output.startToken("END_MOVE");
		output.addToToken(endTurn);
		output.endToken();
		output.startToken("DISCARD");
		output.addToToken(discard);
		output.endToken();
		output.startToken("ABILITYIDENTIFIERBEGIN");
		output.endToken();
		abilityIdentifier.encodeAsDFStyle(output);
		output.startToken("MOVEEND");
		output.endToken();
	}

	bool Target::fillFromDFStyle(utility::DFStyleReader & input) {
		tiles.clear();
		while ( input.hasNextToken() ) {
			std::vector< std::string > info = input.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				return false;
			}
			std::string type = info[0];
			if ( type == "TARGETEND" ) {
				return true;
			} else if ( type == "COORDINATES" ) {
				if (  static_cast<int>( info.size() ) < 3 ) {
					return false;
				}
				coords.first = std::stoi(info[1]);
				coords.second = std::stoi(info[2]);
			} else if ( type == "ORIENTATION" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					return false;
				}
				orientation = std::stoi(info[1]);
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

	void Target::encodeAsDFStyle(utility::DFStyleCreator & output) const {
		output.startToken("COORDINATES");
		output.addToToken(coords.first);
		output.addToToken(coords.second);
		output.endToken();
		output.startToken("ORIENTATION");
		output.addToToken(orientation);
		output.endToken();
		output.startToken("TILES");
		for ( const int & t : tiles ) {
			output.addToToken(t);
		}
		output.endToken();
		output.startToken("TARGETEND");
		output.endToken();
	}

}
