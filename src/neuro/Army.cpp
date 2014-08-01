#include"neuro/Army.hpp"

namespace neuro {

	void Army::shuffle() {
		std::shuffle(std::begin(tiles), std::end(tiles), std::default_random_engine());
	}

	void Army::setOwner(int player) {
		owner = player;
		for ( TileP tile : tiles ) {
			tile->setOwner(player);
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
