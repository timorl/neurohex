#include<gtest/gtest.h>
#include"viewmodel/ArmyLoader.hpp"

namespace viewmodel {

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
		neuro::ArmyP moloch = theLoader.getArmy( molochName );
		ASSERT_NE( nullptr, moloch );
		EXPECT_EQ( 35, moloch->tilesLeft() );
	}

	TEST_F(ArmyLoaderTest, GetArmyFromEmpty) {
		ArmyLoader theLoader;
		neuro::ArmyP moloch = theLoader.getArmy( "Moloch" );
		EXPECT_EQ( nullptr, moloch );
	}

	TEST_F(ArmyLoaderTest, GetNonexistentArmy) {
		char * home = std::getenv( "HOME" );
		ASSERT_NE( nullptr, home );
		std::string armyDirectory( home );
		armyDirectory += "/.local/share/neurohex/armies/";
		ArmyLoader theLoader;
		theLoader.loadArmies( armyDirectory );
		neuro::ArmyP nonexistent = theLoader.getArmy( "Sparklypoo" );
		EXPECT_EQ( nullptr, nonexistent );
	}

}
