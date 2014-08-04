#ifndef NEURO_CONFIGURER_HPP
#define NEURO_CONFIGURER_HPP

#include<memory>
#include<vector>
#include"neuro/GameOptions.hpp"

namespace neuro {

	using TileP = std::shared_ptr< Tile >;
	using ContestantP = std::shared_ptr< Contestant >;
	using PlayerP = std::shared_ptr< Player >;
	using Constestants = std::vector< ContestantP >;
	using Players = std::vector< PlayerP >;

	/**
		* @brief A class for crating a game configuration.
		*/
	class Configurer : public Observable<Configurer> {
		public:
			/**
				* @brief Returns the created GameOptions.
				*/
			GameOptions getConfiguration() const { return config; }

			/**
				* @brief Adds a contestant with an army.
				* @param[in] contestant A pointer to the contestant to add.
				* @param[in] army A pointer to the contestant's army.
				* @return The contestants ID.
				*/
			int addContestant(ContestantP contestant, ArmyP army) { config.contestants.push_back(contestant), config.armies.push_back(army); return config.contestant.size() - 1 };

			/**
				* @brief Sets the initial health to use in the game.
				* @param[in] health The health to set.
				*/
			void setInitialHealth(int health) { config.initialHealth = health };

			/**
				* @brief Sets the board to use in the game.
				* @param[in] board A BoardDescription describing the board.
				*/
			void setBoard(BoardDescription board) { config.board = board };
		private:
			GameOptions config;
	};

}

#endif
