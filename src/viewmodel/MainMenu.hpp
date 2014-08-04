#ifndef VIEWMODEL_MAINMENU_HPP
#define VIEWMODEL_MAINMENU_HPP

#include<memory>
#include"ui/Observable.hpp"
#include"viewmodel/Configurer.hpp"

namespace viewmodel {

	enum class MenuEvent {
		PLAY_GAME
	};

	/**
		* @brief An abstract representation of the main menu.
		*/
	class MainMenu : public ui::Observable<MainMenu> {
		public:
			/**
				* @brief Creates a configurer and notifies listeners.
				*/
			void playGame();

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
			MenuEvent	event;
	};

	using MainMenuP = std::shared_ptr< MainMenu >;

}

#endif
