#ifndef VIEWMODEL_BOARDLOADER_HPP
#define VIEWMODEL_BOARDLOADER_HPP

#include<vector>
#include"ui/Observable.hpp"
#include"utility/DFStyleParser.hpp"
#include"neuro/Board.hpp"

namespace viewmodel {

	/**
		* @brief A class for loading files with board descriptions.
		*/
	class BoardLoader : public ui::Observable<BoardLoader> {
		public:
			/**
				* @brief Load all board description files in the given directory.
				* @param[in] directory The directory in which to look for the files.
				*/
			void loadBoards(std::string directory);

			/**
				* @brief Add a board description.
				* @param[in] name The name for the board being added.
				* @param[in] description A longer description of the board.
				* @param[in] board The BoardDescription of the board being added.
				*/
			void addBoard(std::string name, std::string description, neuro::BoardDescription board);

			/**
				* @brief Returns a std::map of names of loaded boards.
				*/
			const std::map< std::string, std::string > & getDescriptions() const { return descriptions; }

			/**
				* @brief Get a board with the specified name.
				* @param[in] name The name of the board we are looking for.
				* @return A BoardDescription of the board with the specified name. If no
				* board has such name the behaviour is undefined.
				*/
			neuro::BoardDescription	getBoard(std::string name) const;

			/**
				* @brief Fills in the specified BoardDescription using the provided string.
				* @param[out] boardFields The BoardDescription to fill in.
				* @param[in] x,y The dimensions of the BoardDescription to fill in.
				* @param[in] flds The string describing the fields of the board.
				*/
			static void fillInBoardFields( neuro::BoardDescription & boardFields, int x, int y, std::string flds );
		private:
			std::map< std::string, neuro::BoardDescription > boards;
			std::map< std::string, std::string > descriptions;
	};

	using BoardLoaderP = std::shared_ptr< BoardLoader >;

}

#endif
