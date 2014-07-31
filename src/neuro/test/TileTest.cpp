#include<gtest/gtest.h>
#include"neuro/Tile.hpp"

namespace neuro {

	class TileTest : public ::testing::Test {
		public:
			TileTest() {}
			~TileTest() {}
	};

	TEST_F(TileTest, createInstantActionTile) {
		Tile * tile = new Tile(TileType::INSTANT_ACTION);
		EXPECT_EQ( TileType::INSTANT_ACTION, tile->getType() );
		EXPECT_EQ( false, tile->isSolid() );
	}

}
