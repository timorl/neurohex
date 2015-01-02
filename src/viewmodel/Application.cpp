#include"Application.hpp"

namespace viewmodel {

	std::string Application::globalDataDirectory = "";
	std::string Application::localDataDirectory = "";

	void Application::start() {
		std::string boardSubdirectory("/boards");
		boardLoader.loadBoards(globalDataDirectory + boardSubdirectory);
		boardLoader.loadBoards(localDataDirectory + boardSubdirectory);
		std::string armySubdirectory("/armies");
		armyLoader.loadArmies(globalDataDirectory + armySubdirectory);
		armyLoader.loadArmies(localDataDirectory + armySubdirectory);
		sigModified(*this);
	}

}
