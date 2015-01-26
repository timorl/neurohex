#include"network/Acceptor.hpp"

using boost::asio::ip::tcp;

namespace network {

	Acceptor::Acceptor(int portNumber) {
		this -> portNumber = portNumber;
		firstUnusedSocket = 0;
	}

	Acceptor::~Acceptor() {
	}

	Connection Acceptor::getNextConnection() {
		startAccepting(1);
		return Connection(arraySocketsP[firstUnusedSocket]);
	}
	
	void Acceptor::accept_handler(const boost::system::error_code& error, SocketP sockPointer){
		if (!error){
			arraySocketsP.push_back(sockPointer);
		}

		//startAccepting(1);
	}
	void Acceptor::startAccepting(int max) {
		for(int i=0; i<max; i++){		
			SocketP sockPointer(new tcp::socket(Connection::io_service));
			tcp::acceptor acceptor(Connection::io_service, tcp::endpoint(tcp::v4(), portNumber));

			acceptor.async_accept(*(sockPointer.get()), boost::bind(&Acceptor::accept_handler, this, boost::asio::placeholders::error, sockPointer));
		}
	}
}
