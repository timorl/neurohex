#ifndef NEURO_BOARD_HPP
#define NEURO_BOARD_HPP

#include<vector>
#include<list>
#include<memory>
#include<utility>
#include"Tile.hpp"
#include"utility/DFStyleReader.hpp"
#include"utility/DFStyleCreator.hpp"

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
				* @brief Returns the description of this board.
				*/
			const BoardDescription	& getBoardDescription() const { return fields; }

			/**
				* @brief Returns the FieldType of the field at the provided coordinates.
				* @param[in] coord Coordinates of the field as a std::pair<int, int>.
				*/
			FieldType getFieldDescription(Coordinates coord) const;

			/**
				* @brief Returns the tiles laying on the board.
				*/
			const Tiles & getTiles() const { return tiles; }

			/**
				* @brief Returns a list of tiles with the specified initiative on board.
				* @param[in] initiative The initiative of the tiles to find.
				*/
			std::list< TileP > getTilesWithInitiative( int initiative ) const;

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
				* @brief Whether there is a solid tile at the given coordinate.
				*/
			bool containsSolid( Coordinates coord ) const;

			/**
				* @brief Looks up the specified tile on the board.
				* @param[in] tile A shared_ptr to the tile to look up.
				* @return A std::pair<int, int> containing the coordinates of the tile if it
				* is on the board or (-1, -1) otherwise.
				*/
			Coordinates findTile(TileP tile) const;

			/**
				* @brief Returns the highest initiative any tile currently on the board has.
				*/
			int getMaxInitiative() const;

			/**
				* @brief Whether the board is full, and therefore a battle should happen.
				*/
			bool isFull() const;

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
				* @param[in] orientation The initial orientation with which the tile is
				* placed.
				* @param[in] tile A pointer to the Tile to place at the coordinates.
				*/
			void placeTile(Coordinates coord, Orientation orientation, TileP tile);

			/**
				* @brief Removes a tile from the board.
				* @param[in] tile The tile to be removed.
				* @return true if the removal was completed successfully, false otherwise.
				*/
			bool removeTile(TileP tile);

			/**
				* @brief Get any activated tile on the board.
				* @return A pointer to an activated tile on the board or an empty pointer if
				* no tile is activated.
				*/
			TileP getActivatedTile();

			/**
				* @brief Make the board represent the one described.
				* @details If this fails, the board might be in an illegal state, so this
				* should be immiediately called again, before doing anything else.
				* @param[in] input The description.
				* @return True if this operation was successfull, false otherwise.
				* @todo This probably should actually be in a different class representing
				* the game for the client. Really needs work, but... after The Eduardo.
				*/
			bool fillFromDFStyle(utility::DFStyleReader & input);

			/**
				* @brief Encode the board as DFStyle.
				* @param[out] output The encoder to which to write.
				*/
			void encodeAsDFStyle(utility::DFStyleCreator & output);
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
