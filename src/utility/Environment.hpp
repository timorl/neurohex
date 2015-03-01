#ifndef UTILITY_ENVIRONMENT_HPP
#define UTILITY_ENVIRONMENT_HPP

#include <string>
#include <SDL.h>

namespace utility {

	/**
		* @brief All the environment variables we might need.
		*/
	class Environment {
		public:
			/**
				* @brief Initialize all the environment variables.
				*/
			static void initialize();

			/**
				* @brief Whether the environment has been successfully initialized.
				*/
			static bool isInitialized() { return initialized; }

			/**
				* @brief Returns the global directory containing all the application data.
				*/
			static std::string getGlobalResourcePath();

			/**
				* @brief Returns the preferences path for the user running the program.
				*/
			static std::string getPreferencesPath();
		private:
			static bool initialized;
			static std::string globalResourcePath;
			static std::string preferencesPath;

			static bool setGlobalResourcePath();
			static bool setPreferencesPath();
	};

}

#endif
