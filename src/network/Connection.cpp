#include"network/Connection.hpp"

namespace network {

	Connection::~Connection() {
		//TODO: This is a stub.
	}

	bool Connection::isClosed() const {
		//TODO: This is a stub.
		return false;
	}

	bool Connection::setResponseHandler(ResponseHandler handler) {
		//TODO: This is a stub.
		return false;
	}

	bool Connection::sendMessage(std::string message, ResponseHandler handler) {
		//TODO: This is a stub.
		return false;
	}

	Connection Connection::connectTo(std::string address) {
		//TODO: This is a stub.
		return Connection();
	}

}
