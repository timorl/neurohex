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
				* @brief Provides a deep copy of the given army, but with no owner or observer.
				* @param[in] other The object to be copied.
				*/
			Army( const Army & other );

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
				* @brief Sets the owner of the army and sets parents for tile abilities.
				* @param[in] player The id of the player to own the army.
				*/
			void initialize(int player);

			/**
				* @brief Draw one tile from the army.
				* @returns A std::shared_ptr to the Tile drawn. It is empty if the army was
				* empty.
				*/
			TileP drawTile();

			/**
				* @brief Make the army represent the one described.
				* @details If this fails, the army might be in an illegal state, so this
				* should be immiediately called again, before doing anything else.
				* @param[in] input The description.
				* @return True if this operation was successfull, false otherwise.
				* @todo This probably should actually be in a different class representing
				* the game for the client. Really needs work, but... after The Eduardo.
				*/
			bool fillFromDFStyle(utility::DFStyleReader & input);

			/**
				* @brief Encode the army as DFStyle.
				* @param[out] output The encoder to which to write.
				*/
			void encodeAsDFStyle(utility::DFStyleCreator & output);
		private:
			int owner;
			std::vector< TileP > tiles;
	};

	using ArmyP = std::shared_ptr< Army >;

}

#endif
