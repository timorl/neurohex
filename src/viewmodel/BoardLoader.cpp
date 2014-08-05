#include"viewmodel/BoardLoader.hpp"

namespace viewmodel {

	void BoardLoader::loadBoards(std::string directory) {
		//TODO: Actually load something.
	}

	void BoardLoader::addBoard(std::string name, std::string description, neuro::BoardDescription board) {
		if ( boards.count( name ) == 0 ) {
			boards[name] = board;
			descriptions[name] = description;
		}
	}

	neuro::BoardDescription	BoardLoader::getBoard(std::string name) const {
		//FIXME: This just returns a default board always.
		neuro::BoardDescription	board;
		board.resize(5);
		for ( int i = 0; i < 5; i++ ) {
			board[i].resize(5);
		}
		board[0][0] = neuro::FieldType::NO_FIELD;
		board[0][1] = neuro::FieldType::NORMAL;
		board[0][2] = neuro::FieldType::NORMAL;
		board[0][3] = neuro::FieldType::NORMAL;
		board[0][4] = neuro::FieldType::NO_FIELD;
		board[1][0] = neuro::FieldType::NORMAL;
		board[1][1] = neuro::FieldType::NORMAL;
		board[1][2] = neuro::FieldType::NORMAL;
		board[1][3] = neuro::FieldType::NORMAL;
		board[1][4] = neuro::FieldType::NO_FIELD;
		board[2][0] = neuro::FieldType::NORMAL;
		board[2][1] = neuro::FieldType::NORMAL;
		board[2][2] = neuro::FieldType::NORMAL;
		board[2][3] = neuro::FieldType::NORMAL;
		board[2][4] = neuro::FieldType::NORMAL;
		board[3][0] = neuro::FieldType::NORMAL;
		board[3][1] = neuro::FieldType::NORMAL;
		board[3][2] = neuro::FieldType::NORMAL;
		board[3][3] = neuro::FieldType::NORMAL;
		board[3][4] = neuro::FieldType::NO_FIELD;
		board[4][0] = neuro::FieldType::NO_FIELD;
		board[4][1] = neuro::FieldType::NORMAL;
		board[4][2] = neuro::FieldType::NORMAL;
		board[4][3] = neuro::FieldType::NORMAL;
		board[4][4] = neuro::FieldType::NO_FIELD;
		return board;
	}

}
