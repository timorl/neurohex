#ifndef UTILITY_DFSTYLEPARSER_HPP
#define UTILITY_DFSTYLEPARSER_HPP

#include<boost/filesystem.hpp>
#include<boost/filesystem/fstream.hpp>
#include<vector>
#include<string>
#include<sstream>
#include<utility>

namespace utility {

	/**
		* @brief A parser for Dwarf Fortress style data files.
		*/
	class DFStyleParser {
		public:
			/**
				* @brief Construct a parser for files in the given directory.
				* @param[in] dir The directory containing files to parse.
				*/
			DFStyleParser( std::string dir );

			/**
				* @brief Start parsing the next file.
				* @returns false when no more files exist, true otherwise.
				*/
			bool nextFile();

			/**
				* @brief Start parsing the given file.
				* @param[in] filename The file to consider.
				* @return false if no such file exists, true otherwise.
				*/
			bool loadFile( std::string filename );

			/**
				* @brief Returns whether there are more files in the directory.
				*/
			bool hasNextToken() const;

			/**
				* @brief Returns the next token in the currently considered file.
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
			const std::string extension = ".dfs";
			boost::filesystem::path directory;
			bool valid;
			boost::filesystem::directory_iterator curFile;
			bool started;
			boost::filesystem::ifstream fileStream;
	};

}

#endif
