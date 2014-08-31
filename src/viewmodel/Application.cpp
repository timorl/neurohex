#include"Application.hpp"

namespace viewmodel {

	std::string Application::globalDataDirectory = "";
	std::string Application::localDataDirectory = "";

	void Application::start() {
		std::string boardSubdirectory("/boards");
		boardLoader = BoardLoaderP( new BoardLoader() );
		boardLoader->loadBoards(globalDataDirectory + boardSubdirectory);
		boardLoader->loadBoards(localDataDirectory + boardSubdirectory);
		std::string armySubdirectory("/armies");
		armyLoader = ArmyLoaderP( new ArmyLoader() );
		armyLoader->loadArmies(globalDataDirectory + armySubdirectory);
		armyLoader->loadArmies(localDataDirectory + armySubdirectory);
		mainMenu = MainMenuP( new MainMenu( boardLoader, armyLoader ) );
		sigModified(*this);
	}

}
