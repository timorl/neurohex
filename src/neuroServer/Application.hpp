#ifndef NEUROSERVER_APPLICATION_HPP
#define NEUROSERVER_APPLICATION_HPP

#include<memory>
#include"ui/Observable.hpp"
#include"neuroServer/BoardLoader.hpp"
#include"neuroServer/ArmyLoader.hpp"
#include"neuroServer/Configurer.hpp"

namespace neuroServer {

	/**
		* @brief The class to start the whole application.
		*/
	class Application : public ui::Observable<Application> {
		public:
			/**
				* @brief Create a new application object.
				*/
			Application() : configurer(boardLoader, armyLoader) {}

			/**
				* @brief Loads resources.
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
				* @brief Returns a reference to the game configurer.
				*/
			Configurer & getConfigurer() { return configurer; }

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
			BoardLoader boardLoader;
			ArmyLoader armyLoader;
			Configurer configurer;
	};

}

#endif