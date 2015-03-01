#ifndef NEUROSERVER_ARBITER_HPP
#define NEUROSERVER_ARBITER_HPP

#include<memory>
#include<vector>
#include"neuro/Board.hpp"
#include"neuro/Player.hpp"
#include"neuro/Game.hpp"
#include"neuroServer/Contestant.hpp"

namespace neuroServer {

	using Players = std::vector< neuro::Player >;

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
				* @param[in] game A reference to the game.
				* @return A validated move the contestant wants to make.
				* @todo Actually validate the moves.
				*/
			neuro::Move getMove(int playerId, const neuro::Game & game);

			/**
				* @brief Returns a validated list of targets the contestant wants to affect
				* with the specified ability.
				* @param[in] playerId The id of the Player the contestant controls.
				* @param[in] game A reference to the game.
				* @param[in] abilityIdentifier The ability for which the target is being
				* requested.
				* @return A validated list of targets.
				* @todo Actually validate the targets.
				*/
			neuro::Targets getTargets(int playerId, const neuro::Game & game, neuro::AbilityIdentifier & abilityIdentifier );

			/**
				* @brief Returns a tile the contestant wants to discard.
				* @param[in] playerId The id of the Player the contestant controls.
				* @param[in] game A reference to the game.
				* @return The global ID of the tile the contestant wants to discard.
				* @todo Actually validate the tile.
				*/
			int getDiscard(int playerId, const neuro::Game & game);

			/**
				* @brief Starts requesting targets for a list of abilities from the
				* contestant until it gets valid ones. Does not block.
				* @param[in] playerId The id of the Player the contestant controls.
				* @param[in] game A reference to the game.
				* @param[in] abilities The abilities for whose targets to ask.
				* @todo This is a stub, implement.
				*/
			void requestTargetsForAbilities( int playerId, const neuro::Game & game, std::vector< neuro::AbilityIdentifier > abilities );

			/**
				* @brief Get the targets for one of previously requested abilities.
				* @param[in] id The id of the ability for which we want the targets. It is
				* the index of it in the vector provided for the request earlier.
				* @return A validated list of targets for the ability. If no targets were
				* earlier requested or id does not describe one of them the behaviour is
				* undefined.
				* @todo This is a stub, implement.
				*/
			neuro::Targets getTargetsForAbility( int id );
		private:
			ContestantP contestant;
	};

}

#endif
