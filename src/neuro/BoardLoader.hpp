#ifndef NEURO_BOARDLOADER_HPP
#define NEURO_BOARDLOADER_HPP

#include<vector>
#include"neuro/Board.hpp"

namespace neuro {

	/**
		* @brief A class for loading files with board descriptions.
		* @todo This is mostly a stub.
		*/
	class BoardLoader {
		public:
			/**
				* @brief Load all board description files in the given directory.
				* @param[in] directory The directory in which to look for the files.
				* @todo This is a stub, does nothing.
				*/
			void loadBoards(std::string directory);

			/**
				* @brief Add a board description.
				* @param[in] name The name for the board being added.
				* @param[in] description A longer description of the board.
				* @param[in] board The BoardDescription of the board being added.
				*/
			void addBoard(std::string name, std::string description, BoardDescription board);

			/**
				* @brief Returns a std::map of names of loaded boards.
				*/
			const std::map< std::string, std::string > & getDescriptions() const { return descriptions; }

			/**
				* @brief Get a board with the specified name.
				* @param[in] name The name of the board we are looking for.
				* @return A BoardDescription of the board with the specified name. If no
				* board has such name the behaviour is undefined.
				* @todo For now this always returns a default board.
				*/
			BoardDescription	getBoard(std::string name) const;
		private:
			std::map< std::string, BoardDescription > boards;
			std::map< std::string, std::string > descriptions;
	};

}

#endif
