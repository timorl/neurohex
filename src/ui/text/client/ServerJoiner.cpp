
#include"ui/text/client/ServerJoiner.hpp"

namespace ui {
namespace text {
    
    ServerJoiner::ServerJoiner(neuroClient::Application & application) :
            application(application), shell("neuroClient") {
        shell.addCommand("join", "Join server command. Syntax: join address port username",
                    std::bind(&ServerJoiner::joinCommand, this, std::placeholders::_1));
    }

    void ServerJoiner::run() {
        shell.run();
    }

    bool ServerJoiner::joinCommand(Arguments args) {
        assert(args.size() == 4);
        if ( application.joinServer(args[1], args[2], args[3]) ) {
            shell.write("Connection successful.");
            return true;
        } else {
            shell.write("Connection NOT successful!");
            return false;
        }
    }

}
}

