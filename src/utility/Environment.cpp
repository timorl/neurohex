#include"utility/Environment.hpp"

namespace utility {

	bool Environment::initialized = false;
	std::string Environment::globalResourcePath = "";
	std::string Environment::preferencesPath = "";

	void Environment::initialize() {
		if ( setGlobalResourcePath() && setPreferencesPath() ) {
			initialized = true;
		}
	}

	std::string Environment::getGlobalResourcePath() {
		if ( !isInitialized() ) {
			initialize();
		}
		return globalResourcePath;
	}

	std::string Environment::getPreferencesPath() {
		if ( !isInitialized() ) {
			initialize();
		}
		return preferencesPath;
	}

	bool Environment::setGlobalResourcePath() {
		char *basePath = SDL_GetBasePath();
		if ( basePath ) {
			globalResourcePath = basePath;
			SDL_free(basePath);
			return true;
		}
		return false;
	}

	bool Environment::setPreferencesPath() {
		char *basePath = SDL_GetPrefPath("timorl", "Neurohex");
		if ( basePath ) {
			preferencesPath = basePath;
			SDL_free(basePath);
			return true;
		}
		return false;
	}

}
