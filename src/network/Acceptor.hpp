#ifndef NETWORK_ACCEPTOR_HPP
#define NETWORK_ACCEPTOR_HPP

#include<string>
#include"network/Connection.hpp"

namespace network {

	/**
		* @brief A class for accepting all connections on a specific socket.
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
			Connection getNextConnection();

			/**
				* @brief Stop accepting connections on the port. If there are any pending
				* connections, close them.
				*/
			void stopAccepting();

			/**
				* @brief Start accepting connections on the port.
				*/
			void startAccepting();
		private:
	};

}

#endif
