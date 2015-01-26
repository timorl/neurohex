#include"neuroServer/Arbiter.hpp"

namespace neuroServer {

	Move Arbiter::getMove(int playerId, const Game & game) {
		//TODO: Actually validate the move.
		return contestant->getMove(playerId, game);
	}

	Targets Arbiter::getTargets(int playerId, const Game & game, neuro::AbilityIdentifier & abilityIdentifier ) {
		//TODO: Actually validate the targets.
		std::vector<neuro::AbilityIdentifier> abilities;
		abilities.push_back(abilityIdentifier);
		return contestant->getTargets(playerId, game, abilities);
	}

	int Arbiter::getDiscard(int playerId, const Game & game) {
		//TODO: Actually validate the tile.
		return contestant->getDiscard(playerId, game);
	}

	void Arbiter::requestTargetsForAbilities( int playerId, const Game & game, std::vector< neuro::AbilityIdentifier > abilities ) {
		//TODO: This is a stub.
	}

	Targets Arbiter::getTargetsForAbility( int id ) {
		Targets result;
		return result;
	}

	Targets Arbiter::generateTargets( const neuro::Board & board, neuro::AbilityIdentifier & abilityIdentifier ) {
		Targets result;
		return result;
	}

	bool Arbiter::purgeTargets( Targets & targets, const neuro::Board & board, neuro::AbilityIdentifier identifier ) {
		//TODO: This is a stub.
		return true;
	}

}
