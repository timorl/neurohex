#include"viewmodel/MainMenu.hpp"

namespace viewmodel {

	void MainMenu::createGame() {
		if ( !configurer ) {
			configurer = ConfigurerP( new Configurer( boardLoader, armyLoader ) );
		}
		event = MenuEvent::CREATE_GAME;
		sigModified(*this);
	}

}
