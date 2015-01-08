#include<cstdlib>
#include<iostream>
#include"neuroClient/Application.hpp"
#include"ui/text/client/Application.hpp"

bool setEnvironmentVariables() {
	char * tmp;
	std::string home;
	tmp = std::getenv( "HOME" );
	if ( tmp != nullptr ) {
		home = std::string(tmp);
	} else {
		return false;
	}
	std::string defaultXdgDataHome = home + "/.local/share";
	std::string defaultXdgConfigHome = home + "/.config";
	std::string defaultXdgDataDirs("/usr/local/share/:/usr/share/");
	std::string defaultXdgConfigDirs("/etc/xdg");
	std::string defaultXdgCacheHome = home + "/.cache";
	setenv("XDG_DATA_HOME", defaultXdgDataHome.c_str(), 0);
	setenv("XDG_CONFIG_HOME", defaultXdgConfigHome.c_str(), 0);
	setenv("XDG_DATA_DIRS", defaultXdgDataDirs.c_str(), 0);
	setenv("XDG_CONFIG_DIRS", defaultXdgConfigDirs.c_str(), 0);
	setenv("XDG_CACHE_HOME", defaultXdgCacheHome.c_str(), 0);
	return true;
}

void setApplicationVariables() {
	std::string programDirectory("/neurohex");
	std::string globalDataDirectories( std::getenv("XDG_DATA_DIRS") );
	neuroClient::Application::globalDataDirectory = globalDataDirectories.substr( 0, globalDataDirectories.find_first_of(':') ) + programDirectory;
	std::string localDataDirectories( std::getenv("XDG_DATA_HOME") );
	neuroClient::Application::localDataDirectory = localDataDirectories.substr( 0, localDataDirectories.find_first_of(':') ) + programDirectory;
}

int main() {
	if ( !setEnvironmentVariables() ) {
		std::cerr << "$HOME variable is not set, what are you, using win***s?" << std::endl;
		return 1;
	}
	setApplicationVariables();
	neuroClient::Application application;
	ui::text::client::Application applicationUI(application);
	application.start();
	return 0;
}
