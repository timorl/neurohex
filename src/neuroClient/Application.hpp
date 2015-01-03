#ifndef NEUROCLIENT_APPLICATION_HPP
#define NEUROCLIENT_APPLICATION_HPP

#include<memory>
#include"ui/Observable.hpp"

namespace neuroClient {

	/**
		* @brief The class to start the whole application.
		* @todo This is a stub.
		*/
	class Application : public ui::Observable<Application> {
		public:
			/**
				* @brief Loads resources.
				* @todo This is a stub.
				*/
			void start();

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
	};

}

#endif
