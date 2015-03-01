#include<cstdlib>
#include<iostream>
#include"neuroServer/Application.hpp"
#include"ui/text/gameServer/Application.hpp"

int main() {
	utility::Environment::initialize();
	if ( !utility::Environment::isInitialized() ) {
		std::cerr << "Could not initialize environment variables, aborting!" << std::endl;
		return 1;
	}
	neuroServer::Application application;
	ui::text::gameServer::Application applicationUI(application);
	application.start();
	return 0;
}
