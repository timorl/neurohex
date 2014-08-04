#ifndef NEURO_ARMYLOADER_HPP
#define NEURO_ARMYLOADER_HPP

#include<vector>
#include"neuro/Army.hpp"

namespace neuro {

	/**
		* @brief A class for loading files with army descriptions.
		* @todo This is mostly a stub.
		* @todo Figure out how to handle the possibility of duplicate armies.
		*/
	class ArmyLoader {
		public:
			/**
				* @brief Load all army description files in the given directory.
				* @param[in] directory The directory in which to look for the files.
				* @todo This is a stub, does nothing.
				*/
			void loadArmies(std::string directory);

			/**
				* @brief Returns a std::map of names and descriptions of loaded armies.
				*/
			const std::map< std::string, std::string > & getDescriptions() const { return descriptions; }

			/**
				* @brief Get an army with the specified name.
				* @param[in] name The name of the army we are looking for.
				* @return A pointer to the army with the specified name. If no
				* army has such name the pointer is empty.
				* @todo For now this always returns a stub army.
				*/
			ArmyP	getArmy(std::string name) const;
		private:
			std::map< std::string, ArmyP > armies;
			std::map< std::string, std::string > descriptions;
	};

}

#endif
