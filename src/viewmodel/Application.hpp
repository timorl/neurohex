#ifndef VIEWMODEL_APPLICATION_HPP
#define VIEWMODEL_APPLICATION_HPP

#include<memory>
#include"ui/Observable.hpp"
#include"viewmodel/BoardLoader.hpp"
#include"viewmodel/ArmyLoader.hpp"
#include"viewmodel/MainMenu.hpp"

namespace viewmodel {

	/**
		* @brief The class to start the whole application.
		*/
	class Application : public ui::Observable<Application> {
		public:
			/**
				* @brief Create a new application object.
				*/
			Application() : mainMenu(boardLoader, armyLoader) {}

			/**
				* @brief Loads resources and starts the main menu.
				*/
			void start();

			/**
				* @brief Returns the BoardLoader used.
				*/
			const BoardLoader & getBoardLoader() const {	return boardLoader;	}

			/**
				* @brief Returns the ArmyLoader used.
				*/
			const ArmyLoader & getArmyLoader() const {	return armyLoader;	}

			/**
				* @brief Returns a pointer to the main menu.
				*/
			MainMenu & getMainMenu() { return mainMenu; }

			/**
				* @brief The global directory containing all the application data.
				*/
			static std::string globalDataDirectory;

			/**
				* @brief The local directory containing the application data changed by the
				* user.
				*/
			static std::string localDataDirectory;
		private:
			MainMenu mainMenu;
			BoardLoader boardLoader;
			ArmyLoader armyLoader;
	};

}

#endif
