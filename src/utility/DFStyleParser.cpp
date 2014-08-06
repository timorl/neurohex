#include"utility/DFStyleParser.hpp"

namespace utility {

	std::vector< std::string > DFStyleParser::explode( const std::string & toExplode, char delim ) {
		std::vector< std::string > result;
		std::istringstream iss( toExplode );
		for ( std::string token; std::getline( iss, token, delim ); ) {
			result.push_back(token);
		}
		return result;
	}

	DFStyleParser::DFStyleParser( std::string dir ) :
		directory(dir),
		started(false) {
		valid = ( boost::filesystem::exists( dir ) && boost::filesystem::is_directory( dir ) );
		if ( valid ) {
			curFile = boost::filesystem::directory_iterator( directory );
		}
	}

	bool DFStyleParser::nextFile() {
		if ( !valid ) {
			return false;
		}
		boost::filesystem::directory_iterator end;
		if ( curFile == end ) { // No more files.
			return false;
		}
		if ( !started ) {
			started = true;
		} else {
			curFile++;
		}
		if ( curFile == end ) { // No more files.
			return false;
		}
		while ( curFile->path().extension() != extension ) {
			curFile++;
			if ( curFile == end ) { // No more files.
				return false;
			}
		}
		return loadFile( curFile->path().filename().generic_string() );
	}

	bool DFStyleParser::loadFile( std::string filename ) {
		if ( !valid ) {
			return false;
		}
		boost::filesystem::path pathToLoad( directory / filename );
		if ( fileStream.is_open() ) {
			fileStream.close();
		}
		fileStream.open( pathToLoad );
		if ( !fileStream ) {
			return false;
		}
		std::string tmp;
		std::getline( fileStream, tmp, '[' );
		return true;
	}

	bool DFStyleParser::hasNextToken() const {
		return !fileStream.eof();
	}

	std::vector< std::string > DFStyleParser::getNextToken() {
		std::string tokenContents;
		std::getline( fileStream, tokenContents, ']' );
		std::vector< std::string > result = explode( tokenContents, ':' );
		std::getline( fileStream, tokenContents, '[' );
		return result;
	}

}
