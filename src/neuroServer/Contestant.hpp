#ifndef NEUROSERVER_CONTESTANT_HPP
#define NEUROSERVER_CONTESTANT_HPP

#include<memory>
#include<vector>
#include"neuro/Board.hpp"
#include"neuro/Player.hpp"
#include"neuro/Tile.hpp"

namespace neuroServer {

	using Players = std::vector< neuro::Player >;

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
			* @brief Whether to discard a tile.
			*/
		bool discard;

		/**
			* @brief The identifier of the ability to use, if discard is true everything
			* but the tile is ignored.
			*/
		neuro::AbilityIdentifier abilityIdentifier;
	};

	/**
		* @brief Represents a single targetted field.
		*/
	struct Target {
		/**
			* @brief The coordinates of the field.
			*/
		neuro::Coordinates coords;

		/**
			* @brief An orientation, if applicable for targetting.
			*/
		neuro::Orientation orientation;

		/**
			* @brief A list of tiles on the field being targetted.
			*/
		std::list< int > tiles;
	};

	using Targets = std::list< Target > ;

	/**
		* @brief The interface any contestant (i.e. bot or player ui) should
		* implement.
		*/
	class Contestant {
		public:
			virtual Move getMove(int playerId, const Players & players, const neuro::Board & board, bool noArmy) = 0;
			virtual Targets getTargets(int playerId, const Players & players, const neuro::Board & board, bool noArmy, neuro::AbilityIdentifier & abilityIdentifier) = 0;
			virtual int requestDiscard(int playerId, const Players & players, const neuro::Board & board, bool noArmy) = 0;
	};

	using ContestantP = std::shared_ptr<Contestant>;

}

#endif
