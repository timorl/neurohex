#include"network/Connection.hpp"
#include<thread>

using boost::asio::ip::tcp;

namespace network {
	boost::asio::io_service Connection::io_service;

	Connection::Connection(SocketP sockPointer){
		this->sockPointer = sockPointer;
		this->handlerPointer = NULL;
		//this->mutexPointer = mutexPointer(new std::mutex());
	}

	Connection::~Connection() {
		close();
	}

	bool Connection::isClosed() const {
		return !(*sockPointer.get()).is_open();
	}

	bool Connection::setResponseHandler(ResponseHandler handler) {
		if((*mutexPointer.get()).try_lock()){
			
			(*mutexPointer.get()).unlock();
		}
		else return false;
	}

	bool Connection::sendMessage(std::string message, ResponseHandler handler) {
		if((*mutexPointer.get()).try_lock()){
			
			(*mutexPointer.get()).unlock();
		}
		else return false;
	}

	void Connection::wait() {
		(*mutexPointer.get()).lock();
		(*mutexPointer.get()).unlock();
	}

	void Connection::close() {
		(*sockPointer.get()).close();
	}

	Connection Connection::connectTo(std::string address, std::string portNumber) {
		tcp::resolver resolver(Connection::io_service);
		tcp::resolver::query query(address, portNumber);
		SocketP sockPointer(new tcp::socket(Connection::io_service));
		resolver.async_resolve(query, boost::bind(Connection::handle_resolve, boost::asio::placeholders::error, boost::asio::placeholders::iterator, sockPointer));

		return Connection(sockPointer);
	}

	void Connection::handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator, SocketP sockPointer){
		if (!err){
      			// Attempt a connection to the first endpoint in the list. Each endpoint
      			// will be tried until we successfully establish a connection.
      			tcp::endpoint endpoint = *endpoint_iterator;
			(*sockPointer.get()).async_connect(endpoint, boost::bind(Connection::handle_connect, boost::asio::placeholders::error, ++endpoint_iterator, sockPointer));
    		}
		else{
		      std::cout << "Error: " << err.message() << "\n";
		}
	}

	void Connection::handle_connect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator, SocketP sockPointer){
		if (!err){
			// The connection was successful.
		}
		else if (endpoint_iterator != tcp::resolver::iterator()){
			// The connection failed. Try the next endpoint in the list.
			(*sockPointer.get()).close();
			tcp::endpoint endpoint = *endpoint_iterator;
			(*sockPointer.get()).async_connect(endpoint, boost::bind(Connection::handle_connect, boost::asio::placeholders::error, ++endpoint_iterator, sockPointer));
		}
		else{
			std::cout << "Error: " << err.message() << "\n";
		}
	}

	void Connection::runIO_service(){
		try{
			Connection::io_service.run();
		}
 		catch (std::exception& e){
			std::cerr << e.what() << std::endl;
		}
	}

	void Connection::runAll(){
		std::thread thread(Connection::runIO_service);
	}
}
