#include<gtest/gtest.h>
#include"neuroServer/BoardLoader.hpp"

namespace neuroServer {

	class BoardLoaderTest : public ::testing::Test {
		public:
			BoardLoaderTest() {}
			~BoardLoaderTest() {}
	};

	//This test assumes that there is a standard 5x5 board in ~/.local/share/neuro/boards/.
	TEST_F(BoardLoaderTest, LoadStandardBoard) {
		char * home = std::getenv( "HOME" );
		ASSERT_NE( nullptr, home );
		std::string boardDirectory( home );
		boardDirectory += "/.local/share/neurohex/boards/";
		BoardLoader theLoader;
		theLoader.loadBoards( boardDirectory );
		auto descs = theLoader.getDescriptions();
		std::string name = "Standard Board";
		EXPECT_EQ( 1, descs.count( name ) );
		neuro::BoardDescription fields = theLoader.getBoard( name );
		EXPECT_EQ( neuro::FieldType::NO_FIELD, fields[0][0] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[1][0] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[2][0] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[3][0] );
		EXPECT_EQ( neuro::FieldType::NO_FIELD, fields[4][0] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[0][1] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[1][1] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[2][1] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[3][1] );
		EXPECT_EQ( neuro::FieldType::NO_FIELD, fields[4][1] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[0][2] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[1][2] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[2][2] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[3][2] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[4][2] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[0][3] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[1][3] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[2][3] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[3][3] );
		EXPECT_EQ( neuro::FieldType::NO_FIELD, fields[4][3] );
		EXPECT_EQ( neuro::FieldType::NO_FIELD, fields[0][4] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[1][4] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[2][4] );
		EXPECT_EQ( neuro::FieldType::NORMAL, fields[3][4] );
		EXPECT_EQ( neuro::FieldType::NO_FIELD, fields[4][4] );
	}

}
