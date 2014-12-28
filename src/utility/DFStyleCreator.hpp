#ifndef UTILITY_DFSTYLECREATOR_HPP
#define UTILITY_DFSTYLECREATOR_HPP

#include<vector>
#include<string>
#include<sstream>
#include<utility>

namespace utility {

	/**
		* @brief A creator of Dwarf Fortress style data.
		*/
	class DFStyleCreator {
		public:
			/**
				* @brief Construct a creator for the given stream.
				* @param[in] outputStream The stream to which the created data will be send.
				*/
			DFStyleCreator( std::ostream & outputStream ) : outputStream(outputStream) {std::boolalpha(outputStream);}

			/**
				* @brief Starts a new token with the specified name.
				*/
			void startToken(const std::string & type);

			/**
				* @brief Adds another part of the token.
				*/
			template<class Data>
			void addToToken(const Data & data);

			/**
				* @brief Finishes the token.
				*/
			void endToken();
		private:
			std::ostream & outputStream;
	};

}

#endif
