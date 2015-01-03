#ifndef NEURO_ARMY_HPP
#define NEURO_ARMY_HPP

#include<vector>
#include<algorithm>
#include"ui/Observable.hpp"
#include"neuro/Tile.hpp"
#include"utility/DFStyleReader.hpp"
#include"utility/DFStyleCreator.hpp"

namespace neuro {

	/**
		* @brief A container for tiles, to be given to a player.
		*/
	class Army : public ui::Observable<Army> {
		public:
			/**
				* @brief Construct an army from a vector of tiles.
				* @param[in] tiles The vector to initialize the army with.
				* @param[in] owner The id of the player to own the army.
				*/
			Army(std::vector<int> tiles, int owner);

			/**
				* @brief Move an army.
				*/
			Army(Army && orig) : owner(orig.owner), tiles(orig.tiles) {}

			/**
				* @brief Returns the number of tiles left in the army.
				*/
			int tilesLeft() const { return tiles.size(); }

			/**
				* @brief Returns the id of the player owning the army.
				*/
			int getOwner() const { return owner; }

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
				* @returns The global ID of the Tile drawn. It is -1 if the army was
				* empty.
				*/
			int drawTile();

			/**
				* @brief Make the army represent the one described.
				* @details If this fails, the army might be in an illegal state, so this
				* should be immiediately called again, before doing anything else.
				* @param[in] input The description.
				* @return True if this operation was successfull, false otherwise.
				*/
			bool fillFromDFStyle(utility::DFStyleReader & input);

			/**
				* @brief Encode the army as DFStyle.
				* @param[out] output The encoder to which to write.
				* @todo SECURITY: shuffle tiles as they are being encoded, since this is not
				* public information.
				*/
			void encodeAsDFStyle(utility::DFStyleCreator & output);
		private:
			int owner;
			std::vector< int > tiles;
	};

}

#endif
