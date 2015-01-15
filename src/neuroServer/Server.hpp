#ifndef NEUROSERVER_SERVER_HPP
#define NEUROSERVER_SERVER_HPP

#include<memory>
#include"ui/Observable.hpp"
#include"network/Acceptor.hpp"
#include"neuroServer/BoardLoader.hpp"
#include"neuroServer/ArmyLoader.hpp"
#include"neuroServer/Configurer.hpp"

namespace neuroServer {

	/**
		* @brief Accepts incoming connections, handles them, configures the game and
		* starts it.
		*/
	class Server : public ui::Observable<Server> {
		public:
			/**
				* @brief Create a new server.
				* @param[in] boardLoader A reference to the BoardLoader that has loaded
				* available boards.
				* @param[in] armyLoader A reference to the ArmyLoader that has loaded
				* available armies.
				* @param[in] port The port on which the server should be started. Default is
				* 14521, the numerization of "neu".
				* @param[in] maxConnections The maximal number of simultaneous connections
				* the server should accept. Default is 10.
				*/
			Server(BoardLoader & boardLoader, ArmyLoader & armyLoader, int port = 14521, int maxConnections = 10) : configurer(boardLoader, armyLoader), port(port), maxConnections(maxConnections) {}

			/**
				* @brief Start the configuration of the server.
				*/
			void beginConfiguration();

			/**
				* @brief Starts the server with the current settings.
				*/
			void start();

			/**
				* @brief Returns a reference to the game configurer.
				*/
			Configurer & getConfigurer() { return configurer; }

			/**
				* @brief Returns the maximum number of connections this server will accept.
				*/
			int getMaxConnections() const { return maxConnections; }

			/**
				* @brief Sets the maximum number of connections this server will accept.
				*/
			void setMaxConnections(int mc) { maxConnections = mc; }

			/**
				* @brief Returns the port on which this server will accept connections.
				*/
			int getPort() const { return port; }

			/**
				* @brief Sets the port on which this server will listen.
				*/
			void setPort(int p) { port = p; }
		private:
			Configurer configurer;
			int port;
			int maxConnections;
	};

}

#endif
