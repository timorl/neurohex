#ifndef NEUROSERVER_GAME_HPP
#define NEUROSERVER_GAME_HPP

#include<memory>
#include<vector>
#include"neuro/Board.hpp"
#include"neuro/Player.hpp"
#include"neuro/Tile.hpp"
#include"neuroServer/Arbiter.hpp"
#include"neuroServer/Contestant.hpp"
#include"neuroServer/GameOptions.hpp"
#include"ui/Observable.hpp"
#include"utility/DFStyleReader.hpp"
#include"utility/DFStyleCreator.hpp"

namespace neuroServer {

	using Players = std::vector<neuro::Player>;
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
			void updateTile( neuro::Coordinates coords, neuro::Orientation orientation, int tile );

			/**
				* @brief Encode the game as DFStyle.
				* @param[out] output The encoder to which to write.
				*/
			void encodeAsDFStyle(utility::DFStyleCreator & output) const;
		private:
			Arbiters arbiters;
			Players players;
			neuro::Board board;
			int currentPlayer;
			bool noArmy;

			void removeFromBoard( int tile );
			void placeOnBoard( int tile, neuro::Coordinates coords, neuro::Orientation orientation );

			void tilePlacing( int tile );
			void executeAbility( neuro::AbilityIdentifier & abilityIdentifier, Targets & targets );
			void abilityUsing( neuro::AbilityIdentifier & abilityIdentifier );
			void useModifications( int tile );
			void clearDead();
			void battleStart();
			void battlePhase( int initiative );
			void runBattle();
			void resolveActivated();
	};

}

#endif
