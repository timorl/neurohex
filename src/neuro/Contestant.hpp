#ifndef NEURO_CONTESTANT_HPP
#define NEURO_CONTESTANT_HPP

#include<memory>
#include<vector>
#include<tuple>
#include"neuro/Board.hpp"
#include"neuro/Player.hpp"
#include"neuro/Tile.hpp"

namespace neuro {

	using Players = std::vector< PlayerP >;

	/**
		* @brief A move one may request from a player during his turn.
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
			* @brief Whether to discard the choosen tile. If true all following
			* parameters are ignored.
			*/
		bool discard;

		/**
			* @brief The group of abilities the ability to use belongs to.
			*/
		AbilityGroup abilityGroup;

		/**
			* @brief the id of the tile's ability to use.
			*/
		int abilityId;
	};

	using Targets = std::list< std::tuple< Coordinates, Orientation, std::list< TileP > > > ;

	/**
		* @brief The interface any contestant (i.e. bot or player ui) should
		* implement.
		*/
	class Contestant {
		public:
			virtual Move getMove(int playerId, const Players & players, const Board & board, bool noArmy) = 0;
			virtual Targets getTargets(int playerId, const Players & players, const Board & board, bool noArmy, const Tile & tile, AbilityGroup abilityGroup, int abilityId) = 0;
	};

	using ContestantP = std::shared_ptr< Contestant >;

}

#endif
