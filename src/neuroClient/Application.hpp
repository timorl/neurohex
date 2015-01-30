#ifndef NEUROCLIENT_APPLICATION_HPP
#define NEUROCLIENT_APPLICATION_HPP

#include<memory>
#include"ui/Observable.hpp"
#include"network/Connection.hpp"

namespace neuroClient {

    enum requestType {
        INITIAL_HEALTH,
        BOARD_CHOICE,
        ARMY_CHOICE,
        MOVE,
        TARGETS,
        DISCARD,
    };

	/**
		* @brief The class to start the whole application.
		* @todo This is a stub.
		*/
	class Application : public ui::Observable<Application> {
		public:
			/**
				* @brief Loads resources.
				* @todo This is a stub.
				*/
			void start();

			/**
				* @brief The global directory containing all the application data.
				*/
			static std::string globalDataDirectory;

			/**
				* @brief The local directory containing the application data changed by the
				* user.
				*/
			static std::string localDataDirectory;

			/**
				* @brief Writes the message to the server.
				*/
			bool writeToServer(std::string message);

			/**
				* @brief Command to join server and send it your username.
				*/
			bool joinServer(std::string address, std::string portNumber, std::string username);

			/**
				* @brief Handles request from the server.
				* @todo Fill gamestate from requests.
				*/
			void requestHandler(std::string request);

            // This is a temporary variable for testing.
            std::string some_text;

			/**
				* @brief Information about the type of the last request
                * received from the server.
				*/
            requestType currentRequest;
        private:
            std::shared_ptr< network::Connection > lineToServer;
	};

}

#endif
