#include<gtest/gtest.h>
#include"neuroServer/ArmyLoader.hpp"

namespace neuroServer {

	class ArmyLoaderTest : public ::testing::Test {
		public:
			ArmyLoaderTest() {}
			~ArmyLoaderTest() {}
	};

	//This test assumes that there is a Moloch army in ~/.local/share/neuro/armies/.
	TEST_F(ArmyLoaderTest, LoadMoloch) {
		char * home = std::getenv( "HOME" );
		ASSERT_NE( nullptr, home );
		std::string armyDirectory( home );
		armyDirectory += "/.local/share/neurohex/armies/";
		ArmyLoader theLoader;
		theLoader.loadArmies( armyDirectory );
		auto descs = theLoader.getDescriptions();
		std::string molochName = "Moloch";
		EXPECT_EQ( 1, descs.count( molochName ) );
		std::vector<neuro::Tile> moloch = theLoader.getArmy( molochName );
		EXPECT_EQ( 35, moloch.size() );
	}

}
