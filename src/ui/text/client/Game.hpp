#ifndef UI_TEXT_CLIENT_GAMESTATE_HPP
#define UI_TEXT_CLIENT_GAMESTATE_HPP

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
				* @brief Print wether the game has ended. 
				*/
            void gameEnded() const;

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

#endif /* UI_TEXT_CLIENT_GAMESTATE_HPP */


