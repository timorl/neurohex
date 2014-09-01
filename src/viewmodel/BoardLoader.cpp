#include<iostream>
#include"viewmodel/BoardLoader.hpp"

namespace viewmodel {

	const std::string unrecognizedTokenMessage = "Ignoring unrecognized token: ";
	const std::string emptyTokenMessage = "Empty token.";
	const std::string tooFewArgumentsMessage = "Too few arguments for token: ";
	const std::string missingInformationMessage = "Missing information.";

	void BoardLoader::fillInBoardFields( neuro::BoardDescription & boardFields, int x, int y, std::string flds ) {
		boardFields.resize(x);
		for ( int i = 0; i < x; i++ ) {
			boardFields[i].resize(y);
		}
		auto curChar = flds.begin();
		for ( int i = 0; i < y; i++ ) {
			for ( int j = 0; j < x; j++ ) {
				while ( *curChar == ' ' || *curChar == '\n' || *curChar == '\t' ) {
					curChar++;
				}
				switch ( *curChar ) {
					case '0':
						boardFields[j][i] = neuro::FieldType::NO_FIELD;
						break;
					case '1':
						boardFields[j][i] = neuro::FieldType::NORMAL;
						break;
				}
				curChar++;
			}
		}
	}

	bool parseBoard( utility::DFStyleParser & parser, std::string & boardName, std::string & boardDescription, neuro::BoardDescription & boardFields ) {
		const std::string abortMessage = "Aborting board load: ";
		int boardX, boardY;
		std::string boardFieldsS;
		while ( parser.hasNextToken() ) {
			std::vector< std::string > info = parser.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				std::cerr << abortMessage << emptyTokenMessage << std::endl;
				return false;
			}
			std::string type = info[0];
			if ( type == "NAME" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				boardName = info[1];
			} else if ( type == "DESCRIPTION" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				boardDescription	= info[1];
			} else if ( type == "DIMENSIONS" ) {
				if (  static_cast<int>( info.size() ) < 3 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				boardX = std::stoi( info[1] );
				boardY = std::stoi( info[2] );
			} else if ( type == "MAP" ) {
				if (  static_cast<int>( info.size() ) < 2 ) {
					std::cerr << abortMessage << tooFewArgumentsMessage << type << std::endl;
					return false;
				}
				boardFieldsS	= info[1];
			} else {
				std::cerr << unrecognizedTokenMessage << type << std::endl;
			}
		}
		if ( boardName.empty() || boardDescription.empty() || boardFieldsS.empty() || boardX == 0 || boardY == 0 ) {
			std::cerr << abortMessage << missingInformationMessage << std::endl;
			return false;
		}
		BoardLoader::fillInBoardFields( boardFields, boardX, boardY, boardFieldsS );
		return true;
	}

	void BoardLoader::loadBoards(std::string directory) {
		utility::DFStyleParser parser(directory);
		int boardCount = 0;
		while ( parser.nextFile() ) {
			std::string boardName;
			std::string boardDescription;
			neuro::BoardDescription boardFields;
			if ( parseBoard(parser, boardName, boardDescription, boardFields) ) {
				boardCount++;
				addBoard( boardName, boardDescription, boardFields );
			}
		}
		if ( boardCount == 0 ) {
			std::clog << "No boards in directory " << directory << std::endl;
		} else {
			std::clog << "Loaded " << boardCount << " new board" << ( (boardCount > 1)?"s":"" ) << " from directory " << directory << std::endl;
		}
	}

	void BoardLoader::addBoard(std::string name, std::string description, neuro::BoardDescription board) {
		if ( boards.count( name ) == 0 ) {
			boards[name] = board;
			descriptions[name] = description;
		}
	}

	neuro::BoardDescription	BoardLoader::getBoard(std::string name) const {
		return boards.at(name);
	}

}
