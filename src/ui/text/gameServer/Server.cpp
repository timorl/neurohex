#include"ui/text/gameServer/Server.hpp"

namespace ui {
namespace text {
namespace gameServer {

	void serverStateMonitor(const neuroServer::Server & srvr) {
	}

	void Server::serverShell(const neuroServer::Server & srvr) {
		std::cout << "Welcome to the Neurohex game server!" << std::endl;
		CommandShell shell("neuroServer");
		shell.addCommand("connections", "Set the maximum number of connections to accept.",
				[this](Arguments args)->bool {
					if ( args.size() > 1 ) {
						server.setMaxConnections(std::stoi(args[1]));
					} else {
						std::cout << "Currently at most " << server.getMaxConnections() << " simultaneous connections will be accepted.\n";
					}
					return false;
				});
		shell.addCommand("port", "Set the port on which to listen.",
				[this](Arguments args)->bool {
					if ( args.size() > 1 ) {
						server.setPort(std::stoi(args[1]));
					} else {
						std::cout << "We will listen at port number " << server.getPort() << ".\n";
					}
					return false;
				});
		shell.addCommand("start", "Starts the server with the current settings.",
				[this](Arguments args)->bool {
					server.sigModified.disconnect_all_slots();
					server.sigModified.connect(serverStateMonitor);
					server.start();
					return true;
				});
		shell.run();
	}

	Server::Server(neuroServer::Server & server) :
		server(server) {
		server.sigModified.connect(
				[this](const neuroServer::Server & srvr) {
					serverShell(srvr);
				});
	}

}
}
}
