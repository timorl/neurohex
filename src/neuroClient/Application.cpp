#include"neuroClient/Application.hpp"
#include<cassert>
#include"utility/DFStyleReader.hpp"

namespace neuroClient {

	std::string Application::globalDataDirectory = "";
	std::string Application::localDataDirectory = "";

	void Application::start() {
		sigModified(*this);
	}

	void Application::requestHandler(std::string request) {
		some_text = request;

		std::istringstream inptStream(request);
		utility::DFStyleReader reader(inptStream);
		std::vector< std::string > message = reader.getNextToken();

		assert(message[0] == "REQUEST");

		if ( message[1] == "initialHealth" ) {
			currentRequest = INITIAL_HEALTH;
		} else if ( message[1] == "board") {
			currentRequest = BOARD_CHOICE;
		} else if ( message[1] == "army") {
			currentRequest = ARMY_CHOICE;
		} else if ( message[1] == "move") {
			currentRequest = MOVE;
		} else if ( message[1] == "targets") {
			currentRequest = TARGETS;
		} else if ( message[1] == "discard") {
			currentRequest = DISCARD;
		}

		// This is a stub
		while(reader.hasNextToken()) {
			message = reader.getNextToken();
			if ( message[0] == "OPTIONSBEGIN" ) {
			} else if ( message[0] == "PLAYERID" ) {
			} else if ( message[0] == "GAMEBEGIN" ) {
			} else if ( message[0] == "ABILITYIDENTIFIERBEGIN" ) {
			}
		}

		// Set reposponse handler.
		sigModified(*this);
	}

	bool Application::writeToServer(std::string message) {
		if ( !lineToServer->isClosed() ) {
			lineToServer->sendMessage(message, std::bind(&Application::requestHandler, this, std::placeholders::_1));
			return true;
		} else {
			// Line, is dead, can't send anything so just return false;
			return false;
		}
	}

	bool Application::joinServer(std::string address, std::string portNumber, std::string username) {
		network::Connection::runAll();
		lineToServer = network::Connection::connectTo(address, portNumber);
		if( !lineToServer->isClosed() ) {
			lineToServer->sendMessage(username, std::bind(&Application::requestHandler, this, std::placeholders::_1));
			return true;
		} else {
			// Line, is dead, can't send anything so just return false;
			return false;
		}
	}

}
