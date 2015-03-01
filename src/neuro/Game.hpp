#ifndef NEURO_GAME_HPP
#define NEURO_GAME_HPP

#include<memory>
#include<vector>
#include"neuro/Board.hpp"
#include"neuro/Player.hpp"
#include"neuro/Tile.hpp"
#include"ui/Observable.hpp"
#include"utility/DFStyleReader.hpp"
#include"utility/DFStyleCreator.hpp"

namespace neuro {

	/**
		* @brief A move one may request from a player during his turn.
		*/
	struct Move {
		/**
			* @brief Whether this move ends the turn. If true all other parameters are
			* ignored.
			*/
		bool endTurn;

		/**
			* @brief Whether to discard a tile.
			*/
		bool discard;

		/**
			* @brief The identifier of the ability to use, if discard is true everything
			* but the tile is ignored.
			*/
		neuro::AbilityIdentifier abilityIdentifier;

		/**
			* @brief Make the move represent the one described.
			* @details If this fails, the move might be in an illegal state, so this
			* should be immiediately called again, before doing anything else.
			* @param[in] input The description.
			* @return True if this operation was successfull, false otherwise.
			*/
		bool fillFromDFStyle(utility::DFStyleReader & input);

		/**
			* @brief Encode the move as DFStyle.
			* @param[out] output The encoder to which to write.
			*/
		void encodeAsDFStyle(utility::DFStyleCreator & output) const;
	};

	/**
		* @brief Represents a single targetted field.
		*/
	struct Target {
		/**
			* @brief The coordinates of the field.
			*/
		neuro::Coordinates coords;

		/**
			* @brief An orientation, if applicable for targetting.
			*/
		neuro::Orientation orientation;

		/**
			* @brief A list of tiles on the field being targetted.
			*/
		std::list< int > tiles;

		/**
			* @brief Make the target represent the one described.
			* @details If this fails, the target might be in an illegal state, so this
			* should be immiediately called again, before doing anything else.
			* @param[in] input The description.
			* @return True if this operation was successfull, false otherwise.
			*/
		bool fillFromDFStyle(utility::DFStyleReader & input);

		/**
			* @brief Encode the target as DFStyle.
			* @param[out] output The encoder to which to write.
			*/
		void encodeAsDFStyle(utility::DFStyleCreator & output) const;
	};

	using Targets = std::vector< Target > ;
	using Players = std::vector<Player>;

	/**
		* @brief A game containing some players.
		*/
	class Game : public ui::Observable<Game> {
		public:
			/**
				* @brief A default constructor crating an empty game, usually to be filled
				* from DFStyle.
				*/
			Game() : board(BoardDescription()) {}

			/**
				* @brief Create a new game from the given options.
				* @param[in] board The description of the board to use in this game.
				* @param[in] armies The armies of the players in game. Also implicitly
				* declares the number of players.
				* @param[in] initialHealth The initial health of the players.
				*/
			Game(BoardDescription board, std::vector< std::vector<neuro::Tile> > armies, int initialHealth);

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
				* @brief Whether the board is full.
				*/
			bool fullBoard() const { return board.isFull(); }

			/**
				* @brief Returns the highest initiative among the tiles on the board.
				*/
			int getMaxInitiative() const { return board.getMaxInitiative(); }

			/**
				* @brief Getter for players.
				*/
			Players & getPlayers();

			/**
				* @brief Start the next turn.
				* @return The id of the player taking the turn.
				*/
			int startTurn();

			/**
				* @brief End the current turn.
				*/
			void endTurn();

			/**
				* @brief Make the current player discard the given tile.
				* @param[in] tile The id of the tile to discard. It should be one of the
				* tiles in the player's hand.
				*/
			void discardTile(int tile);

			/**
				* @brief Place a tile on the board.
				* @param[in] tile The id of the tile to place.
				* @param[in] coords The coordinates at which to place the tile.
				* @param[in] orientation The orientation at which to place the tile.
				*/
			void placeOnBoard( int tile, Coordinates coords, Orientation orientation );

			/**
				* @brief Remove already dead tiles from the board.
				*/
			void clearDead();

			/**
				* @brief Get any activated tile on the board.
				* @return The global id of an activated tile on the board or -1 if
				* no tile is activated.
				*/
			int getActivatedTile() { return board.getActivatedTile(); }

			/**
				* @brief Remove all modifiers from the tile and then apply all proper
				* modifiers again.
				* @param[in] coords Coordinates where the tile is laying.
				* @param[in] orientation The orientation of the tile.
				* @param[in] tile The tile to update.
				*/
			void updateTile( Coordinates coords, Orientation orientation, int tile );

			/**
				* @brief Get abilities that require targets in the prebattle phase.
				* @return A two dimensional vector of ability identifiers for abilities that
				* need targets.
				*/
			std::vector< std::vector< AbilityIdentifier > > prebattleTargetsRequired();

			/**
				* @brief Get abilities that require targets in the specified battle phase.
				* @param[in] curInitiative The initiative of the battle phase for which we
				* need targets.
				* @return A two dimensional vector of ability identifiers for abilities that
				* need targets.
				*/
			std::vector< std::vector< AbilityIdentifier > > battlePhaseTargetsRequired(int curInitiative);

			/**
				* @brief Execute the ability on the targets.
				* @param[in] abilityIdentifier The ability to execute.
				* @param[in] targets Targets for the ability.
				*/
			void executeAbility( AbilityIdentifier & abilityIdentifier, Targets & targets );

			/**
				* @brief Make the game represent the one described.
				* @details If this fails, the game might be in an illegal state, so this
				* should be immiediately called again, before doing anything else.
				* @param[in] input The description.
				* @return True if this operation was successfull, false otherwise.
				*/
			bool fillFromDFStyle(utility::DFStyleReader & input);

			/**
				* @brief Encode the game as DFStyle.
				* @param[out] output The encoder to which to write.
				*/
			void encodeAsDFStyle(utility::DFStyleCreator & output) const;

			/**
				* @brief Create targets for abilities which do not require decisions.
				* @param[in] board A reference to the board.
				* @param[in] abilityIdentifier The ability for which to generate targets.
				* @return A list of targets for the ability.
				* @todo Actually do anything of the above.
				*/
			static neuro::Targets generateTargets( const neuro::Board & board, neuro::AbilityIdentifier & abilityIdentifier );

			/**
				* @brief Removes from the provided targets those, which would not be
				* affected.
				* @param[in, out] targets A list of targets to be purged.
				* @param[in] board A reference to the board.
				* @param[in] identifier The identifier of the ability to target.
				* @return If the purging was successfull true, if the targets are not
				* acceptable false.
				* @todo Actually do anything of the above.
				*/
			static bool purgeTargets( neuro::Targets & targets, const neuro::Board & board, neuro::AbilityIdentifier identifier );
		private:
			Players players;
			Board board;
			int currentPlayer;
			bool noArmy;
			int firstTurns;
			
			void removeFromBoard( int tile );

			void useModifications( int tile );

	};

}

#endif
