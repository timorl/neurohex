#ifndef UI_TEXT_GAMESERVER_BOARDLOADER_HPP
#define UI_TEXT_GAMESERVER_BOARDLOADER_HPP

#include<iostream>
#include"ui/Observable.hpp"
#include"neuroServer/BoardLoader.hpp"

namespace ui {
namespace text {
namespace gameServer {

	/**
		* @brief The class to show the progress of loading boards.
		*/
	class BoardLoader {
		public:
			/**
				* @brief Create a watcher for the given BoardLoader.
				*/
			BoardLoader(neuroServer::BoardLoader & boardLoader);
		private:
			neuroServer::BoardLoader & boardLoader;
	};

}
}
}

#endif
