#include"Board.hpp"

namespace neuro {

	void advanceInDirection(Coordinates & coord, int dir) {
		switch ( dir ) {
			case 0:
			case 2:
				coord.first += (coord.second % 2 == 0) ? 0 : 1;
				break;
			case 3:
			case 5:
				coord.first += (coord.second % 2 == 0) ? -1 : 0;
				break;
			case 1:
				coord.first += 1;
				break;
			case 4:
				coord.first += -1;
				break;
		}
		switch ( dir ) {
			case 0:
			case 5:
				coord.second += -1;
				break;
			case 1:
			case 4:
				coord.second += 0;
				break;
			case 2:
			case 3:
				coord.second += 1;
				break;
		}
	}

	Board::Board( BoardDescription desc ) : fields(desc) {
		width = fields.size();
		if ( width == 0 ) {
			height = 0;
		} else {
			height = fields[0].size();
		}
	}

	FieldType Board::getFieldDescription(Coordinates coord) const {
		if ( insideBoard(coord) ) {
			return fields[coord.first][coord.second];
		} else {
			return FieldType::NO_FIELD;
		}
	}

	std::list< TileOnBoard > Board::getTiles(Coordinates coord) const {
		if ( insideBoard(coord) ) {
			return tiles[coord.first][coord.second];
		} else {
			return std::list< TileOnBoard >();
		}
	}

	Coordinates Board::findTile(TileP tile) const {
		Coordinates result(-1, -1);
		for ( int x = 0; x < static_cast<int>( tiles.size() ); x++ ) {
			for ( int y = 0; y < static_cast<int>( tiles[x].size() ); y++ ) {
				for ( TileOnBoard tob : tiles[x][y] ) {
					if ( tob.first == tile ) {
						result.first = x;
						result.second = y;
						return result;
					}
				}
			}
		}
		return result;
	}

	std::list< TileOnBoard > Board::getSolidTilesInDirection(Coordinates coord, int dir) const {
		std::list< TileOnBoard > result;
		advanceInDirection(coord, dir);
		while ( insideBoard(coord) ) {
			bool foundSolid = false;
			for ( TileOnBoard tob : tiles[coord.first][coord.second] ) {
				if ( tob.first->isSolid() ) {
					result.push_back( tob );
					foundSolid	= true;
					break;
				}
			}
			if ( !foundSolid ) {
				result.push_back( TileOnBoard() );
			}
			advanceInDirection(coord, dir);
		}
		return result;
	}

	bool Board::placeTile( Coordinates coord, TileOnBoard tile ) {
		if ( insideBoard(coord) ) {
			if ( getFieldDescription(coord) != FieldType::NO_FIELD ) {
				// No more validation, assume the game validates; maybe validation later?
				tiles[coord.first][coord.second].push_front(tile);
				sigModified(*this);
				return true;
			}
		}
		return false;
	}

	bool Board::removeTile(TileP tile) {
		Coordinates toRem = findTile(tile);
		if ( toRem.first == -1 ) {
			return false;
		} else {
			tiles[toRem.first][toRem.second].remove_if( 
					[tile]( const TileOnBoard & tob ) { return tob.first == tile; }
					);
			sigModified(*this);
			return true;
		}
	}

}
