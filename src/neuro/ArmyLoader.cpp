#include"neuro/ArmyLoader.hpp"

namespace neuro {

	void ArmyLoader::loadArmies( std::string directory ) {
		//TODO: This is a stub.
	}

	ArmyP ArmyLoader::getArmy( std::string name ) const {
		std::vector< TileP > tiles;
		for ( int i = 0; i < 15; i++ ) {
			tiles.push_back( TileP( new Tile( TileType::UNIT, 1 ) ) );
		}
		tiles.push_back( TileP( new Tile( TileType::HQ, 1 ) ) );
		return ArmyP( new Army( tiles ) );
	}

}
