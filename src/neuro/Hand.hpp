#ifndef NEURO_HAND_HPP
#define NEURO_HAND_HPP

#include<list>
#include"ui/Observable.hpp"
#include"neuro/Tile.hpp"

namespace neuro {

	using TileP = std::shared_ptr< Tile >;

	/**
		* @brief A container for tiles being held in the hand of the player.
		*/
	class Hand : public ui::Observable<Hand> {
		public:
			/**
				* @brief Construct an empty hand.
				*/
			Hand() {}

			/**
				* @brief Returns a std::list of pointers to tiles contained in the hand.
				*/
			const std::list< TileP > & getContents() const {	return tiles;	}

			/**
				* @brief Returns the number of tiles currently held in the hand.
				*/
			int getNumberOfTiles() const { return tiles.size(); }

			/**
				* @brief Returns whether the hand contains the specified tile
				* @param[in] tile A pointer the a tile the hand might contain.
				*/
			bool containsTile( TileP tile ) const;

			/**
				* @brief Adds a tile to the hand.
				* @param[in] tile The tile to add.
				*/
			void insertTile(TileP tile) { tiles.push_back(tile); sigModified(*this); }

			/**
				* @brief Removes the specified tile from the hand.
				* @param[in] tilePos The position of the tile to remove.
				*/
			void removeTile(TileP tile) { tiles.remove(tile); sigModified(*this); }

		private:
			std::list< TileP > tiles;
	};

}

#endif
