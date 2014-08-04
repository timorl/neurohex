#include"viewmodel/MainMenu.hpp"

namespace viewmodel {

	void MainMenu::playGame() {
		if ( !configurer ) {
			configurer = ConfigurerP( new Configurer() );
		}
		event = MenuEvent::PLAY_GAME;
		sigModified(*this);
	}

}
