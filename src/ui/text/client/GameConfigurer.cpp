#include"ui/text/client/GameConfigurer.hpp"

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
        assert(args.size() == 2);
        if ( application.writeToServer(args[1]) ) {
            shell.write("Message successfully sent to server.");
        } else {
            shell.write("WARNING: Connection to server lost.");
        }

        return false;
    }

}
}


