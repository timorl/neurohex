#ifndef NEURO_BOARD_HPP
#define NEURO_BOARD_HPP

#include<vector>
#include<list>
#include<memory>
#include<utility>
#include"Tile.hpp"

namespace neuro {

	using Coordinates = std::pair<int, int>;
	using Orientation = int;
	using TileOnBoard = std::pair< TileP, Orientation >;
	using Tiles = std::vector< std::vector< std::list< TileOnBoard > > >;

	/**
		* @brief Board field types, including a nonexisting field.
		*/
	enum class FieldType {
		NO_FIELD,
		NORMAL
	};

	using BoardDescription = std::vector< std::vector< FieldType > >;

	/**
		* @brief The board on which the game is happening.
		*/
	class Board : public ui::Observable<Board> {
		public:
			/**
				* @brief Create a board from a description.
				* @param[in] desc A std::vector of std::vectors of FieldType enums containing the
				* description of the board.
				*/
			Board(BoardDescription desc);

			/**
				* @brief Returns the width of the board.
				*/
			int getWidth() const { return width; }

			/**
				* @brief Returns the height of the board.
				*/
			int getHeight() const { return height; }

			/**
				* @brief Returns the FieldType of the field at the provided coordinates.
				* @param[in] coord Coordinates of the field as a std::pair<int, int>.
				*/
			FieldType getFieldDescription(Coordinates coord) const;

			/**
				* @brief Returns a list of tiles with their orientation 
				* laying on the specified field.
				* @param[in] coord Coordinates of the field as a std::pair<int, int>.
				* @return A list of pairs of pointers to tiles and integers
				* representing orientation. The list has one entry for every
				* tile on a field, starting with the one nearest to the board.
				*/
			std::list< TileOnBoard > getTiles(Coordinates coord) const;

			/**
				* @brief Looks up the specified tile on the board.
				* @param[in] tile A shared_ptr to the tile to look up.
				* @return A std::pair<int, int> containing the coordinates of the tile if it
				* is on the board or (-1, -1) otherwise.
				*/
			Coordinates findTile(TileP tile) const;

			/**
				* @brief Returns a list of tiles with their orientation in
				* a direction from a specified field.
				* @param[in] coord Coordinates of the field as a std::pair<int, int>.
				* @param[in] dir Direction in which to look for the tiles. Should be a
				* value between 0 and 5, 0 meaning north-east and the next values being
				* consecutive clockwise directions.
				* @return A list of pairs of pointers to tiles and integers
				* representing orientation. The list has one entry for every
				* field from the first neighbour up to the end of the board in the specified
				* direction. An entry is a pair of a std::shared_ptr and an integer.
				* The shared_ptr points either to the solid Tile laying on the
				* field or nullptr if the field does not contain a solid tile.
				*/
			std::list< TileOnBoard > getSolidTilesInDirection(Coordinates coord, int dir) const;

			/**
				* @brief Places a tile on the board.
				* @param[in] coord Coordinates of the field on which the tile is placed as a std::pair<int, int>.
				* @param[in] tile A pair of a pointer to a Tile 
				* to place at the coordinates and an integer representing the initial
				* orientation of the placed tile.
				* @return true if the move was completed successfully, false otherwise.
				* @todo This should use a custom function for every tile for placement.
				* Possibly the validation should be in another class
				*/
			bool placeTile(Coordinates coord, TileOnBoard tile);

			/**
				* @brief Removes a tile from the board.
				* @param[in] tile The tile to be removed.
				* @return true if the removal was completed successfully, false otherwise.
				*/
			bool removeTile(TileP tile);

		private:
			int width;
			int height;
			BoardDescription fields;
			Tiles tiles;

			bool insideBoard(Coordinates coord) const {
				return ( coord.first > 0 ) && ( coord.first < width ) && ( coord.second > 0 ) && ( coord.second < height );
			}
	};

}

#endif
