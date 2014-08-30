#ifndef NEURO_ARBITER_HPP
#define NEURO_ARBITER_HPP

#include<memory>
#include<vector>
#include"neuro/Board.hpp"
#include"neuro/Player.hpp"
#include"neuro/Contestant.hpp"

namespace neuro {

	using Players = std::vector< PlayerP >;

	/**
		* @brief The entity relaying instructions from the Contestant to the Game.
		*/
	class Arbiter {
		public:
			/**
				* @brief Constructs an arbiter for the given contestant.
				* @param[in] contestant The contestant for which we need an arbiter.
				*/
			Arbiter(ContestantP contestant) : contestant(contestant) {}

			/**
				* @brief Returns a validated move the contestant wants to make.
				* @param[in] playerId The id of the Player the contestant controls.
				* @param[in] players A reference to a std::vector of players in the game.
				* @param[in] board A reference to the board.
				* @param[in] noArmy Whether any players army already ran out.
				* @return A validated move the contestant wants to make.
				* @todo Actually validate the moves.
				*/
			Move getMove( int playerId, const Players & players, const Board & board, bool noArmy );

			/**
				* @brief Returns a validated list of targets the contestant wants to affect
				* with the specified ability.
				* @param[in] playerId The id of the Player the contestant controls.
				* @param[in] players A reference to a std::vector of players in the game.
				* @param[in] board A reference to the board.
				* @param[in] noArmy Whether any players army already ran out.
				* @param[in] tile The tile with the ability to target.
				* @param[in] abilityGroup In which ability group to look for the ability.
				* @param[in] abilityId The id of the ability to target.
				* @return A validated list of targets.
				* @todo Actually validate the targets.
				*/
			Targets getTargets( int playerId, const Players & players, const Board & board, bool noArmy, const Tile & tile, AbilityGroup abilityGroup, int abilityId );

			/**
				* @brief Returns a tile the contestant wants to discard.
				* @param[in] playerId The id of the Player the contestant controls.
				* @param[in] players A reference to a std::vector of players in the game.
				* @param[in] board A reference to the board.
				* @param[in] noArmy Whether any players army already ran out.
				* @return A pointer to the tile the contestant wants to discard.
				* @todo Actually validate the tile.
				*/
			TileP requestDiscard( int playerId, const Players & players, const Board & board, bool noArmy );

			/**
				* @brief Starts requesting targets for a list of abilities from the
				* contestant until it gets valid ones. Does not block.
				* @param[in] playerId The id of the Player the contestant controls.
				* @param[in] players A reference to a std::vector of players in the game.
				* @param[in] board A reference to the board.
				* @param[in] noArmy Whether any players army already ran out.
				* @param[in] abilities The abilities for whose targets to ask.
				* @todo This is a stub, implement.
				*/
			void requestTargetsForAbilities( int playerId, const Players & players, const Board & board, bool noArmy, std::vector< AbilityIdentifier > abilities );

			/**
				* @brief Get the targets for one of previously requested abilities.
				* @param[in] id The id of the ability for which we want the targets. It is
				* the index of it in the vector provided for the request earlier.
				* @return A validated list of targets for the ability. If no targets were
				* earlier requested or id does not describe one of them the behaviour is
				* undefined.
				* @todo This is a stub, implement.
				*/
			Targets getTargetsForAbility( int id );
		private:
			ContestantP contestant;
	};

}

#endif
