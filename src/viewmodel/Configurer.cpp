#include"viewmodel/Configurer.hpp"

namespace viewmodel {

	bool Configurer::verifySettings() const {
		//FIXME: Actually chack something.
		return true;
	}

 bool Configurer::startGame() {
		if ( verifySettings() ) {
			game = neuro::GameP( new neuro::Game( config ) );
			sigModified(*this);
			game->play();
			game.reset();
			return true;
		} else {
			return false;
		}
	}

	void Configurer::setArmy( int contestantId, neuro::ArmyP army ) {
		if ( contestantId >= 0 && contestantId < static_cast<int>( config.contestants.size() ) ) {
			config.armies[contestantId] = army;
			sigModified(*this);
		}
	}

}
