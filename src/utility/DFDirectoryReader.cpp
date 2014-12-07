#include"utility/DFDirectoryReader.hpp"

namespace utility {

	DFDirectoryReader::DFDirectoryReader( std::string dir ) :
		directory(dir),
		started(false) {
		valid = ( boost::filesystem::exists( dir ) && boost::filesystem::is_directory( dir ) );
		if ( valid ) {
			curFile = boost::filesystem::directory_iterator( directory );
		}
	}

	bool DFDirectoryReader::nextFile() {
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

	bool DFDirectoryReader::loadFile( std::string filename ) {
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

	DFStyleReader DFDirectoryReader::getCurrentFileReader() {
		return DFStyleReader( fileStream );
	}

}
