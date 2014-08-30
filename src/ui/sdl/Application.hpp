#ifndef UI_SDL_APPLICATION_HPP
#define UI_SDL_APPLICATION_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

namespace ui {
namespace sdl {

	/**
	* @brief A class that starts, handles and stops sdl.
	* @todo Everything.
	*/
	
	class Application {
		public:
			Application() { init(); loadMedia(); }
			~Application() { close(); }

			void play();

		private:

			const int SCREEN_WIDTH = 1024;
			const int SCREEN_HEIGHT = 768;
			const std::string windowName = "default";
			SDL_Window* gWindow = NULL;
			SDL_Surface* gScreenSurface = NULL;
			SDL_Surface* gPhoto = NULL;

			bool init();
			bool loadMedia();
			void close();
	};

}
}

#endif
