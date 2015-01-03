#include"neuroServer/Arbiter.hpp"

namespace neuroServer {

	Move Arbiter::getMove(int playerId, const Players & players, const neuro::Board & board, bool noArmy) {
		//TODO: Actually validate the move.
		return contestant->getMove(playerId, players, board, noArmy);
	}

	Targets Arbiter::getTargets(int playerId, const Players & players, const neuro::Board & board, bool noArmy, neuro::AbilityIdentifier & abilityIdentifier ) {
		//TODO: Actually validate the targets.
		return contestant->getTargets(playerId, players, board, noArmy, abilityIdentifier);
	}

	int Arbiter::requestDiscard(int playerId, const Players & players, const neuro::Board & board, bool noArmy) {
		//TODO: Actually validate the tile.
		return contestant->requestDiscard(playerId, players, board, noArmy);
	}

	void Arbiter::requestTargetsForAbilities( int playerId, const Players & players, const neuro::Board & board, bool noArmy, std::vector< neuro::AbilityIdentifier > abilities ) {
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
