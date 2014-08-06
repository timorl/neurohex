#include"viewmodel/BoardLoader.hpp"

namespace viewmodel {

	void BoardLoader::fillInBoardFields( neuro::BoardDescription & boardFields, int x, int y, std::string flds ) {
		boardFields.resize(x);
		for ( int i = 0; i < x; i++ ) {
			boardFields[i].resize(y);
		}
		auto curChar = flds.begin();
		for ( int i = 0; i < x; i++ ) {
			for ( int j = 0; j < y; j++ ) {
				while ( *curChar == ' ' || *curChar == '\n' ) {
					curChar++;
				}
				switch ( *curChar ) {
					case '0':
						boardFields[i][j] = neuro::FieldType::NO_FIELD;
						break;
					case '1':
						boardFields[i][j] = neuro::FieldType::NORMAL;
						break;
				}
				curChar++;
			}
		}
	}

	void BoardLoader::loadBoards(std::string directory) {
		utility::DFStyleParser parser(directory);
		while ( parser.nextFile() ) {
			std::string boardName;
			std::string boardDescription;
			int boardX, boardY;
			std::string boardFieldsS;
			neuro::BoardDescription boardFields;
			while ( parser.hasNextToken() ) {
				std::vector< std::string > boardInfo = parser.getNextToken();
				std::string type = boardInfo[0];
				if ( type == "NAME" ) {
					boardName = boardInfo[1];
				} else if ( type == "DESCRIPTION" ) {
					boardDescription	= boardInfo[1];
				} else if ( type == "DIMENSIONS" ) {
					boardX = std::stoi( boardInfo[1] );
					boardY = std::stoi( boardInfo[2] );
				} else if ( type == "MAP" ) {
					boardFieldsS	= boardInfo[1];
				}
			}
			fillInBoardFields( boardFields, boardX, boardY, boardFieldsS );
			addBoard( boardName, boardDescription, boardFields );
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
