#include"neuro/Hand.hpp"

namespace neuro {

	bool Hand::containsTile( TileP tile ) const {
		for ( TileP tp : tiles ) {
			if ( tp == tile ) {
				return true;
			}
		}
		return false;
	}

}
