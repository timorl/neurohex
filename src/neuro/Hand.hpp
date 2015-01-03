#ifndef NEURO_HAND_HPP
#define NEURO_HAND_HPP

#include<list>
#include"ui/Observable.hpp"
#include"neuro/Tile.hpp"
#include"utility/DFStyleReader.hpp"
#include"utility/DFStyleCreator.hpp"

namespace neuro {

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
				* @brief Move a hand.
				*/
			Hand(Hand && orig) : tiles(orig.tiles) {}

			/**
				* @brief Returns a std::list of global IDs of tiles contained in the hand.
				*/
			const std::list< int > & getContents() const {	return tiles;	}

			/**
				* @brief Returns the number of tiles currently held in the hand.
				*/
			int getNumberOfTiles() const { return tiles.size(); }

			/**
				* @brief Returns whether the hand contains the specified tile
				* @param[in] tile The global ID of a tile the hand might contain.
				*/
			bool containsTile( int tile ) const;

			/**
				* @brief Adds a tile to the hand.
				* @param[in] tile The tile to add.
				*/
			void insertTile(int tile) { tiles.push_back(tile); sigModified(*this); }

			/**
				* @brief Removes the specified tile from the hand.
				* @param[in] tile The tile to remove.
				*/
			void removeTile(int tile) { tiles.remove(tile); sigModified(*this); }

			/**
				* @brief Make the hand represent the one described.
				* @details If this fails, the hand might be in an illegal state, so this
				* should be immiediately called again, before doing anything else.
				* @param[in] input The description.
				* @return True if this operation was successfull, false otherwise.
				*/
			bool fillFromDFStyle(utility::DFStyleReader & input);

			/**
				* @brief Encode the hand as DFStyle.
				* @param[out] output The encoder to which to write.
				*/
			void encodeAsDFStyle(utility::DFStyleCreator & output);
		private:
			std::list< int > tiles;
	};

}

#endif
