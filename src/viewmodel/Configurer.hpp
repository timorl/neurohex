#ifndef VIEWMODEL_CONFIGURER_HPP
#define VIEWMODEL_CONFIGURER_HPP

#include<memory>
#include<vector>
#include"neuro/GameOptions.hpp"
#include"neuro/Game.hpp"

namespace viewmodel {

	/**
		* @brief A class for crating a game configuration.
		*/
	class Configurer : public ui::Observable<Configurer> {
		public:
			/**
				* @brief Starts a game with the set options.
				*/
			void startGame() {	game = neuro::GameP( new neuro::Game( config ) );	sigModified(*this); game->play(); game.reset();	}

			/**
				* @brief Returns the created GameOptions.
				*/
			neuro::GameOptions getConfiguration() const { return config; }

			/**
				* @brief Adds a contestant with an army.
				* @param[in] contestant A pointer to the contestant to add.
				* @param[in] army A pointer to the contestant's army.
				* @return The contestants ID.
				*/
			int addContestant(neuro::ContestantP contestant, neuro::ArmyP army) { config.contestants.push_back(contestant), config.armies.push_back(army); return config.contestants.size() - 1; };

			/**
				* @brief Sets the initial health to use in the game.
				* @param[in] health The health to set.
				*/
			void setInitialHealth(int health) { config.initialHealth = health; };

			/**
				* @brief Sets the board to use in the game.
				* @param[in] board A BoardDescription describing the board.
				*/
			void setBoard(neuro::BoardDescription board) { config.board = board; };
		private:
			neuro::GameOptions config;
			neuro::GameP game;
	};

	using ConfigurerP = std::shared_ptr< Configurer >;

}

#endif
