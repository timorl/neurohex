#include"neuro/BoardLoader.hpp"

namespace neuro {

	void BoardLoader::loadBoards(std::string directory) {
		//TODO: Actually load something.
	}

	void BoardLoader::addBoard(std::string name, std::string description, BoardDescription board) {
		if ( boards.count( name ) == 0 ) {
			boards[name] = board;
			descriptions[name] = description;
		}
	}

	BoardDescription	BoardLoader::getBoard(std::string name) const {
		//FIXME: This just returns a default board always.
		BoardDescription	board;
		board.resize(5);
		for ( int i = 0; i < 5; i++ ) {
			board[i].resize(5);
		}
		board[0][0] = FieldType::NO_FIELD;
		board[0][1] = FieldType::NORMAL;
		board[0][2] = FieldType::NORMAL;
		board[0][3] = FieldType::NORMAL;
		board[0][4] = FieldType::NO_FIELD;
		board[1][0] = FieldType::NORMAL;
		board[1][1] = FieldType::NORMAL;
		board[1][2] = FieldType::NORMAL;
		board[1][3] = FieldType::NORMAL;
		board[1][4] = FieldType::NO_FIELD;
		board[2][0] = FieldType::NORMAL;
		board[2][1] = FieldType::NORMAL;
		board[2][2] = FieldType::NORMAL;
		board[2][3] = FieldType::NORMAL;
		board[2][4] = FieldType::NORMAL;
		board[3][0] = FieldType::NORMAL;
		board[3][1] = FieldType::NORMAL;
		board[3][2] = FieldType::NORMAL;
		board[3][3] = FieldType::NORMAL;
		board[3][4] = FieldType::NO_FIELD;
		board[4][0] = FieldType::NO_FIELD;
		board[4][1] = FieldType::NORMAL;
		board[4][2] = FieldType::NORMAL;
		board[4][3] = FieldType::NORMAL;
		board[4][4] = FieldType::NO_FIELD;
		return board;
	}

}
