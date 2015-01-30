#ifndef UI_TEXT_CLIENT_SERVERJOINER_HPP
#define UI_TEXT_CLIENT_SERVERJOINER_HPP

#include"ui/text/CommandShell.hpp"
#include"neuroClient/Application.hpp"
#include<cassert>

namespace ui {
namespace text {
	
    using Arguments = std::vector<std::string>;

    /**
        * @brief Simple shell to connect to server.
        */
    class ServerJoiner {
        public:
			/**
				* @brief Run this command shell.
				*/
            void run();

			/**
				* @brief Join server command.
				*/
            bool joinCommand(Arguments args);

			/**
				* @brief Create a serverjoiner shell to a given application.
				*/
			ServerJoiner(neuroClient::Application & application);
        private:   
			neuroClient::Application & application;
            CommandShell shell; 
    };

}
}
#endif /* UI_TEXT_CLIENT_SERVERJOINER_HPP */

