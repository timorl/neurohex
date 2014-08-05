#ifndef VIEWMODEL_MAINMENU_HPP
#define VIEWMODEL_MAINMENU_HPP

#include<memory>
#include"ui/Observable.hpp"
#include"viewmodel/Configurer.hpp"
#include"viewmodel/BoardLoader.hpp"
#include"viewmodel/ArmyLoader.hpp"

namespace viewmodel {

	enum class MenuEvent {
		CREATE_GAME
	};

	/**
		* @brief An abstract representation of the main menu.
		*/
	class MainMenu : public ui::Observable<MainMenu> {
		public:
			/**
				* @brief Construct a main menu with the provided loaders.
				* @param[in] boardLoader A pointer to a BoardLoader which has already loaded
				* boards.
				* @param[in] armyLoader A pointer to an ArmyLoader which has already loaded
				* armies.
				*/
			MainMenu( BoardLoaderP boardLoader, ArmyLoaderP armyLoader ) : boardLoader(boardLoader), armyLoader(armyLoader) {}

			/**
				* @brief Creates a configurer and notifies listeners.
				*/
			void createGame();

			/**
				* @brief Returns the last event that occured.
				*/
			MenuEvent getEvent() const { return event; }

			/**
				* @brief Returns a pointer to the configurer created.
				*/
			ConfigurerP getConfigurer() const {	return configurer;	}
		private:
			ConfigurerP configurer;
			BoardLoaderP	boardLoader;
			ArmyLoaderP	armyLoader;
			MenuEvent	event;
	};

	using MainMenuP = std::shared_ptr< MainMenu >;

}

#endif
