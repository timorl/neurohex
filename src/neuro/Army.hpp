#ifndef NEURO_ARMY_HPP
#define NEURO_ARMY_HPP

#include<vector>
#include<algorithm>
#include"ui/Observable.hpp"
#include"neuro/Tile.hpp"

namespace neuro {

	using TileP = std::shared_ptr< Tile >;

	/**
		* @brief A container for tiles, to be given to a player.
		*/
	class Army : public ui::Observable<Army> {
		public:
			/**
				* @brief Construct an army from a vector of tiles.
				* @param[in] tiles The vector to initialize the army with.
				*/
			Army( std::vector< TileP > tiles ) : tiles(tiles) {}

			/**
				* @brief Returns the number of tiles left in the army.
				*/
			int tilesLeft() const { return tiles.size(); }

			/**
				* @brief Whether the army is empty.
				*/
			bool isEmpty() const { return tiles.empty(); }

			/**
				* @brief Shuffles the tiles in the army.
				*/
			void shuffle();

			/**
				* @brief Draw one tile from the army.
				* @returns A std::shared_ptr to the Tile drawn. It is empty if the army was
				* empty.
				*/
			TileP drawTile();
		private:
			std::vector< TileP > tiles;
	};

}

#endif
