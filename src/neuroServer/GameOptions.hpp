#ifndef NEUROSERVER_GAMEOPTIONS_HPP
#define NEUROSERVER_GAMEOPTIONS_HPP

#include<memory>
#include<vector>
#include"neuro/Army.hpp"
#include"neuro/Board.hpp"
#include"neuroServer/Contestant.hpp"

namespace neuroServer {

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
			* @brief Tiles in armies of players in order of players.
			*/
		std::vector< std::vector<neuro::Tile> > armies;

		/**
			* @brief The health all the players will be starting with.
			*/
		int initialHealth = 20;

		/**
			* @brief The description of the Board to play on.
			*/
		neuro::BoardDescription board;
	};

}

#endif
