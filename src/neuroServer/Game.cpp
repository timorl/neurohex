#include"neuroServer/Game.hpp"

namespace neuroServer {

	Game::Game(GameOptions options) :
		game(options.board, options.armies, options.initialHealth) {
		for ( auto & c : options.contestants ) {
			arbiters.emplace_back(c);
		}
	}

	void Game::play() {
		while ( !game.isFinished() ) {
			int currentPlayer = game.startTurn();
			neuro::Move move;
			do {
				move = arbiters[currentPlayer].getMove(currentPlayer, game);
				if ( !move.endTurn ) {
					if ( move.discard ) {
						game.discardTile(move.abilityIdentifier.tile);
					} else if ( move.abilityIdentifier.group == neuro::AbilityGroup::PLACING ) {
						tilePlacing(move.abilityIdentifier.tile);
					} else {
						abilityUsing(move.abilityIdentifier);
					}
					resolveActivated();
				}
				while ( neuro::Tile::battle ) {
					move.endTurn = true;
					runBattle();
					if ( !game.fullBoard() ) {
						neuro::Tile::battle = false;
					}
				}
			} while ( !move.endTurn );
			game.endTurn();
		}
		runBattle(); //The final battle.
	}

	void Game::abilityUsing( neuro::AbilityIdentifier & abilityIdentifier ) {
		int abilityUser = neuro::Tile::allTiles[abilityIdentifier.tile].getController();
		neuro::Targets targets = arbiters[abilityUser].getTargets(abilityUser, game, abilityIdentifier);
		game.executeAbility(abilityIdentifier, targets);
	}

	void Game::resolveActivated() {
		int tile = game.getActivatedTile();
		while ( tile != -1 ) {
			neuro::AbilityIdentifier ai = neuro::Tile::allTiles[tile].getActivatedAbility();
			abilityUsing( ai );
			tile = game.getActivatedTile();
		}
	}

	void Game::tilePlacing(int tile) {
		int tilePlacer = neuro::Tile::allTiles[tile].getController();
		neuro::AbilityIdentifier ai;
		ai.tile = tile;
		ai.group = neuro::AbilityGroup::PLACING;
		ai.id = 0;
		neuro::Targets targets = arbiters[tilePlacer].getTargets( tilePlacer, game, ai );
		std::list< int > affectedTiles;
		neuro::Target curTarget;
		for ( auto tpl : targets ) {
			curTarget = tpl;
			affectedTiles.splice( affectedTiles.end(), curTarget.tiles );
		}
		neuro::Tile::Ability & placing = neuro::Tile::allTiles[tile].getPlacing();
		if ( placing.placeTile(affectedTiles) ) {
			game.placeOnBoard(tile, curTarget.coords, curTarget.orientation);
		}
	}

	void Game::runBattle() {
		prebattle();
		int curInitiative = game.getMaxInitiative();
		while ( curInitiative >= 0 ) {
			battlePhase( curInitiative );
			curInitiative--;
		}
	}

	void Game::prebattle() {
		std::vector< std::vector< neuro::AbilityIdentifier > > toExecute = game.prebattleTargetsRequired();
		for ( int i = 0; i < game.getNumberOfPlayers(); i++ ) {
			arbiters[i].requestTargetsForAbilities( i, game, toExecute[i] );
		}
		for ( int i = 0; i < game.getNumberOfPlayers(); i++ ) {
			for ( int j = 0; j < static_cast<int>( toExecute[i].size() ); j++ ) {
				neuro::Targets targets = arbiters[i].getTargetsForAbility( j );
				game.executeAbility( toExecute[i][j], targets );
			}
		}
		game.clearDead();
	}

	void Game::battlePhase(int initiative) {
		std::vector< std::vector< neuro::AbilityIdentifier > > toExecute = game.battlePhaseTargetsRequired(initiative);
		for ( int i = 0; i < game.getNumberOfPlayers(); i++ ) {
			arbiters[i].requestTargetsForAbilities( i, game, toExecute[i] );
		}
		for ( int i = 0; i < game.getNumberOfPlayers(); i++ ) {
			for ( int j = 0; j < static_cast<int>( toExecute[i].size() ); j++ ) {
				neuro::Targets targets = arbiters[i].getTargetsForAbility( j );
				game.executeAbility( toExecute[i][j], targets );
			}
		}
		game.clearDead();
	}

}
