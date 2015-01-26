#ifndef NEUROCLIENT_APPLICATION_HPP
#define NEUROCLIENT_APPLICATION_HPP

#include<memory>
#include"ui/Observable.hpp"
#include"network/Connection.hpp"

namespace neuroClient {

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
				* @brief Command to join server and send it your username.
                * @todo Change the handler which this function sets
				*/
            bool joinServer(std::string address, std::string portNumber, std::string username);

			/**
				* @brief Sends and message to server and sets handler to dump
                * everything got from server on stderr.
                * @todo Delete in final release.
				*/
            bool sendAndDump(std::string message);
		private:
            std::shared_ptr< network::Connection > lineToServer;
	};

}

#endif
