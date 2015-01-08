#include"ui/text/client/Application.hpp"

namespace ui {
namespace text {
namespace client {

	static const std::string helpMessage = "The available commands are:\n\tstart\tStarts a game. Not implemented.\n\texit\tExits this client.\n\thelp\tDisplays this help message.\n";

	void applicationStarted(neuroClient::Application const & application) {
		std::cout << "Welcome to the Neurohex game client!" << std::endl;
		while (true) {
			std::cout << "neuroClient:";
			std::string command;
			std::cin >> command;
			if ( command == "exit" ) {
				std::cout << "Bye!" << std::endl;
				break;
			} else if ( command == "start" ) {
				//This is actually a placeholder, not a very smart one at that.
				std::cout << "This would start the game if implemented." << std::endl;
			} else if ( command == "help" ) {
				std::cout << helpMessage;
			} else {
				std::cout << command << ": no such command." << std::endl;
				std::cout << helpMessage;
			}
		}
	}

	Application::Application(neuroClient::Application & application) : application(application) {
		application.sigModified.connect(applicationStarted);
	}

}
}
}
