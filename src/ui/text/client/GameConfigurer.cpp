#include"ui/text/client/GameConfigurer.hpp"
#include<string>

namespace ui {
namespace text {

    GameConfigurer::GameConfigurer(neuroClient::Application & application) :
            application(application), shell("gameConfiguration") {
        shell.addCommand("input", "What you put after it goes to the server.",
                    std::bind(&GameConfigurer::writeToServer, this, std::placeholders::_1)); 
    }

    void GameConfigurer::run() {
        shell.run();
    }

    bool GameConfigurer::writeToServer(Arguments args) {

        std::string message;
        for ( int i = 1; i < args.size(); i++)
            message += args[i];

        if ( application.writeToServer(message) ) {
            shell.write("Message successfully sent to server.");
        } else {
            shell.write("WARNING: Connection to server lost.");
        }

        return false;
    }

}
}


