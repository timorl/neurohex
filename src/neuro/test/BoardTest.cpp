#include<gtest/gtest.h>
#include"neuro/Board.hpp"

namespace neuro {

	class BoardTest : public ::testing::Test {
		public:
			Board * board;
			BoardDescription sampleBoard;
			int width, height;
			BoardTest() {
				width = 9;
				height = 9;
				sampleBoard.resize(width);
				for ( int i = 0; i < width; i++ ) {
					sampleBoard[i].resize(height);
					for ( int j = 0; j < height; j++ ) {
						sampleBoard[i][j] = FieldType::NORMAL;
					}
				}
				board = new Board(sampleBoard);
			}
			~BoardTest() {
				delete board;
			}
	};

	TEST_F(BoardTest, getWidthAndHeight) {
		EXPECT_EQ( width, board->getWidth() );
		EXPECT_EQ( height, board->getHeight() );
	}

}
