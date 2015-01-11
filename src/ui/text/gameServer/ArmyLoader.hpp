#ifndef UI_TEXT_GAMESERVER_ARMYLOADER_HPP
#define UI_TEXT_GAMESERVER_ARMYLOADER_HPP

#include<iostream>
#include"ui/Observable.hpp"
#include"neuroServer/ArmyLoader.hpp"

namespace ui {
namespace text {
namespace gameServer {

	/**
		* @brief The class to show the progress of loading armies.
		*/
	class ArmyLoader {
		public:
			/**
				* @brief Create a watcher for the given ArmyLoader.
				*/
			ArmyLoader(neuroServer::ArmyLoader & armyLoader);
		private:
			neuroServer::ArmyLoader & armyLoader;
	};

}
}
}

#endif
