#include"neuroClient/Application.hpp"
#include<iostream> //delete in finall release

namespace neuroClient {

	std::string Application::globalDataDirectory = "";
	std::string Application::localDataDirectory = "";

	void Application::start() {
		sigModified(*this);
	}

    bool Application::joinServer(std::string address,
            std::string portNumber, std::string username) {
        //lineToServer = (network::Connection::connectTo(address, portNumber));
        lineToServer = std::shared_ptr< network::Connection >(
                std::move( network::Connection::connectTo(address, portNumber) )
            );
        if( !lineToServer->isClosed() ) {
            lineToServer->sendMessage(username, [](std::string text)->std::string{return text;});
            return true;
        } else {
            // Line, is dead, can't send anything so just return false;
            return false;
        }
    }

    bool Application::sendAndDump(std::string message) {
        lineToServer->sendMessage(message, [](std::string text)->bool{std::cerr << text << std::endl;});
    }
}
