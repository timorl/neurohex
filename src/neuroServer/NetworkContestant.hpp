#ifndef NEUROSERVER_NETWORK_CONTESTANT_HPP
#define NEUROSERVER_NETWORK_CONTESTANT_HPP

#include<memory>
#include<vector>
#include"neuro/Game.hpp"
#include"utility/DFStyleReader.hpp"
#include"utility/DFStyleCreator.hpp"
#include"neuroServer/Contestant.hpp"
#include"network/Acceptor.hpp"

namespace neuroServer {

	enum class RequestState {
		NONE,
		IN_PROGRESS,
		DONE
	};

	/**
		* @brief A wrapper of a network connection as a contestant.
		*/
	class NetworkContestant : public Contestant {
		public:
			/**
				* @brief Create a wrapper.
				* @param[in] acceptor The acceptor from which to get the connection to wrap.
				*/
			NetworkContestant(network::Acceptor & acceptor) : acceptor(acceptor) {}

			/**
				* @brief Get the connection from the acceptor to be the contestant.
				* Nonblocking.
				*/
			void requestContestant();

			/**
				* @brief Make the contestant choose the initial health. Nonblocking.
				*/
			void requestInitialHealthChoice();

			/**
				* @brief Get the initial health the contestant chose. Blocks if no choice is available
				* yet and requests one if it hasn't been requested yet.
				* @returns The amount of health players should initially have.
				*/
			int getInitialHealthChoice();

			/**
				* @brief Make the contestant choose a board. Nonblocking.
				* @param[in] boards The boards from which to choose.
				*/
			void requestBoardChoice(const std::map<std::string, std::string> & boards);

			/**
				* @brief Get the board the contestant chose. Blocks if no choice is available
				* yet and requests one if it hasn't been requested yet.
				* @param[in] boards The boards from which to choose.
				* @return The name of the board chosen.
				*/
			std::string getBoardChoice(const std::map<std::string, std::string> & boards);

			/**
				* @brief Make the contestant choose an army. Nonblocking.
				* @param[in] armies The armies from which to choose.
				*/
			void requestArmyChoice(const std::map<std::string, std::string> & armies);

			/**
				* @brief Get the army the contestant chose. Blocks if no choice is available
				* yet and requests one if it hasn't been requested yet.
				* @param[in] armies The armies from which to choose.
				* @return The name of the army chosen.
				*/
			std::string getArmyChoice(const std::map<std::string, std::string> & armies);

			/**
				* @brief Request a move from the contestant. Nonblocking.
				* @param[in] playerId The id of the contestant.
				* @param[in] game A reference to the game.
				*/
			void requestMove(int playerId, const neuro::Game & game);

			/**
				* @brief Get a move from the contestant. Blocks if no move is available
				* yet and requests one if it hasn't been requested yet.
				* @param[in] playerId The id of the contestant.
				* @param[in] game A reference to the game.
				* @return A nonvalidated move send by the contestant.
				*/
			neuro::Move getMove(int playerId, const neuro::Game & game);

			/**
				* @brief Request a list of targets for a number of specific abilities from the contestant. Nonblocking.
				* @param[in] playerId The id of the contestant.
				* @param[in] game A reference to the game.
				* @param[in] abilities A vector of abilities we need targets for.
				*/
			void requestTargets(int playerId, const neuro::Game & game, std::vector<neuro::AbilityIdentifier> abilities);

			/**
				* @brief Get a list of targets for a specific ability from the contestant. Blocks if no choice is available yet and requests one if it hasn't been requested yet.
				* @param[in] playerId The id of the contestant.
				* @param[in] game A reference to the game.
				* @param[in] abilities A vector of abilities we need targets for.
				* @param[in] id The id of the ability we want the targets for. Should be
				* contained in the vector.
				* @return A nonvalidated set of targets. The behaviour is undefined if the
				* id is out of bounds.
				*/
			neuro::Targets getTargets(int playerId, const neuro::Game & game, std::vector<neuro::AbilityIdentifier> abilities);

			/**
				* @brief Request the contestant to choose a tile to discard. Nonblocking.
				* @param[in] playerId The id of the contestant.
				* @param[in] game A reference to the game.
				* @return A nonvalidated number referencing the tile in the hand to discard.
				*/
			void requestDiscard(int playerId, const neuro::Game & game);

			/**
				* @brief Get a the id of the tile to discard from the contestant. Blocks if no choice is available yet and requests one if it hasn't been requested yet.
				* @param[in] playerId The id of the contestant.
				* @param[in] game A reference to the game.
				* @return A nonvalidated number referencing the tile in the hand to discard.
				*/
			int getDiscard(int playerId, const neuro::Game & game);
		private:
			network::Acceptor & acceptor;

			std::shared_ptr<network::Connection> contestant;

			void requestNick();
			RequestState	nickRequested;
			std::string nick;

			void reestablishContestant();
			bool validateContestant();

			void initialHealthRequestResponseHandler(std::string resp);
			RequestState initialHealthRequested;
			int initialHealthChoice;

			void boardRequestResponseHandler(std::string resp);
			RequestState	boardRequested;
			std::string board;

			void armyRequestResponseHandler(std::string resp);
			RequestState	armyRequested;
			std::string army;

			void moveRequestResponseHandler(std::string resp);
			RequestState	moveRequested;
			neuro::Move move;

			void targetsRequestResponseHandler(std::string resp);
			RequestState	targetsRequested;
			neuro::Targets targets;

			void discardRequestResponseHandler(std::string resp);
			RequestState	discardRequested;
			int discard;
	};

	using NetworkContestantP = std::shared_ptr<NetworkContestant>;

}

#endif
