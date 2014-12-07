#ifndef UTILITY_DFDIRECTORYREADER_HPP
#define UTILITY_DFDIRECTORYREADER_HPP

#include<boost/filesystem.hpp>
#include<boost/filesystem/fstream.hpp>
#include<vector>
#include<string>
#include<sstream>
#include<utility>

#include"utility/DFStyleReader.hpp"

namespace utility {

	/**
		* @brief A utility for reading Dwarf Fortress style data files from a
		* directory.
		*/
	class DFDirectoryReader {
		public:
			/**
				* @brief Construct a reader for files in the given directory.
				* @param[in] dir The directory containing files to parse.
				*/
			DFDirectoryReader( std::string dir );

			/**
				* @brief Start parsing the next file.
				* @returns false when no more files exist, true otherwise.
				*/
			bool nextFile();

			/**
				* @brief Start parsing the given file in the directory.
				* @details If called manually further calls of nextFile have undefined
				* behaviour.
				* @param[in] filename The file to consider.
				* @return false if no such file exists, true otherwise.
				*/
			bool loadFile( std::string filename );

			/**
				* @brief Returns a DFStyleReader for the last loaded file.
				* @details Should be called only once per loaded file, otherwise the
				* behaviour is undefined.
				*/
			DFStyleReader	getCurrentFileReader();
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
