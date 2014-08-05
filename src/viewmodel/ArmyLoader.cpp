#include"viewmodel/ArmyLoader.hpp"

namespace viewmodel {

	void ArmyLoader::loadArmies( std::string directory ) {
		//TODO: This is a stub.
	}

	neuro::ArmyP ArmyLoader::getArmy( std::string name ) const {
		std::vector< neuro::TileP > tiles;
		for ( int i = 0; i < 15; i++ ) {
			tiles.push_back( neuro::TileP( new neuro::Tile( neuro::TileType::UNIT, 1 ) ) );
		}
		tiles.push_back( neuro::TileP( new neuro::Tile( neuro::TileType::HQ, 1 ) ) );
		return neuro::ArmyP( new neuro::Army( tiles ) );
	}

}
