#ifndef UI_TEXT_GAMESERVER_APPLICATION_HPP
#define UI_TEXT_GAMESERVER_APPLICATION_HPP

#include<iostream>
#include"ui/Observable.hpp"
#include"neuroServer/Application.hpp"
#include"ui/text/CommandShell.hpp"
#include"ui/text/gameServer/Server.hpp"
#include"ui/text/gameServer/ArmyLoader.hpp"
#include"ui/text/gameServer/BoardLoader.hpp"

namespace ui {
namespace text {
namespace gameServer {

	/**
		* @brief The class to start the text ui for the server.
		*/
	class Application {
		public:
			/**
				* @brief Create an application watching the given application.
				*/
			Application(neuroServer::Application & application);
		private:
			neuroServer::Application & application;
			ArmyLoader armyLoader;
			BoardLoader boardLoader;
			Server server;
	};

}
}
}

#endif
