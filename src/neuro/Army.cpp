#include"neuro/Army.hpp"

namespace neuro {

	Army::Army( const Army & other ) {
		for ( auto tile : other.tiles ) {
			tiles.emplace_back( new Tile( *tile ) );
		}
	}

	void Army::shuffle() {
		std::shuffle(std::begin(tiles), std::end(tiles), std::default_random_engine());
	}

	void Army::initialize(int player) {
		owner = player;
		for ( TileP tile : tiles ) {
			tile->setOwner( player );
			tile->setParents( tile );
		}
		sigModified(*this);
	}

	TileP Army::drawTile() {
		TileP result;
		if ( !isEmpty() ) {
			result = tiles.back();
			tiles.pop_back();
		}
		return result;
	}

}
