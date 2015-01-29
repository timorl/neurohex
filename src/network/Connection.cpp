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
			sockPointer->async_receive(boost::asio::buffer(buffer, BUF_SIZE), std::bind(&Connection::execResponseHandler, this,  std::placeholders::_1, std::placeholders::_2));
			return true;
		}
		else return false;
	}

	bool Connection::sendMessage(std:: string message, ResponseHandler handler) {
		if(mutex.try_lock()){
			curHandler = handler;
			sockPointer->async_send(boost::asio::buffer(message), std::bind(&Connection::handle_send, this,  std::placeholders::_1, std::placeholders::_2));
			return true;
		}
		else return false;
	}

	void Connection::handle_send(const boost::system::error_code& err, std::size_t bytes_transferred){
		if (!err){
		}
		else {
			curHandler("");
		}

		curHandler = ResponseHandler();
		mutex.unlock();
	}

	void Connection::execResponseHandler(const boost::system::error_code& err, std::size_t bytes_transferred) {
		if (!err){
			curHandler(std::string(buffer));
		}
		else {
			curHandler("");
		}

		curHandler = ResponseHandler();
		mutex.unlock();
	}

	void Connection::wait() {
		//mutex.lock();
		//mutex.unlock();
	}

	void Connection::close() {
		sockPointer->close();
	}

	std::shared_ptr<Connection> Connection::connectTo(std::string address, std::string portNumber) {
		tcp::resolver resolver(Connection::io_service);
		tcp::resolver::query query(address, portNumber);
		SocketP sockPointer(new tcp::socket(Connection::io_service));
		auto endpts = resolver.resolve(query);
		boost::asio::connect(*sockPointer, endpts);

		return std::shared_ptr<Connection>(new Connection(sockPointer));
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
