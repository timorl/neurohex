#ifndef NEUROSERVER_CONTESTANT_HPP
#define NEUROSERVER_CONTESTANT_HPP

#include<memory>
#include<vector>
#include"neuro/Game.hpp"

namespace neuroServer {

	/**
		* @brief The interface any contestant (i.e. bot or player ui) should
		* implement.
		*/
	class Contestant {
		public:
			virtual void requestMove(int playerId, const neuro::Game & game) = 0;
			virtual neuro::Move getMove(int playerId, const neuro::Game & game) = 0;
			virtual void requestTargets(int playerId, const neuro::Game & game, std::vector< neuro::AbilityIdentifier > abilities) = 0;
			virtual neuro::Targets getTargets(int playerId, const neuro::Game & game, std::vector< neuro::AbilityIdentifier > abilities) = 0;
			virtual void requestDiscard(int playerId, const neuro::Game & game) = 0;
			virtual int getDiscard(int playerId, const neuro::Game & game) = 0;
	};

	using ContestantP = std::shared_ptr<Contestant>;

}

#endif
