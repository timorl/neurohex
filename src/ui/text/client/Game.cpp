#ifndef UI_TEXT_CLIENT_GAMESTATE_CPP
#define UI_TEXT_CLIENT_GAMESTATE_CPP

namespace ui {
namespace text {
namespace client {

    int Game::Game(neuroClient::Game & game) {
        game_state = game;
        //TODO register signal
    }

    void Game::gameEnded() {
        if ( game_state.isFinished() ) {
            std::cout << "Game has finished!" << endl;
        } else {
            std::cout << "Game not yet finished." << endl;
        }
    }

}
}
}

#endif /* UI_TEXT_CLIENT_GAMESTATE_CPP */

