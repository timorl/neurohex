#ifndef NEURO_GAME_HPP
#define NEURO_GAME_HPP

#include<memory>
#include<vector>
#include"neuro/Arbiter.hpp"
#include"neuro/Board.hpp"
#include"neuro/Contestant.hpp"
#include"neuro/GameOptions.hpp"
#include"neuro/Player.hpp"
#include"neuro/Tile.hpp"
#include"ui/Observable.hpp"

namespace neuro {

	using Players = std::vector<PlayerP>;
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
			bool isFinished() const;

			/**
				* @brief Start the game and play it to the end.
				*/
			void play();

			/**
				* @brief Remove all modifiers from the tile and then apply all proper
				* modifiers again.
				* @param[in] coords Coordinates where the tile is laying.
				* @param[in] orientation The orientation of the tile.
				* @param[in] tile The tile to update.
				*/
			void updateTile( Coordinates coords, Orientation orientation, TileP tile );
		private:
			Arbiters arbiters;
			Players players;
			Board board;
			int currentPlayer;
			bool noArmy;

			void removeFromBoard( TileP tile );
			void placeOnBoard( TileP tile, Coordinates coords, Orientation orientation );

			void tilePlacing( TileP tile );
			void executeAbility( AbilityIdentifier & abilityIdentifier, Targets & targets );
			void abilityUsing( AbilityIdentifier & abilityIdentifier );
			void useModifications( TileP tile );
			void clearDead();
			void battleStart();
			void battlePhase( int initiative );
			void runBattle();
			void resolveActivated();
	};

	using GameP = std::shared_ptr< Game >;

}

#endif
