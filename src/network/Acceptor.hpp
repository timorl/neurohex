#ifndef NETWORK_ACCEPTOR_HPP
#define NETWORK_ACCEPTOR_HPP

#include<string>
#include<network/Connection.hpp>
#include<boost/asio.hpp>
#include<atomic>
#include<queue>
#include<mutex>
#include<condition_variable>
#include <memory>


using boost::asio::ip::tcp;

namespace network {

	/**
		* @brief A class for accepting all connections on a specific port.
		* @todo Implement all the functions.
		*/
	class Acceptor {
		public:
			/**
				* @brief Create an acceptor on the specified port number.
				* @param[in] portNumber The port on which to accept connections.
				*/
			Acceptor(int portNumber);

			/**
				* @brief Stop accepting connections and destroy the acceptor.
				*/
			~Acceptor();

			/**
				* @brief Returns the first available connection. If there are no
				* connections wait for one.
				*/
			std::shared_ptr<Connection> getNextConnection();

			/**
				* @brief Start accepting connections on the port.
				* @param[in] max The maximum number of connections to keep open.
				*/
			void startAccepting(int max);

		private:
			boost::asio::ip::tcp::acceptor acceptor;
			std::queue<SocketP> readySockets;
			std::atomic<int> waitingSockets;
			std::mutex mtx;
			std::condition_variable cv;

			void acceptHandler(const boost::system::error_code& error, SocketP SockPointer);
	};

}

#endif
