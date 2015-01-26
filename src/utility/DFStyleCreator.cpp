#include"utility/DFStyleCreator.hpp"

namespace utility {

	void DFStyleCreator::startToken(const std::string & type) {
		outputStream << "[" << type;
	}

	template<class Data>
	void DFStyleCreator::addToToken(const Data & data) {
		outputStream << ":" << data;
	}

	template void DFStyleCreator::addToToken<int>(const int & data);
	template void DFStyleCreator::addToToken<bool>(const bool & data);
	template void DFStyleCreator::addToToken<std::string>(const std::string & data);
	template void DFStyleCreator::addToToken<unsigned long>(const unsigned long & data);

	void DFStyleCreator::endToken() {
		outputStream << "]";
	}

}
