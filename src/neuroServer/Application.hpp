#ifndef NEUROSERVER_APPLICATION_HPP
#define NEUROSERVER_APPLICATION_HPP

#include<memory>
#include"ui/Observable.hpp"
#include"utility/Environment.hpp"
#include"neuroServer/BoardLoader.hpp"
#include"neuroServer/ArmyLoader.hpp"
#include"neuroServer/Server.hpp"

namespace neuroServer {

	/**
		* @brief The class to start the whole application.
		*/
	class Application : public ui::Observable<Application> {
		public:
			/**
				* @brief Create a new application object.
				*/
			Application() : server(boardLoader, armyLoader) {}

			/**
				* @brief Loads resources.
				*/
			void start();

			/**
				* @brief Returns the BoardLoader used.
				*/
			BoardLoader & getBoardLoader() {	return boardLoader;	}

			/**
				* @brief Returns the ArmyLoader used.
				*/
			ArmyLoader & getArmyLoader() {	return armyLoader;	}

			/**
				* @brief Returns a reference to the game server pointer.
				*/
			Server & getServer() { return server; }
		private:
			BoardLoader boardLoader;
			ArmyLoader armyLoader;
			Server server;
	};

}

#endif
