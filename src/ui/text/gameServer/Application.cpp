#include"ui/text/gameServer/Application.hpp"

namespace ui {
namespace text {
namespace gameServer {

	void finishedLoading(const neuroServer::Application & app) {
		std::cout << "Finished loading assets, setting up server..." << std::endl;
		app.sigModified.disconnect_all_slots();
	}

	Application::Application(neuroServer::Application & application) :
		application(application),
		armyLoader(application.getArmyLoader()),
		boardLoader(application.getBoardLoader()),
		server(application.getServer()) {
		application.sigModified.connect(finishedLoading);
	}

}
}
}
