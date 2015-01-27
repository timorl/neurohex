#ifndef NETWORK_ACCEPTOR_HPP
#define NETWORK_ACCEPTOR_HPP

#include<string>
#include"network/Connection.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
//#include <boost/asio/ip/tcp.hpp>
#include<vector>
#include<mutex>
#include <memory>
#include <boost/thread.hpp>
#include <boost/ptr_container/ptr_vector.hpp>


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

			void accept_handler(const boost::system::error_code& error, SocketP SockPointer);
		private:
			int portNumber;
			int firstUnusedSocket;
			std::vector<SocketP> arraySocketsP;
			boost::ptr_vector<boost::mutex> arrayMutex;
	};

}

#endif
