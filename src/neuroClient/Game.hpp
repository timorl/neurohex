#ifndef NEUROCLIENT_GAME_HPP
#define NEUROCLIENT_GAME_HPP

#include<memory>
#include<vector>
#include"neuro/Board.hpp"
#include"neuro/Player.hpp"
#include"neuro/Tile.hpp"
#include"ui/Observable.hpp"
#include"utility/DFStyleReader.hpp"
#include"utility/DFStyleCreator.hpp"

namespace neuroClient {

	using Players = std::vector<neuro::Player>;

	/**
		* @brief A game containing some players.
		*/
	class Game : public ui::Observable<Game> {
		public:
			/**
				* @brief Returns the number of players in the game.
				*/
			int getNumberOfPlayers() const { return players.size(); }

			/**
				* @brief Returns the number of players still alive in the game.
				*/
			int getNumberOfLivingPlayers() const;

			/**
				* @brief Returns the id of the player currently playing.
				*/
			int getCurrentPlayer() const { return currentPlayer; }

			/**
				* @brief Whether the current game is finished.
				*/
			bool isFinished() const;

			/**
				* @brief Getter for players.
				*/
            std::vector<neuro::Player> & getPlayers();

			/**
				* @brief Make the game represent the one described.
				* @details If this fails, the game might be in an illegal state, so this
				* should be immiediately called again, before doing anything else.
				* @param[in] input The description.
				* @return True if this operation was successfull, false otherwise.
				*/
			bool fillFromDFStyle(utility::DFStyleReader & input);
		private:
			Players players;
			neuro::Board board;
			int currentPlayer;
			bool noArmy;
	};

}

#endif
