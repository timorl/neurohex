#include"ui/text/gameServer/BoardLoader.hpp"

namespace ui {
namespace text {
namespace gameServer {

	void boardLoaded(const neuroServer::BoardLoader & bl) {
		const std::string boardLoadedMessage = "Currently loaded boards: ";
		std::cout << boardLoadedMessage << bl.getDescriptions().size() << std::endl;
	}

	BoardLoader::BoardLoader(neuroServer::BoardLoader & boardLoader) :
		boardLoader(boardLoader) {
		boardLoader.sigModified.connect(boardLoaded);
	}

}
}
}
