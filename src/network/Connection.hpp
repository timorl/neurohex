#ifndef NETWORK_CONNECTION_HPP
#define NETWORK_CONNECTION_HPP

#include<string>
#include<functional>
#include<boost/asio.hpp>
#include<memory>
#include<mutex>
#include<string>
#include<thread>
#include<condition_variable>
#include<atomic>

using boost::asio::ip::tcp;

namespace network {

	using ResponseHandler = std::function<void(std::string)>;
	using SocketP = std::shared_ptr<tcp::socket>;
	using ULock = std::unique_lock<std::recursive_mutex>;

	/**
		* @brief A representation of a single network connection to which you can write
		* and from which you can read.
		* @todo Add a constructor for creating accepting connections.
		* @todo Implement all the functions.
		*/
	class Connection {
		public:
			/**
				* @brief Takes care whole network services.
				*/
			static boost::asio::io_service io_service;

			/**
				* @brief Constructs object from specified socket.
				*/
			Connection(SocketP sockpointer);

			/**
				* @brief Destroys the object and closes the connection.
				*/
			~Connection();

			/**
				* @brief Returns whether the connection has been in some way closed and will
				* get no more messages.
				*/
			bool isClosed() const;

			/**
				* @brief Sets a function to call after we got the next full response.
				* @param[in] handler The function to handle the response.
				* @return Whether it was possible to set the handler -- it is impossible if
				* the Connection is currently used for some other communication.
				*/
			bool setResponseHandler(ResponseHandler handler);

			/**
				* @brief Sends the specified message to the other side of this connection.
				* @param[in] message The message to be send.
				* @param[in] handler The handler to set for receiving responces after
				* sending the message. This should be empty, if no response is anticipated.
				* @return Whether it was possible to send the message -- it is impossible if
				* the Connection is currently used for some other communication.
				*/
			bool sendMessage(std::string message, ResponseHandler handler);

			/**
				* @brief Blocks until the next communication sequence ends or the connection
				* is closed for some reason.
				*/
			void wait();

			/**
				* @brief Closes the connection.
				*/
			void close();

			/**
				* @brief Create a connection to the specified address.
				* @param[in] address The address to which to connect.
				* @todo Either specify properly the format of the address string or change
				* the arguments for this function and then specify it anyway, but for the
				* new arguments.
				*/
			static std::shared_ptr<Connection> connectTo(std::string address, std::string portNumber);

			/**
				* @brief Run io_service and start all connections.
				*/
			static void runAll();
		private:
			static void runIOservice();
			static boost::asio::io_service::work work;
			static std::shared_ptr<std::thread> netThread;
			void sendHandler(const boost::system::error_code& err, std::size_t bytes_transferred, ResponseHandler handler);
			void execResponseHandler(const boost::system::error_code& err, std::size_t bytes_transferred);
			SocketP sockPointer;
			ResponseHandler	curHandler;
			std::recursive_mutex mtx;
			std::condition_variable_any cv;
			std::atomic<int> dreamLevel;

			static const int BUF_SIZE = 2048;
			char buffer[BUF_SIZE];
	};
}

#endif
