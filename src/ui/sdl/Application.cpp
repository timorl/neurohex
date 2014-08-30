#include"ui/sdl/Application.hpp"

namespace ui {
namespace sdl {
	bool Application::init() {
		bool success = true;

		if(SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else {
			gWindow = SDL_CreateWindow( windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
			if(gWindow == NULL) {
				printf("Window could not be created! SDL_Error: %s\n", SDL_GetError() );
				success = false;
			}
			else {
				gScreenSurface = SDL_GetWindowSurface(gWindow);
			}
		}

		if(IMG_Init(IMG_INIT_JPG) < 0) {
			printf("IMG could not initialize! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}

		return success;
	}

	bool Application::loadMedia() {
		bool success = true;
		std::string localDataDirectories( std::getenv("XDG_DATA_HOME") );
		std::string localDataDirectory = localDataDirectories.substr( 0, localDataDirectories.find_first_of(':') ) + ("/neurohex");
		std::string path = localDataDirectory + "/media/img/testTla.png";
		gPhoto = IMG_Load( path.c_str() );	
		if(gPhoto == NULL) {
			printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    	    success = false;
		}

		return success;
	}

	void Application::close() {
		SDL_FreeSurface( gPhoto );
		gPhoto = NULL;

		IMG_Quit();
	
		SDL_DestroyWindow( gWindow );
		gWindow = NULL;

		SDL_Quit();
	}

	void Application::play() {
		bool quit = false;
		SDL_Event e;

		while( !quit ) {
			while( SDL_PollEvent( &e ) != 0 ) {
				if( e.type == SDL_QUIT ) {
					quit = true;
				}
			}

			SDL_BlitSurface( gPhoto, NULL, gScreenSurface, NULL );
			SDL_UpdateWindowSurface( gWindow );
		}
	}
}
}
