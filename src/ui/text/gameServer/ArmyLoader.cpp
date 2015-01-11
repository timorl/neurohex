#include"ui/text/gameServer/ArmyLoader.hpp"

namespace ui {
namespace text {
namespace gameServer {

	void armyLoaded(const neuroServer::ArmyLoader & al) {
		const std::string armyLoadedMessage = "Currently loaded armies: ";
		std::cout << armyLoadedMessage << al.getDescriptions().size() << std::endl;
	}

	ArmyLoader::ArmyLoader(neuroServer::ArmyLoader & armyLoader) :
		armyLoader(armyLoader) {
		armyLoader.sigModified.connect(armyLoaded);
	}

}
}
}
