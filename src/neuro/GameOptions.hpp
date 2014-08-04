#ifndef NEURO_GAMEOPTIONS_HPP
#define NEURO_GAMEOPTIONS_HPP

#include<memory>
#include<vector>
#include"neuro/Army.hpp"
#include"neuro/Board.hpp"
#include"neuro/Contestant.hpp"

namespace neuro {

	using ContestantP = std::shared_ptr<Contestant>;
	using Contestants = std::vector<ContestantP>;

	/**
		* @brief Options needed to create a new Game.
		*/
	struct GameOptions {
		/**
			* @brief Pointers to contestants playing in order of players.
			*/
		Contestants contestants;

		/**
			* @brief Pointers to armies of players in order of players.
			*/
		std::vector< ArmyP > armies;

		/**
			* @brief The health all the players will be starting with.
			*/
		int initialHealth = 20;

		/**
			* @brief The description of the Board to play on.
			*/
		BoardDescription board;
	};

}

#endif
