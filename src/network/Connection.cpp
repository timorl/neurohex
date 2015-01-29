#include"network/Connection.hpp"

using boost::asio::ip::tcp;

namespace network {
	boost::asio::io_service Connection::io_service;
	boost::asio::io_service::work Connection::work(Connection::io_service);
	std::shared_ptr<std::thread> Connection::netThread;

	Connection::Connection(SocketP sockPointer) : sockPointer(sockPointer) {}

	Connection::~Connection() {
		close();
	}

	bool Connection::isClosed() const {
		return !sockPointer->is_open();
	}

	bool Connection::setResponseHandler(ResponseHandler handler) {
		if(mutex.try_lock()){
			curHandler = handler;
			//sockPointer->async_receive(boost::asio::buffer(buffer, 1024), boost::bind(&Connection::execResponseHandler, this,  boost::asio::placeholders::error, 1024));
			return true;
		}
		else return false;
	}

	bool Connection::sendMessage(std:: string message, ResponseHandler handler) {
		if(mutex.try_lock()){
			curHandler = handler;
			sockPointer->async_send(boost::asio::buffer(message, 1024), boost::bind(&Connection::handle_send, this,  boost::asio::placeholders::error, 1024));
			return true;
		}
		else return false;
	}

	void Connection::handle_send(const boost::system::error_code& err, std::size_t bytes_transferred){
		if (!err){
		}
		else {
			curHandler = ResponseHandler();
			mutex.unlock();
		}
	}

	void Connection::execResponseHandler(const boost::system::error_code& err, std::size_t bytes_transferred) {
		if (!err){
			curHandler(buffer);
		}
		else {
		}

		curHandler = ResponseHandler();
		mutex.unlock();
	}

	void Connection::wait() {
		mutex.lock();
		mutex.unlock();
	}

	void Connection::close() {
		sockPointer->close();
	}

	std::shared_ptr<Connection> Connection::connectTo(std::string address, std::string portNumber) {
		tcp::resolver resolver(Connection::io_service);
		tcp::resolver::query query(address, portNumber);
		SocketP sockPointer(new tcp::socket(Connection::io_service));
		resolver.async_resolve(query, boost::bind(Connection::handle_resolve, boost::asio::placeholders::error, boost::asio::placeholders::iterator, sockPointer));

		return std::shared_ptr<Connection>(new Connection(sockPointer));
	}

	void Connection::handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator, SocketP sockPointer){
		if (!err){
      			// Attempt a connection to the first endpoint in the list. Each endpoint
      			// will be tried until we successfully establish a connection.
      			tcp::endpoint endpoint = *endpoint_iterator;
			sockPointer->async_connect(endpoint, boost::bind(Connection::handle_connect, boost::asio::placeholders::error, ++endpoint_iterator, sockPointer));
    		}
		else{
		      //std::cout << "Error: " << err.message() << "\n";
		}
	}

	void Connection::handle_connect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator, SocketP sockPointer){
		if (!err){
			// The connection was successful.
		}
		else if (endpoint_iterator != tcp::resolver::iterator()){
			// The connection failed. Try the next endpoint in the list.
			sockPointer->close();
			tcp::endpoint endpoint = *endpoint_iterator;
			sockPointer->async_connect(endpoint, boost::bind(Connection::handle_connect, boost::asio::placeholders::error, ++endpoint_iterator, sockPointer));
		}
		else{
			//std::cout << "Error: " << err.message() << "\n";
		}
	}

	void Connection::runIO_service(){
		try{
			Connection::io_service.run();
		}
 		catch (std::exception& e){
			//std::cerr << e.what() << std::endl;
		}
	}

	void Connection::runAll(){
		if ( !netThread ) {
			netThread.reset(new std::thread(Connection::runIO_service));
		}
	}
}
