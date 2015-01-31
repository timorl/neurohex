#include"ui/text/client/Application.hpp"

namespace ui {
namespace text {
namespace client {

	void Application::requestHandler(const neuroClient::Application & app) {
		// This is a stub, should handle the request though
		std::cout << application.some_text << std::endl;
	}

	void Application::applicationStarted(const neuroClient::Application & app) {
		application.sigModified.disconnect_all_slots();
		application.sigModified.connect(std::bind(&Application::requestHandler, this, std::placeholders::_1));
		ServerJoiner(application).run();
		GameConfigurer(application).run();
	}

	Application::Application(neuroClient::Application & application) : application(application) {
		application.sigModified.connect(std::bind(&Application::applicationStarted, this, std::placeholders::_1));
	}

}
}
}
