#include"network/Connection.hpp"

namespace network {

	Connection::~Connection() {
		//TODO: This is a stub.
	}

	bool Connection::isClosed() const {
		//TODO: This is a stub.
		return false;
	}

	std::string Connection::nextMessage() {
		//TODO: This is a stub.
		return "";
	}

	void Connection::sendMessage(std::string message) {
		//TODO: This is a stub.
	}

	Connection Connection::connectTo(std::string address) {
		//TODO: This is a stub.
		return Connection();
	}

}
