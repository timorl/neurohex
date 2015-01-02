#include"viewmodel/MainMenu.hpp"

namespace viewmodel {

	void MainMenu::createGame() {
		event = MenuEvent::CREATE_GAME;
		sigModified(*this);
	}

}
