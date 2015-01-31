#include"network/Connection.hpp"
#include"iostream"

using boost::asio::ip::tcp;

namespace network {

	const std::string Connection::delimiter("\r\n\r\n");
	boost::asio::io_service Connection::io_service;
	boost::asio::io_service::work Connection::work(Connection::io_service);
	std::shared_ptr<std::thread> Connection::netThread;

	Connection::Connection(SocketP sockPointer) : sockPointer(sockPointer), dreamLevel(0) {}

	Connection::~Connection() {
		close();
	}

	bool Connection::isClosed() const {
		return !sockPointer->is_open();
	}

	bool Connection::setResponseHandler(ResponseHandler handler) {
		ULock lk(mtx);
		if (curHandler) {
			return false;
		}

		curHandler = handler;
		boost::asio::async_read_until(*sockPointer, buffer, delimiter, std::bind(&Connection::execResponseHandler, this, std::placeholders::_1, std::placeholders::_2));
		return true;
	}

	bool Connection::sendMessage(std:: string message, ResponseHandler handler) {
		ULock lk(mtx);
		if (curHandler) {
			return false;
		}

		message += delimiter;
		curHandler = handler;
		sockPointer->async_send(boost::asio::buffer(message), std::bind(&Connection::sendHandler, this,  std::placeholders::_1, std::placeholders::_2, handler));
		return true;
	}

	void Connection::sendHandler(const boost::system::error_code& err, std::size_t bytes_transferred, ResponseHandler handler){
		if(handler){
			ULock lk(mtx);
			if (!err){
				boost::asio::async_read_until(*sockPointer, buffer, delimiter, std::bind(&Connection::execResponseHandler, this, std::placeholders::_1, std::placeholders::_2));
			}
			else {
				ResponseHandler handler = curHandler;
				curHandler = ResponseHandler();
				handler("");

			}
		}
		cv.notify_one();
	}

	void Connection::execResponseHandler(const boost::system::error_code& err, std::size_t bytes_transferred) {
		ULock lk(mtx);
		if (!err){
			std::istream is(&buffer);
			std::string message, grbg;
			std::getline(is, message, '\r');
			std::getline(is, grbg);
			std::getline(is, grbg);
			ResponseHandler handler = curHandler;
			curHandler = ResponseHandler();
			handler(message);
		} else {
			ResponseHandler handler = curHandler;
			curHandler = ResponseHandler();
			handler("");
		}

		cv.notify_one();
	}

	void Connection::wait() {
		ULock lk(mtx);
		if (curHandler) {
			return;
		}

		cv.wait(lk, [this]()->bool{return !curHandler;});
		return;
	}

	void Connection::close() {
		sockPointer->close();
	}

	std::shared_ptr<Connection> Connection::connectTo(std::string address, std::string portNumber) {
		tcp::resolver resolver(Connection::io_service);
		tcp::resolver::query query(address, portNumber);
		SocketP sockPointer(new tcp::socket(Connection::io_service));
		try{
			auto endpts = resolver.resolve(query);
			boost::asio::connect(*sockPointer, endpts);
		}
		catch (std::exception& e){
			sockPointer->close();
			//TO DO
		}

		return std::shared_ptr<Connection>(new Connection(sockPointer));
	}

	void Connection::runIOservice(){
		try{
			Connection::io_service.run();
		}
 		catch (std::exception& e){
			//std::cerr << e.what() << std::endl;
		}
	}

	void Connection::runAll(){
		if ( !netThread ) {
			netThread.reset(new std::thread(Connection::runIOservice));
		}
	}

}
