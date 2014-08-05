#ifndef VIEWMODEL_CONFIGURER_HPP
#define VIEWMODEL_CONFIGURER_HPP

#include<memory>
#include<vector>
#include"viewmodel/BoardLoader.hpp"
#include"viewmodel/ArmyLoader.hpp"
#include"neuro/GameOptions.hpp"
#include"neuro/Game.hpp"

namespace viewmodel {

	/**
		* @brief A class for crating a game configuration.
		*/
	class Configurer : public ui::Observable<Configurer> {
		public:
			/**
				* @brief Create a configurer with the given resource loaders.
				* @param[in] boardLoader A pointer to a BoardLoader which has already loaded
				* boards.
				* @param[in] armyLoader A pointer to an ArmyLoader which has already loaded
				* armies.
				*/
			Configurer( BoardLoaderP boardLoader, ArmyLoaderP armyLoader ) : boardLoader(boardLoader), armyLoader(armyLoader) {}

			/**
				* @brief Check whether the current settings are valid.
				* @todo Actually verify something.
				*/
			bool verifySettings() const;

			/**
				* @brief Starts a game with the set options.
				* @return true if successful, false if the specified configuration is not
				* valid.
				*/
			bool startGame();

			/**
				* @brief Returns the created GameOptions.
				*/
			neuro::GameOptions getConfiguration() const { return config; }

			/**
				* @brief Returns a list of available boards with short descriptions.
				*/
			const std::map< std::string, std::string > & getAvailableBoards() const { return boardLoader->getDescriptions(); }

			/**
				* @brief Returns a list of available armies with short descriptions.
				*/
			const std::map< std::string, std::string > & getAvailableArmies() const { return armyLoader->getDescriptions(); }

			/**
				* @brief Adds a contestant with an army.
				* @param[in] contestant A pointer to the contestant to add.
				* @return The contestants ID.
				*/
			int addContestant(neuro::ContestantP contestant) { config.contestants.push_back(contestant), config.armies.push_back( neuro::ArmyP() ); sigModified(*this); return config.contestants.size() - 1; };

			/**
				* @brief Sets an army for the specified contestant.
				* @param[in] contestantId The id of the contestant.
				* @param[in] army A pointer to the army to set.
				*/
			void setArmy(int contestantId, neuro::ArmyP army); 

			/**
				* @brief Sets an army for the specified contestant.
				* @param[in] contestantId The id of the contestant.
				* @param[in] name A name of the army to set.
				*/
			void setArmy(int contestantId, std::string name) { setArmy(contestantId, armyLoader->getArmy(name) ); }

			/**
				* @brief Sets the initial health to use in the game.
				* @param[in] health The health to set.
				*/
			void setInitialHealth(int health) { config.initialHealth = health; };

			/**
				* @brief Sets the board to use in the game.
				* @param[in] board A BoardDescription describing the board.
				*/
			void setBoard( neuro::BoardDescription board ) { config.board = board; };

			/**
				* @brief Sets the board to use in the game.
				* @param[in] name The name of the board to set.
				*/
			void setBoard( std::string name ) { setBoard( boardLoader->getBoard( name ) ); }
		private:
			BoardLoaderP boardLoader;
			ArmyLoaderP armyLoader;
			neuro::GameOptions config;
			neuro::GameP game;
	};

	using ConfigurerP = std::shared_ptr< Configurer >;

}

#endif
