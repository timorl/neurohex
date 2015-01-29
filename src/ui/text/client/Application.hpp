#ifndef UI_TEXT_CLIENT_APPLICATION_HPP
#define UI_TEXT_CLIENT_APPLICATION_HPP

#include<iostream>
#include"ui/Observable.hpp"
#include"neuroClient/Application.hpp"

namespace ui {
namespace text {
namespace client {

	/**
		* @brief The class to start the text ui fot the client.
		* @todo This is just meant as a general guideline for now.
		*/
	class Application {
		public:
            /**
                * @brief Start the command line interface.
                */
            void applicationStarted(const neuroClient::Application &);

			/**
				* @brief Create an application watching the given application.
				*/
			Application(neuroClient::Application & application);
		private:
			neuroClient::Application & application;
	};

}
}
}

#endif
