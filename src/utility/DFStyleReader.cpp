#include"utility/DFStyleReader.hpp"

namespace utility {

	DFStyleReader::DFStyleReader( std::istream & inputStream ) : inputStream(inputStream) {
		std::string tmp;
		std::getline( inputStream, tmp, '[' );
	}

	std::vector< std::string > DFStyleReader::explode( const std::string & toExplode, char delim ) {
		std::vector< std::string > result;
		std::istringstream iss( toExplode );
		for ( std::string token; std::getline( iss, token, delim ); ) {
			result.push_back(token);
		}
		return result;
	}

	bool DFStyleReader::hasNextToken() const {
		return !inputStream.eof();
	}

	std::vector< std::string > DFStyleReader::getNextToken() {
		std::string tokenContents;
		std::getline( inputStream, tokenContents, ']' );
		std::vector< std::string > result = explode( tokenContents, ':' );
		std::getline( inputStream, tokenContents, '[' );
		return result;
	}

}
