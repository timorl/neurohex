#ifndef NETWORK_CONNECTION_HPP
#define NETWORK_CONNECTION_HPP

#include<string>

namespace network {

	/**
		* @brief A representation of a single network connection to which you can write
		* and from which you can read.
		* @todo Add a constructor for creating accepting connections.
		* @todo Implement all the functions.
		*/
	class Connection {
		public:
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
				* @brief Returns the next message we got from this connection. Might block
				* and wait for something to return. Returns an empty string if closed.
				*/
			std::string nextMessage();

			/**
				* @brief Sends the specified message to the other side of this connection.
				* No guarantee as to whether the message has been send.
				* @param[in] message The message to be send.
				*/
			void sendMessage(std::string message);

			/**
				* @brief Create a connection to the specified address.
				* @param[in] address The address to which to connect.
				* @todo Either specify properly the format of the address string or change
				* the arguments for this function and then specify it anyway, but for the
				* new arguments.
				*/
			static Connection connectTo(std::string address);
		private:
	};

}

#endif
