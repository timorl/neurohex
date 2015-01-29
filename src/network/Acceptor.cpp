#include"network/Acceptor.hpp"

using boost::asio::ip::tcp;

namespace network {

	Acceptor::Acceptor(int portNumber) : portNumber(portNumber), waitingSockets(0) {}

	Acceptor::~Acceptor() {
	}

	std::shared_ptr<Connection> Acceptor::getNextConnection() {
		std::unique_lock<std::mutex> lk(mtx);
		if ( readySockets.empty() && waitingSockets == 0 ) {
			startAccepting(1);
		}
		cv.wait(lk, [this]{return !readySockets.empty();});
		SocketP rdySocket = readySockets.front();
		readySockets.pop();
		return std::shared_ptr<Connection>(new Connection(rdySocket));
	}

	void Acceptor::startAccepting(int max) {
		for(int i=0; i<max; i++){
			SocketP sockPointer(new tcp::socket(Connection::io_service));
			tcp::acceptor acceptor(Connection::io_service, tcp::endpoint(tcp::v4(), portNumber));

			waitingSockets++;
			acceptor.async_accept(*sockPointer, std::bind(&Acceptor::acceptHandler, this, std::placeholders::_1, sockPointer));
		}
	}

	void Acceptor::acceptHandler(const boost::system::error_code& error, SocketP sockPointer){
		std::unique_lock<std::mutex> lk(mtx);
		if (!error){
			readySockets.push(sockPointer);
			waitingSockets--;
			lk.unlock();
			cv.notify_one();
		}
		else{
			waitingSockets--;
			startAccepting(1);
		}
	}

}
