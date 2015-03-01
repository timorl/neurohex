#include"neuroServer/Arbiter.hpp"

namespace neuroServer {

	neuro::Move Arbiter::getMove(int playerId, const neuro::Game & game) {
		//TODO: Actually validate the move.
		return contestant->getMove(playerId, game);
	}

	neuro::Targets Arbiter::getTargets(int playerId, const neuro::Game & game, neuro::AbilityIdentifier & abilityIdentifier ) {
		//TODO: Actually validate the targets.
		std::vector<neuro::AbilityIdentifier> abilities;
		abilities.push_back(abilityIdentifier);
		return contestant->getTargets(playerId, game, abilities);
	}

	int Arbiter::getDiscard(int playerId, const neuro::Game & game) {
		//TODO: Actually validate the tile.
		return contestant->getDiscard(playerId, game);
	}

	void Arbiter::requestTargetsForAbilities( int playerId, const neuro::Game & game, std::vector< neuro::AbilityIdentifier > abilities ) {
		//TODO: This is a stub.
	}

	neuro::Targets Arbiter::getTargetsForAbility( int id ) {
		neuro::Targets result;
		return result;
	}

}
