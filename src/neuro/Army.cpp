#include"neuro/Army.hpp"

namespace neuro {

	void Army::shuffle() {
		std::shuffle(std::begin(tiles), std::end(tiles), std::default_random_engine());
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
