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

	void Connection::wait() {
		//TODO: This is a stub.
	}

	void Connection::close() {
		//TODO: This is a stub.
	}

	Connection Connection::connectTo(std::string address) {
		//TODO: This is a stub.
		return Connection();
	}

}
