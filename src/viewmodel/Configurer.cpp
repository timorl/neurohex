#include"viewmodel/Configurer.hpp"

namespace viewmodel {

	bool Configurer::verifySettings() const {
		//FIXME: Actually chack something.
		return true;
	}

 bool Configurer::startGame() {
		if ( verifySettings() ) {
			game = GameP( new neuro::Game( config ) );
			sigModified(*this);
			game->play();
			game.reset();
			return true;
		} else {
			return false;
		}
	}

	void Configurer::setArmy( int contestantId, std::vector<neuro::Tile> army ) {
		if ( contestantId >= 0 && contestantId < static_cast<int>( config.contestants.size() ) ) {
			config.armies[contestantId].clear();
			for ( neuro::Tile & tile : army ) {
				config.armies[contestantId].emplace_back(tile);
			}
			sigModified(*this);
		}
	}

}
