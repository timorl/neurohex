#ifndef NEURO_ARBITER_HPP
#define NEURO_ARBITER_HPP

#include<memory>
#include<vector>
#include"neuro/Board.hpp"
#include"neuro/Player.hpp"
#include"neuro/Contestant.hpp"

namespace neuro {

	using Players = std::vector< PlayerP >;

	/**
		* @brief The entity relaying instructions from the Contestant to the Game.
		*/
	class Arbiter {
		public:
			/**
				* @brief Constructs an arbiter for the given contestant.
				* @param[in] contestant The contestant for which we need an arbiter.
				*/
			Arbiter(ContestantP contestant) : contestant(contestant) {}

			/**
				* @brief Returns a validated move the contestant wants to make.
				* @param[in] playerId The id of the Player the contestant controls.
				* @param[in] players A reference to a std::vector of players in the game.
				* @param[in] board A reference to the board.
				* @param[in] noArmy Whether any players army already ran out.
				* @return A validated move the contestant wants to make.
				* @todo Actually validate the moves.
				*/
				Move getMove( int playerId, const Players & players, const Board & board, bool noArmy );
		private:
			ContestantP contestant;
	};

}

#endif
