#ifndef NEURO_CONTESTANT_HPP
#define NEURO_CONTESTANT_HPP

#include<memory>
#include<vector>
#include"neuro/Board.hpp"

namespace neuro {

	using TileP = std::shared_ptr< Tile >;
	using PlayerP = std::shared_ptr< Player >;
	using Players = std::vector< PlayerP >;

	/**
		* @brief A move one may request from a player.
		*/
	struct Move {
		/**
			* @brief Whether this move ends the turn. If true all other parameters are
			* ignored.
			*/
		bool endTurn;

		/**
			* @brief A pointer to the tile to use in the move.
			*/
		TileP tile;

		/**
			* @brief Whether to discard the choosen tile. If true coords is ignored.
			*/
		bool discard;

		/**
			* @brief A list of coordinates to affect with the choosen tile. It should
			* correspond to the requirements of this tile's targetting in the current
			* context.
			*/
		std::list< Coordinates > coords;

		/**
			* @brief The orientation of the tile, if applicable.
			*/
		int orientation;
	};

	/**
		* @brief The interface any contestant (i.e. bot or player ui) should
		* implement.
		*/
	class Contestant {
		public:
			virtual Move getMove(int playerId, const Players & players, const Board & board, bool noArmy) = 0;
	};

}

#endif
