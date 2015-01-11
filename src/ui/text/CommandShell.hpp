#ifndef UI_TEXT_COMMANDSHELL_HPP
#define UI_TEXT_COMMANDSHELL_HPP

#include<iostream>
#include<functional>
#include<map>
#include"utility/DFStyleReader.hpp"

namespace ui {
namespace text {

	using Arguments = std::vector<std::string>;
	using Command = std::function<bool(Arguments)>;

	/**
		* @brief A simple shell to handle commands.
		*/
	class CommandShell {
		public:
			/**
				* @brief Create a default shell containing no nontrivial commands.
				*/
			CommandShell(std::string prompt);

			/**
				* @brief Add a command to the shell.
				* @param[in] name The name of the command.
				* @param[in] description A description for the command, to be shown in the
				* help string.
				* @param[in] command The function to execute when the command is invoked.
				*/
			void addCommand(std::string name, std::string description, Command command);

			/**
				* @brief Run the shell until the user exits.
				*/
			void run();
		private:
			std::string prompt;
			std::map<std::string, std::string> descriptions;
			std::map<std::string, Command> commands;

			std::string getHelpString() const;
	};

}
}

#endif
