#include"neuro/Arbiter.hpp"

namespace neuro {

	Move Arbiter::getMove(int playerId, const Players & players, const Board & board, bool noArmy) {
		//TODO: Actually validate the move.
		return contestant->getMove(playerId, players, board, noArmy);
	}

}
