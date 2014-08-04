#ifndef NEURO_GAME_HPP
#define NEURO_GAME_HPP

#include<memory>
#include<vector>
#include"neuro/Arbiter.hpp"
#include"neuro/Contestant.hpp"
#include"neuro/Player.hpp"
#include"neuro/Board.hpp"
#include"ui/Observable.hpp"

namespace neuro {

	using ContestantP = std::shared_ptr<Contestant>;
	using PlayerP = std::shared_ptr<Player>;
	using Players = std::vector<PlayerP>;
	using Arbiters = std::vector<Arbiter>;
	using Contestants = std::vector<ContestantP>;

	/**
		* @brief Options needed to create a new Game.
		*/
	struct GameOptions {
		/**
			* @brief Pointers to contestants playing in order of players.
			*/
		Contestants contestants;

		/**
			* @brief Pointers to armies of players in order of players.
			*/
		std::vector< ArmyP > armies;

		/**
			* @brief The health all the players will be starting with.
			*/
		int initialHealth = 20;

		/**
			* @brief The description of the Board to play on.
			*/
		BoardDescription board;
	};

	/**
		* @brief A game containing some players and controlling the flow.
		* @todo Battles should happen.
		*/
	class Game : public ui::Observable<Game> {
		public:
			/**
				* @brief Create a game with the given options.
				* @param[in] options A struct with all the needed options.
				*/
			Game(GameOptions options);

			/**
				* @brief Returns the number of players in the game.
				*/
			int getNumberOfPlayers() const { return players.size(); }

			/**
				* @brief Returns the number of players still alive in the game.
				*/
			int getNumberOfLivingPlayers() const;

			/**
				* @brief Returns the id of the player currently playing.
				*/
			int getCurrentPlayer() const { return currentPlayer; }

			/**
				* @brief Whether the current game is finished.
				*/
			bool isFinished() const { return finished; }

			/**
				* @brief Start the game and play it to the end.
				* @todo Actually playing tiles and using already played ones. Also, battles.
				*/
			void play();
		private:
			Arbiters arbiters;
			Players players;
			Board board;
			int currentPlayer;
			bool noArmy;
			bool finished;
	};

}

#endif
