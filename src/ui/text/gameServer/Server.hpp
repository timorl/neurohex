#ifndef UI_TEXT_GAMESERVER_SERVER_HPP
#define UI_TEXT_GAMESERVER_SERVER_HPP

#include<iostream>
#include"ui/Observable.hpp"
#include"neuroServer/Server.hpp"
#include"ui/text/CommandShell.hpp"

namespace ui {
namespace text {
namespace gameServer {

	/**
		* @brief The ui for the server.
		*/
	class Server {
		public:
			/**
				* @brief Create a serve watching the given server.
				*/
			Server(neuroServer::Server & srvr);
		private:
			neuroServer::Server & server;

			void serverShell(const neuroServer::Server & srvr);
	};

}
}
}

#endif
