#ifndef NEUROSERVER_ARMYLOADER_HPP
#define NEUROSERVER_ARMYLOADER_HPP

#include<vector>
#include"ui/Observable.hpp"
#include"utility/DFDirectoryReader.hpp"
#include"utility/DFStyleReader.hpp"
#include"neuro/Army.hpp"

namespace neuroServer {

	/**
		* @brief A class for loading files with army descriptions.
		*/
	class ArmyLoader : public ui::Observable< ArmyLoader > {
		public:
			/**
				* @brief Load all army description files in the given directory.
				* @param[in] directory The directory in which to look for the files.
				*/
			void loadArmies(std::string directory);

			/**
				* @brief Returns a std::map of names and descriptions of loaded armies.
				*/
			const std::map< std::string, std::string > & getDescriptions() const { return descriptions; }

			/**
				* @brief Get an army with the specified name.
				* @param[in] name The name of the army we are looking for.
				* @return A reference to a vector containing tiles in the requested army.
				*/
			const std::vector<neuro::Tile> &	getArmy(std::string name) const { return armies.at(name); }
		private:
			std::map< std::string, std::vector<neuro::Tile> > armies;
			std::map< std::string, std::string > descriptions;
	};

}

#endif
