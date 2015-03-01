#ifndef NEUROSERVER_GAME_HPP
#define NEUROSERVER_GAME_HPP

#include<memory>
#include<vector>
#include"neuro/Tile.hpp"
#include"neuro/Game.hpp"
#include"neuroServer/Arbiter.hpp"
#include"neuroServer/Contestant.hpp"
#include"neuroServer/GameOptions.hpp"
#include"ui/Observable.hpp"
#include"utility/DFStyleReader.hpp"
#include"utility/DFStyleCreator.hpp"

namespace neuroServer {

	using Arbiters = std::vector<Arbiter>;
	using Contestants = std::vector<ContestantP>;

	/**
		* @brief A game containing some players and controlling the flow.
		*/
	class Game : public ui::Observable<Game> {
		public:
			/**
				* @brief Create a game with the given options.
				* @param[in] options A struct with all the needed options.
				*/
			Game(GameOptions options);

			/**
				* @brief Start the game and play it to the end.
				*/
			void play();
		private:
			Arbiters arbiters;
			neuro::Game game;

			void resolveActivated();
			void abilityUsing(neuro::AbilityIdentifier & abilityIdentifier);
			void tilePlacing(int tile);
			void runBattle();
			void prebattle();
			void battlePhase(int initiative);

	};

}

#endif
