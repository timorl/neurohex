#include<iostream>
#include"ui/Observable.hpp"
#include"neuroClient/Game.hpp"

namespace ui {
namespace text {
namespace client {

	/**
		* @brief A class to store and display game state.
		*/
	class Game {
		public:
			/**
				* @brief Print whether the game has ended. 
				*/
            void gameEnded() const;

			/**
				* @brief Print the player to which the current move belongs.
				*/
            void currentPlayer() const;

			/**
				* @brief Print the amount of living players.
				*/
            void numberOfLivingPlayers() const;

			/**
				* @brief Print the amount of players in the whole game.
				*/
            void numberOfPlayers() const;

			/**
				* @brief Initialize with neuroClient/Game.
				*/
		    Game(neuroClient::Game & game);
		private:
			neuroClient::Game & game_state;
	};

}
}
}



