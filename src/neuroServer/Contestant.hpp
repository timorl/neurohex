#ifndef NEUROSERVER_CONTESTANT_HPP
#define NEUROSERVER_CONTESTANT_HPP

#include<memory>
#include<vector>
#include"neuro/Board.hpp"
#include"neuro/Player.hpp"
#include"neuro/Tile.hpp"
#include"utility/DFStyleReader.hpp"
#include"utility/DFStyleCreator.hpp"

namespace neuroServer {

	class Game;
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

		/**
			* @brief Make the move represent the one described.
			* @details If this fails, the move might be in an illegal state, so this
			* should be immiediately called again, before doing anything else.
			* @param[in] input The description.
			* @return True if this operation was successfull, false otherwise.
			*/
		bool fillFromDFStyle(utility::DFStyleReader & input);

		/**
			* @brief Encode the move as DFStyle.
			* @param[out] output The encoder to which to write.
			*/
		void encodeAsDFStyle(utility::DFStyleCreator & output) const;
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

		/**
			* @brief Make the target represent the one described.
			* @details If this fails, the target might be in an illegal state, so this
			* should be immiediately called again, before doing anything else.
			* @param[in] input The description.
			* @return True if this operation was successfull, false otherwise.
			*/
		bool fillFromDFStyle(utility::DFStyleReader & input);

		/**
			* @brief Encode the target as DFStyle.
			* @param[out] output The encoder to which to write.
			*/
		void encodeAsDFStyle(utility::DFStyleCreator & output) const;
	};

	using Targets = std::vector< Target > ;

	/**
		* @brief The interface any contestant (i.e. bot or player ui) should
		* implement.
		*/
	class Contestant {
		public:
			virtual void requestMove(int playerId, const Game & game) = 0;
			virtual Move getMove(int playerId, const Game & game) = 0;
			virtual void requestTargets(int playerId, const Game & game, std::vector< neuro::AbilityIdentifier > abilities) = 0;
			virtual Targets getTargets(int playerId, const Game & game, std::vector< neuro::AbilityIdentifier > abilities) = 0;
			virtual void requestDiscard(int playerId, const Game & game) = 0;
			virtual int getDiscard(int playerId, const Game & game) = 0;
	};

	using ContestantP = std::shared_ptr<Contestant>;

}

#endif
