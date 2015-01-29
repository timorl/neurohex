#include"ui/text/client/Application.hpp"

namespace ui {
namespace text {
namespace client {

	static const std::string helpMessage = "The available commands are:\n\tstart\tStarts a game. Not implemented.\n\texit\tExits this client.\n\thelp\tDisplays this help message.\n";

	void Application::applicationStarted(const neuroClient::Application & app) {
        std::string address, port, username;
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
            } else if ( command == "join" ) {
                std::cin >> address;
                std::cin >> port;
                std::cin >> username;
                bool tmp = application.joinServer(address, port, username);
                if ( tmp ) {
                    std::cout << "Connection successful." << std::endl;
                } else {
                    std::cout << "Connection NOT successful!" << std::endl;
                }
			} else {
				std::cout << command << ": no such command." << std::endl;
				std::cout << helpMessage;
			}
		}
	}

	Application::Application(neuroClient::Application & application) : application(application) {
        application.sigModified.connect(std::bind(&Application::applicationStarted, this, std::placeholders::_1));
	}

}
}
}
