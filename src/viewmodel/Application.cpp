#include"Application.hpp"

namespace viewmodel {

	void Application::start() {
		std::string programDirectory("/neurohex");
		std::string globalDataDirectories( std::getenv("XDG_DATA_DIRS") );
		std::string globalDataDirectory = globalDataDirectories.substr( 0, globalDataDirectories.find_first_of(':') ) + programDirectory;
		std::string localDataDirectories( std::getenv("XDG_DATA_HOME") );
		std::string localDataDirectory = localDataDirectories.substr( 0, localDataDirectories.find_first_of(':') ) + programDirectory;
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
