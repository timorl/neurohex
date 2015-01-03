#include"neuroClient/Application.hpp"

namespace neuroClient {

	std::string Application::globalDataDirectory = "";
	std::string Application::localDataDirectory = "";

	void Application::start() {
		sigModified(*this);
	}

}
