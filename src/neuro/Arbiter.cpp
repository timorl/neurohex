#include"neuro/Arbiter.hpp"

namespace neuro {

	Move Arbiter::getMove(int playerId, const Players & players, const Board & board, bool noArmy) {
		//TODO: Actually validate the move.
		return contestant->getMove(playerId, players, board, noArmy);
	}

	Targets Arbiter::getTargets(int playerId, const Players & players, const Board & board, bool noArmy, const Tile & tile, AbilityGroup abilityGroup, int abilityId ) {
		//TODO: Actually validate the targets.
		return contestant->getTargets(playerId, players, board, noArmy, tile, abilityGroup, abilityId);
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

}
