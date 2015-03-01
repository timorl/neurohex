#include"neuroServer/Application.hpp"

namespace neuroServer {

	void Application::start() {
		std::string boardSubdirectory("boards");
		boardLoader.loadBoards(utility::Environment::getGlobalResourcePath() + boardSubdirectory);
		boardLoader.loadBoards(utility::Environment::getPreferencesPath() + boardSubdirectory);
		std::string armySubdirectory("armies");
		armyLoader.loadArmies(utility::Environment::getGlobalResourcePath() + armySubdirectory);
		armyLoader.loadArmies(utility::Environment::getPreferencesPath() + armySubdirectory);
		sigModified(*this);
		server.beginConfiguration();
		sigModified(*this);
	}

}
