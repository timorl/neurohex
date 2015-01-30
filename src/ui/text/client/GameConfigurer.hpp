#ifndef UI_TEXT_CLIENT_GAMECONFIGURER_HPP
#define UI_TEXT_CLIENT_GAMECONFIGURER_HPP

#include"ui/text/CommandShell.hpp"
#include"neuroClient/Application.hpp"
#include<cassert>

namespace ui {
namespace text {

    using Arguments = std::vector<std::string>;

    /**
        * @brief A simple shell for game configuration purposes.
        */
    class GameConfigurer {
        public:
			/**
				* @brief Run this command shell.
				*/
            void run();

			/**
				* @brief Brutally writes the string to server.
				*/
            bool writeToServer(Arguments args);

			/**
				* @brief Create a gameconfigurer.
				*/
            GameConfigurer(neuroClient::Application & application);
        private:
            neuroClient::Application & application;
            CommandShell shell;
    };

}
}
#endif /* UI_TEXT_CLIENT_GAMECONFIGURER_HPP */

