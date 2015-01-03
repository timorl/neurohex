#ifndef NEUROSERVER_CONFIGURER_HPP
#define NEUROSERVER_CONFIGURER_HPP

#include<memory>
#include<vector>
#include"neuroServer/BoardLoader.hpp"
#include"neuroServer/ArmyLoader.hpp"
#include"neuroServer/GameOptions.hpp"
#include"neuroServer/Game.hpp"

namespace neuroServer {

	using GameP = std::unique_ptr<Game>;

	/**
		* @brief A class for crating a game configuration.
		*/
	class Configurer : public ui::Observable<Configurer> {
		public:
			/**
				* @brief Create a configurer with the given resource loaders.
				* @param[in] boardLoader A BoardLoader which has already loaded
				* boards.
				* @param[in] armyLoader An ArmyLoader which has already loaded
				* armies.
				*/
			Configurer( BoardLoader & boardLoader, ArmyLoader & armyLoader ) : boardLoader(boardLoader), armyLoader(armyLoader) {}

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
			GameOptions getConfiguration() const { return config; }

			/**
				* @brief Returns a list of available boards with short descriptions.
				*/
			const std::map< std::string, std::string > & getAvailableBoards() const { return boardLoader.getDescriptions(); }

			/**
				* @brief Returns a list of available armies with short descriptions.
				*/
			const std::map< std::string, std::string > & getAvailableArmies() const { return armyLoader.getDescriptions(); }

			/**
				* @brief Adds a contestant with an army.
				* @param[in] contestant A pointer to the contestant to add.
				* @return The contestants ID.
				*/
			int addContestant(ContestantP contestant) { config.contestants.push_back(contestant), config.armies.push_back( std::vector<neuro::Tile>() ); sigModified(*this); return config.contestants.size() - 1; };

			/**
				* @brief Sets an army for the specified contestant.
				* @param[in] contestantId The id of the contestant.
				* @param[in] army The tiles the army should consist of.
				*/
			void setArmy(int contestantId, std::vector<neuro::Tile> army);

			/**
				* @brief Sets an army for the specified contestant.
				* @param[in] contestantId The id of the contestant.
				* @param[in] name A name of the army to set.
				*/
			void setArmy(int contestantId, std::string name) { setArmy(contestantId, armyLoader.getArmy(name) ); }

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
			void setBoard( std::string name ) { setBoard( boardLoader.getBoard( name ) ); }
		private:
			BoardLoader & boardLoader;
			ArmyLoader & armyLoader;
			GameOptions config;
			GameP game;
	};

}

#endif
