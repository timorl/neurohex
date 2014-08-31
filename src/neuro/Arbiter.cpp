#include"neuro/Arbiter.hpp"

namespace neuro {

	Move Arbiter::getMove(int playerId, const Players & players, const Board & board, bool noArmy) {
		//TODO: Actually validate the move.
		return contestant->getMove(playerId, players, board, noArmy);
	}

	Targets Arbiter::getTargets(int playerId, const Players & players, const Board & board, bool noArmy, AbilityIdentifier & abilityIdentifier ) {
		//TODO: Actually validate the targets.
		return contestant->getTargets(playerId, players, board, noArmy, abilityIdentifier);
	}

	TileP Arbiter::requestDiscard(int playerId, const Players & players, const Board & board, bool noArmy) {
		//TODO: Actually validate the tile.
		return contestant->requestDiscard(playerId, players, board, noArmy);
	}

	void Arbiter::requestTargetsForAbilities( int playerId, const Players & players, const Board & board, bool noArmy, std::vector< AbilityIdentifier > abilities ) {
		//TODO: Whis is a stub.
	}

	Targets Arbiter::getTargetsForAbility( int id ) {
		Targets result;
		return result;
	}

	Targets Arbiter::generateTargets( const Board & board, AbilityIdentifier & abilityIdentifier ) {
		Targets result;
		return result;
	}

	bool Arbiter::purgeTargets( Targets & targets, const Board & board, AbilityIdentifier identifier ) {
		//TODO: This is a stub.
		return true;
	}

}
