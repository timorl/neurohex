#include"neuroServer/Server.hpp"

namespace neuroServer {

	void Server::beginConfiguration() {
		sigModified(*this);
	}

	void Server::start() {
		network::Acceptor acceptor(port);
		acceptor.startAccepting(maxConnections);
		for ( int i = 0; i < maxConnections; i++ ) {
			contestants.emplace_back(new NetworkContestant(acceptor));
			NetworkContestantP & cont = contestants[i];
			cont->requestContestant();
			configurer.addContestant(cont);
		}
		do {
			for ( int i = 0; i < static_cast<int>(contestants.size()); i++ ) {
				NetworkContestantP & cont = contestants[i];
				cont->requestArmyChoice(configurer.getAvailableArmies());
			}
			configureGlobal();
			for ( int i = 0; i < static_cast<int>(contestants.size()); i++ ) {
				NetworkContestantP & cont = contestants[i];
				configurer.setArmy(i, cont->getArmyChoice(configurer.getAvailableArmies()));
			}
		} while ( !configurer.startGame() );
	}

	void Server::configureGlobal() {
		NetworkContestantP & admin = contestants[0];
		configurer.setBoard(admin->getBoardChoice(configurer.getAvailableBoards()));
		configurer.setInitialHealth(admin->getInitialHealthChoice());
	}

}
