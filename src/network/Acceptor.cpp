#include"network/Acceptor.hpp"

using boost::asio::ip::tcp;

namespace network {

	Acceptor::Acceptor(int portNumber) {
		this -> portNumber = portNumber;
		firstUnusedSocket = 0;
	}

	Acceptor::~Acceptor() {
	}

	std::shared_ptr<Connection> Acceptor::getNextConnection() {
		if (firstUnusedSocket == arraySocketsP.size()) startAccepting(1);
		arrayMutex[firstUnusedSocket].lock();
		arrayMutex[firstUnusedSocket].unlock();
		return std::shared_ptr<Connection>(new Connection(arraySocketsP[firstUnusedSocket++]));
	}
	
	void Acceptor::accept_handler(const boost::system::error_code& error, SocketP sockPointer){
		if (!error){
			arraySocketsP.push_back(sockPointer);
			arrayMutex[arraySocketsP.size()-1].unlock();
		}
		else{
			arrayMutex[arraySocketsP.size()].unlock();
			startAccepting(1);
		}
	}
	void Acceptor::startAccepting(int max) {
		for(int i=0; i<max; i++){
			arrayMutex.push_back(new boost::mutex);
			arrayMutex.back().lock();
			SocketP sockPointer(new tcp::socket(Connection::io_service));
			tcp::acceptor acceptor(Connection::io_service, tcp::endpoint(tcp::v4(), portNumber));

			acceptor.async_accept(*(sockPointer.get()), boost::bind(&Acceptor::accept_handler, this, boost::asio::placeholders::error, sockPointer));
		}
	}
}
