#include"neuroServer/NetworkContestant.hpp"

namespace neuroServer {

	void NetworkContestant::requestNick() {
		nickRequested	= RequestState::IN_PROGRESS;
		while ( !contestant->setResponseHandler([this](std::string resp) {
					nick = resp;
					nickRequested	= RequestState::DONE;
				}) ) {
			if ( contestant->isClosed() ) {
				requestContestant();
				return;
			} else {
				contestant->wait();
			}
		}
	}

	void NetworkContestant::requestContestant() {
		do {
			contestant = acceptor.getNextConnection();
		} while (contestant->isClosed());
		requestNick();
	}

	bool NetworkContestant::validateContestant() {
		std::string contNick;
		while ( !contestant->setResponseHandler([&contNick](std::string resp) {
					contNick = resp;
				}) ) {
			if ( contestant->isClosed() ) {
				return false;
			} else {
				contestant->wait();
			}
		}
		contestant->wait();
		if ( contNick == nick ) {
			return true;
		} else {
			contestant->close();
			return false;
		}
	}

	void NetworkContestant::reestablishContestant() {
		if ( nickRequested == RequestState::NONE ) {
			requestContestant();
		} else if ( nickRequested == RequestState::IN_PROGRESS ) {
			return; //Already looking for someone.
		} else if ( nickRequested == RequestState::DONE ) {
			nickRequested	= RequestState::IN_PROGRESS;
			do {
				contestant = acceptor.getNextConnection();
			} while (contestant->isClosed() || !validateContestant());
		}
	}

	void NetworkContestant::initialHealthRequestResponseHandler(std::string resp) {
		initialHealthChoice = std::stoi(resp);
		initialHealthRequested	= RequestState::DONE;
	}

	void NetworkContestant::requestInitialHealthChoice() {
		const std::string initialHealthRequestMessage = "[REQUEST:initialHealth]";
		if ( initialHealthRequested == RequestState::DONE || initialHealthRequested == RequestState::IN_PROGRESS ) {
			return;
		}
		initialHealthRequested	= RequestState::IN_PROGRESS;
		while ( !contestant->sendMessage(initialHealthRequestMessage, std::bind(&NetworkContestant::initialHealthRequestResponseHandler, this, std::placeholders::_1)) ) {
			if ( contestant->isClosed() ) {
				reestablishContestant();
			} else {
				contestant->wait();
			}
		}
	}

	int NetworkContestant::getInitialHealthChoice() {
		requestInitialHealthChoice();
		while ( initialHealthRequested == RequestState::IN_PROGRESS ) {
			contestant->wait();
		}
		return initialHealthChoice;
	}

	void makeOptions(utility::DFStyleCreator rc, const std::map<std::string, std::string> & opt) {
		rc.startToken("OPTIONSBEGIN");
		rc.addToToken(opt.size());
		rc.endToken();
		for ( auto & op : opt ) {
			rc.startToken(op.first);
			rc.addToToken(op.second);
			rc.endToken();
		}
		rc.startToken("OPTIONSEND");
		rc.endToken();
	}

	void NetworkContestant::boardRequestResponseHandler(std::string resp) {
		board = resp;
		boardRequested	= RequestState::DONE;
	}

	void NetworkContestant::requestBoardChoice(const std::map<std::string, std::string> & boards) {
		if ( boardRequested == RequestState::DONE || boardRequested == RequestState::IN_PROGRESS ) {
			return;
		}
		boardRequested	= RequestState::IN_PROGRESS;
		std::ostringstream request;
		utility::DFStyleCreator	requestCreator(request);
		requestCreator.startToken("REQUEST");
		requestCreator.addToToken(std::string("board"));
		requestCreator.endToken();
		makeOptions(requestCreator, boards);
		std::string requestMessage = request.str();
		while ( !contestant->sendMessage(requestMessage, std::bind(&NetworkContestant::boardRequestResponseHandler, this, std::placeholders::_1)) ) {
			if ( contestant->isClosed() ) {
				reestablishContestant();
			} else {
				contestant->wait();
			}
		}
	}

	std::string NetworkContestant::getBoardChoice(const std::map<std::string, std::string> & boards) {
		requestBoardChoice(boards);
		while ( boardRequested == RequestState::IN_PROGRESS ) {
			contestant->wait();
		}
		return board;
	}

	void NetworkContestant::armyRequestResponseHandler(std::string resp) {
		army = resp;
		armyRequested	= RequestState::DONE;
	}

	void NetworkContestant::requestArmyChoice(const std::map<std::string, std::string> & armies) {
		if ( armyRequested == RequestState::DONE || armyRequested == RequestState::IN_PROGRESS ) {
			return;
		}
		armyRequested	= RequestState::IN_PROGRESS;
		std::ostringstream request;
		utility::DFStyleCreator	requestCreator(request);
		requestCreator.startToken("REQUEST");
		requestCreator.addToToken(std::string("army"));
		requestCreator.endToken();
		makeOptions(requestCreator, armies);
		std::string requestMessage = request.str();
		while ( !contestant->sendMessage(requestMessage, std::bind(&NetworkContestant::armyRequestResponseHandler, this, std::placeholders::_1)) ) {
			if ( contestant->isClosed() ) {
				reestablishContestant();
			} else {
				contestant->wait();
			}
		}
	}

	std::string NetworkContestant::getArmyChoice(const std::map<std::string, std::string> & armies) {
		requestArmyChoice(armies);
		while ( armyRequested == RequestState::IN_PROGRESS ) {
			contestant->wait();
		}
		return army;
	}

	void NetworkContestant::moveRequestResponseHandler(std::string resp) {
		std::istringstream inptStream(resp);
		utility::DFStyleReader reader(inptStream);
		move.fillFromDFStyle(reader);
		moveRequested	= RequestState::DONE;
	}

	void NetworkContestant::requestMove(int playerId, const neuro::Game & game) {
		if ( moveRequested == RequestState::DONE || moveRequested == RequestState::IN_PROGRESS ) {
			return;
		}
		moveRequested	= RequestState::IN_PROGRESS;
		std::ostringstream request;
		utility::DFStyleCreator	requestCreator(request);
		requestCreator.startToken("REQUEST");
		requestCreator.addToToken(std::string("move"));
		requestCreator.endToken();
		requestCreator.startToken("PLAYERID");
		requestCreator.addToToken(playerId);
		requestCreator.endToken();
		requestCreator.startToken("GAMEBEGIN");
		requestCreator.endToken();
		game.encodeAsDFStyle(requestCreator);
		std::string requestMessage = request.str();
		while ( !contestant->sendMessage(requestMessage, std::bind(&NetworkContestant::moveRequestResponseHandler, this, std::placeholders::_1)) ) {
			if ( contestant->isClosed() ) {
				reestablishContestant();
			} else {
				contestant->wait();
			}
		}
	}

	neuro::Move NetworkContestant::getMove(int playerId, const neuro::Game & game) {
		requestMove(playerId, game);
		while ( moveRequested == RequestState::IN_PROGRESS ) {
			contestant->wait();
		}
		moveRequested	= RequestState::NONE;
		return move;
	}

	void NetworkContestant::targetsRequestResponseHandler(std::string resp) {
		std::istringstream inptStream(resp);
		utility::DFStyleReader reader(inptStream);
		while ( reader.hasNextToken() ) {
			std::vector< std::string > info = reader.getNextToken();
			assert(info[0] == "TARGETSBEGIN");
			targets[std::stoi(info[1])].fillFromDFStyle(reader);
		}
		targetsRequested	= RequestState::DONE;
	}

	void NetworkContestant::requestTargets(int playerId, const neuro::Game & game, std::vector<neuro::AbilityIdentifier> abilities) {
		if ( targetsRequested == RequestState::DONE || targetsRequested == RequestState::IN_PROGRESS ) {
			return;
		}
		targetsRequested	= RequestState::IN_PROGRESS;
		targets.resize(abilities.size());
		std::ostringstream request;
		utility::DFStyleCreator	requestCreator(request);
		requestCreator.startToken("REQUEST");
		requestCreator.addToToken(std::string("targets"));
		requestCreator.addToToken(abilities.size());
		requestCreator.endToken();
		requestCreator.startToken("PLAYERID");
		requestCreator.addToToken(playerId);
		requestCreator.endToken();
		requestCreator.startToken("GAMEBEGIN");
		requestCreator.endToken();
		game.encodeAsDFStyle(requestCreator);
		for ( auto a : abilities ) {
			requestCreator.startToken("ABILITYIDENTIFIERBEGIN");
			requestCreator.endToken();
			a.encodeAsDFStyle(requestCreator);
		}
		std::string requestMessage = request.str();
		while ( !contestant->sendMessage(requestMessage, std::bind(&NetworkContestant::targetsRequestResponseHandler, this, std::placeholders::_1)) ) {
			if ( contestant->isClosed() ) {
				reestablishContestant();
			} else {
				contestant->wait();
			}
		}
	}

	neuro::Targets NetworkContestant::getTargets(int playerId, const neuro::Game & game, std::vector<neuro::AbilityIdentifier> abilities) {
		requestTargets(playerId, game, abilities);
		while ( targetsRequested == RequestState::IN_PROGRESS ) {
			contestant->wait();
		}
		targetsRequested	= RequestState::NONE;
		return targets;
	}

	void NetworkContestant::discardRequestResponseHandler(std::string resp) {
		discard = std::stoi(resp);
		discardRequested	= RequestState::DONE;
	}

	void NetworkContestant::requestDiscard(int playerId, const neuro::Game & game) {
		if ( discardRequested == RequestState::DONE || discardRequested == RequestState::IN_PROGRESS ) {
			return;
		}
		discardRequested	= RequestState::IN_PROGRESS;
		std::ostringstream request;
		utility::DFStyleCreator	requestCreator(request);
		requestCreator.startToken("REQUEST");
		requestCreator.addToToken(std::string("discard"));
		requestCreator.endToken();
		requestCreator.startToken("PLAYERID");
		requestCreator.addToToken(playerId);
		requestCreator.endToken();
		requestCreator.startToken("GAMEBEGIN");
		requestCreator.endToken();
		game.encodeAsDFStyle(requestCreator);
		std::string requestMessage = request.str();
		while ( !contestant->sendMessage(requestMessage, std::bind(&NetworkContestant::discardRequestResponseHandler, this, std::placeholders::_1)) ) {
			if ( contestant->isClosed() ) {
				reestablishContestant();
			} else {
				contestant->wait();
			}
		}
	}

	int NetworkContestant::getDiscard(int playerId, const neuro::Game & game) {
		requestDiscard(playerId, game);
		while ( discardRequested == RequestState::IN_PROGRESS ) {
			contestant->wait();
		}
		discardRequested	= RequestState::NONE;
		return discard;
	}

}
