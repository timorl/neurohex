#include"ui/text/client/Application.hpp"

namespace ui {
namespace text {
namespace client {

	static const std::string helpMessage = "The available commands are:\n\tjoin\tAsks for necesary data and connects to the server.\n\tstart\tStarts a game. Not implemented.\n\texit\tExits this client.\n\thelp\tDisplays this help message.\n";
    
    void Application::requestHandler() {
        // This is a stub, should handle the request though
        std::cout << application.some_text << std::endl;
    }

	void Application::applicationStarted(const neuroClient::Application & app) {
        application.sigModified.disconnect_all_slots();
        ServerJoiner(application).run();
        application.sigModified.connect(std::bind(&Application::requestHandler, this));
        GameConfigurer(application).run();
	}

	Application::Application(neuroClient::Application & application) : application(application) {
        application.sigModified.connect(std::bind(&Application::applicationStarted, this, std::placeholders::_1));
	}

}
}
}
