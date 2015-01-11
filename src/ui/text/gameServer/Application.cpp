#include"ui/text/gameServer/Application.hpp"

namespace ui {
namespace text {
namespace gameServer {


	void finishedLoading(const neuroServer::Application & app) {
		std::cout << "Finished loading assets." << std::endl;
		std::cout << "Welcome to the Neurohex game server!" << std::endl;
		CommandShell shell("neuroServer");
		int maxConnections = 10;
		int port = 14521; //"neu"
		shell.addCommand("connections", "Set the maximum number of connections to accept.",
				[&maxConnections](Arguments args)->bool {
					if ( args.size() > 1 ) {
						maxConnections = std::stoi(args[1]);
					} else {
						std::cout << "Currently at most " << maxConnections << " simultaneous connections will be accepted.\n";
					}
					return false;
				});
		shell.addCommand("port", "Set the port on which to listen.",
				[&port](Arguments args)->bool {
					if ( args.size() > 1 ) {
						port = std::stoi(args[1]);
					} else {
						std::cout << "We will listen at port number " << port << ".\n";
					}
					return false;
				});
		shell.addCommand("start", "Starts the server with the current settings.",
				[port, maxConnections](Arguments args)->bool {
					std::cout << "Not implemented.\n";
					return false;
				});
		shell.run();
	}

	Application::Application(neuroServer::Application & application) :
		application(application),
		armyLoader(application.getArmyLoader()),
		boardLoader(application.getBoardLoader()) {
		application.sigModified.connect(finishedLoading);
	}

}
}
}
