#ifndef UTILITY_DFSTYLEREADER_HPP
#define UTILITY_DFSTYLEREADER_HPP

#include<vector>
#include<string>
#include<sstream>
#include<utility>

namespace utility {

	/**
		* @brief A reader of Dwarf Fortress style data.
		*/
	class DFStyleReader {
		public:
			/**
				* @brief Construct a reader for the given stream.
				* @param[in] inputStream The stream outputting data to parse.
				*/
			DFStyleReader( std::istream & inputStream ) : inputStream(inputStream) {}

			/**
				* @brief Returns whether there is a next token in the stream.
				*/
			bool hasNextToken() const;

			/**
				* @brief Returns the next token in the stream.
				* @returns A std::vector containing one std::string for every piece of data
				* in the token. The name of the token is at position 0.
				*/
			std::vector< std::string > getNextToken();

			/**
				* @brief Break the provided string into a vector of strings.
				* @param[in] toExplode A reference to the string to break.
				* @param[in] delim The delimiter by which to break the string.
				* @return A std::vector containing one std::string or every token in the
				* string.
				*/
			static std::vector< std::string > explode( const std::string & toExplode, char delim );
		private:
			std::istream & inputStream;
	};

}

#endif
