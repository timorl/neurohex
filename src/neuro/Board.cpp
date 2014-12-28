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
			tiles.resize( width );
			for ( int i = 0; i < width; i++ ) {
				tiles[i].resize( height );
			}
		}
	}

	FieldType Board::getFieldDescription(Coordinates coord) const {
		if ( insideBoard(coord) ) {
			return fields[coord.first][coord.second];
		} else {
			return FieldType::NO_FIELD;
		}
	}

	std::list< TileP > Board::getTilesWithInitiative( int initiative ) const {
		std::list< TileP > result;
		for ( auto tileColumn : tiles ) {
			for ( auto tileList : tileColumn ) {
				for ( auto tileOnBoard : tileList ) {
					if ( tileOnBoard.first->hasInitiative( initiative ) ) {
						result.push_back( tileOnBoard.first );
					}
				}
			}
		}
		return result;
	}

	std::list< TileOnBoard > Board::getTiles(Coordinates coord) const {
		if ( insideBoard(coord) ) {
			return tiles[coord.first][coord.second];
		} else {
			return std::list< TileOnBoard >();
		}
	}

	bool Board::containsSolid( Coordinates coord ) const {
		auto tls = getTiles( coord );
		for ( auto t : tls ) {
			if ( t.first->isSolid() ) {
				return true;
			}
		}
		return false;
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

	bool Board::isFull() const {
		for ( int i = 0; i < width; i++ ) {
			for ( int j = 0; j < height; j++ ) {
				if ( fields[i][j] != FieldType::NO_FIELD && !containsSolid( std::make_pair(i, j) ) ) {
					return false;
				}
			}
		}
		return true;
	}

	int Board::getMaxInitiative() const {
		int result = -1;
		for ( auto tileColumn : tiles ) {
			for ( auto tileList : tileColumn ) {
				for ( auto tileOnBoard : tileList ) {
					if ( tileOnBoard.first->getHighestInitiative() > result ) {
						result = tileOnBoard.first->getHighestInitiative();
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

	void Board::placeTile( Coordinates coord, Orientation orientation, TileP tile ) {
		tiles[coord.first][coord.second].push_front( std::make_pair( tile, orientation ) );
		sigModified(*this);
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

	TileP Board::getActivatedTile() {
		TileP result;
		for ( auto tileColumn : tiles ) {
			for ( auto tileList : tileColumn ) {
				for ( auto tileOnBoard : tileList ) {
					if ( tileOnBoard.first->isActivated() ) {
						return tileOnBoard.first;
					}
				}
			}
		}
		return result;
	}

	bool Board::fillFromDFStyle(utility::DFStyleReader & input) {
		fields.clear();
		tiles.clear();
		while ( input.hasNextToken() ) {
			std::vector< std::string > info = input.getNextToken();
			if (  static_cast<int>( info.size() ) < 1 ) {
				return false;
			}
			std::string type = info[0];
			if ( type == "BOARDEND" ) {
				return true;
			} else if ( type == "DIMENSIONS" ) {
				if (  static_cast<int>( info.size() ) < 3 ) {
					return false;
				}
				width = std::stoi( info[1] );
				height = std::stoi( info[2] );
				fields.resize(width);
				tiles.resize(height);
				for ( int i = 0; i < width; i++ ) {
					fields[i].resize(height);
					tiles[i].resize(height);
				}
			} else if ( type == "TILEBEGIN" ) {
				if (  static_cast<int>( info.size() ) < 4 ) {
					return false;
				}
				Coordinates coords;
				int & x = coords.first;
				int & y = coords.second;
				x = std::stoi(info[1]);
				y = std::stoi(info[2]);
				int ori = std::stoi(info[3]);
				if ( !insideBoard(coords) || ori < 0 || ori > 5 ) {
					return false;
				}
				TileP tile = Tile::getDummy();
				if ( !tile->fillFromDFStyle(input) ) {
					return false;
				}
				tile->setParents(tile);
				tiles[x][y].push_back(std::make_pair(tile, ori));
			} else if ( type == "FIELD" ) {
				if (  static_cast<int>( info.size() ) < 4 ) {
					return false;
				}
				Coordinates coords;
				int & x = coords.first;
				int & y = coords.second;
				x = std::stoi(info[1]);
				y = std::stoi(info[2]);
				if ( !insideBoard(coords) ) {
					return false;
				}
				if ( info[3] == "no_field" ) {
					fields[x][y] = FieldType::NO_FIELD;
				} else if ( info[3] == "normal" ) {
					fields[x][y] = FieldType::NORMAL;
				} else {
					return false;
				}
			} else {
				return false;
			}
		}
		return false;
	}

}
