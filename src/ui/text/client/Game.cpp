
namespace ui {
namespace text {
namespace client {

    int Game::Game(neuroClient::Game & game) {
        game_state = game;
        //TODO register signal
    }

    void Game::gameEnded() const {
        if ( game_state.isFinished() ) {
            std::cout << "Game has finished!" << endl;
        } else {
            std::cout << "Game not yet finished." << endl;
        }
    }

    void Game::currentPlayer() const {
        std::cout << game_state.getCurrentPlayer() << endl;
    }

    void Game::numberOfLivingPlayers() const {
        std::cout << game_state.getNumberOfLivingPlayers() << endl;
    }

    void Game::numberOfPlayers() const {
        std::cout << game_state.getNumberOfPlayers() << endl;
    }

}
}
}


