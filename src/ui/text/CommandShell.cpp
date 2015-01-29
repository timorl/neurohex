#include"ui/text/CommandShell.hpp"

namespace ui {
namespace text {


	CommandShell::CommandShell(std::string prompt) : prompt(prompt) {
		addCommand("help", "Displays this help message.",
				[this](Arguments)->bool{
					std::cout << getHelpString();
					return false;
				});
		addCommand("exit", "Exits this shell.",
				[](Arguments)->bool{
					return true;
				});
	}

	void CommandShell::addCommand(std::string name, std::string description, Command command) {
		descriptions[name] = description;
		commands[name] = command;
	}
    
    void CommandShell::write(std::string message) {
        std::cout << message << std::endl;
    }

	void CommandShell::run() {
		std::string line;
		bool finished = false;
		while ( !finished ) {
			std::cout << prompt << ": ";
			getline(std::cin, line);
			if ( line == "" ) {
				continue;
			}
			Arguments args = utility::DFStyleReader::explode(line, ' ');
			if ( commands.count(args[0]) > 0 ) {
				finished = commands[args[0]](args);
			} else {
				std::cout << args[0] << ": command not found.\n";
				finished = commands["help"](args);
			}
		}
	}

	std::string CommandShell::getHelpString() const {
		std::string result = "The available commands are:\n";
		for ( const auto & cmd : descriptions ) {
			result += "\t" + cmd.first + "\t" + cmd.second + "\n";
		}
		return result;
	}

}
}
