#include"neuro/Tile.hpp"

namespace neuro {

	int Tile::terrorEndOnPlayer = -1;
	bool Tile::battle = false;

	const std::vector< Tile::Ability > emptyAbility;
	const std::vector< Tile::Attack > emptyAttack;
	const std::vector< Tile::Modifier > emptyModifier;

	Tile::Ability createSavior( TileP sacrifice ) {
		const std::string name = "Being saved";
		const std::string description = "This tile is being saved from damage. If anything tries to damage it, another will die instead.";
		//TODO: This is a stub.
		Targetting targetting;
		std::string actions;
		return Tile::Ability( name, description, -1, targetting, actions );
	}

	const std::vector< Tile::Ability > & Tile::getOnBattleStart() const {
		if ( webbed ) {
			return emptyAbility;
		}
		return onBattleStart;
	}

	const std::vector< Tile::Attack > & Tile::getAttacks() const {
		if ( webbed ) {
			return emptyAttack;
		}
		return attacks;
	}

	const std::vector< Tile::Modifier > & Tile::getModifiers() const {
		if ( webbed ) {
			return emptyModifier;
		}
		return modifiers;
	}

	const std::vector< Tile::Ability > & Tile::getActiveAbilities() const {
		if ( webbed ) {
			return emptyAbility;
		}
		return activeAbilities;
	}

	const std::vector< Tile::Ability > & Tile::getDefensiveAbilities() const {
		if ( webbed ) {
			return emptyAbility;
		}
		return defensiveAbilities;
	}

	void Tile::Initiative::motivate() {
		if ( modifiable && !initiative.empty() ) {
			initiative.insert( *( initiative.begin() ) - 1 );
		}
	}

	void Tile::Initiative::changeInitiative( int amount, bool fix ) {
		if ( modifiable && !initiative.empty() ) {
			std::set< int > newInitiative;
			for ( int i : initiative ) {
				newInitiative.insert( i + amount );
			}
			initiative = newInitiative;
			modifiable = !fix;
		}
	}

	int Tile::Initiative::getHighestInitiative() const {
		if ( initiative.empty() ) {
			return -1;
		}
		return *(initiative.rbegin());
	}

	bool Tile::Placing::placeTile( std::list< TileP > targets ) {
		bool stayOnBoard = false;
		for ( char action : placeActions ) {
			switch ( action ) {
				case 'P':
					stayOnBoard	= true;
					break;
				case 'd':
					for ( TileP trgt : targets ) {
						trgt->dealDamage( 1 );
					}
					break;
				case 'm':
					for ( TileP trgt : targets ) {
						trgt->move();
					}
					break;
				case 'p':
					for ( TileP trgt : targets ) {
						trgt->push( parent.lock() );
					}
					break;
				case 'b':
					startBattle();
					break;
			}
		}
		return stayOnBoard;
	}

	void Tile::Ability::useAbility( std::list< TileP > targets ) {
		for ( auto action = abilityActions.begin(); action != abilityActions.end(); action++ ) {
			switch ( *action ) {
				//TODO: Nothing here yet, cause we have no nondefensive abilities.
			}
		}
	}

	void Tile::Ability::useDefensiveAbility( int & damage, bool ranged ) {
		for ( auto action = abilityActions.begin(); action != abilityActions.end(); action++ ) {
			switch ( *action ) {
				case 'p':
					action++;
					int amount = (*action) - '0';
					action++;
					if ( *action == 'r' && ranged ) {
						damage -= amount;
					}
					break;
			}
		}
	}

	void Tile::Attack::executeAttack( std::list< TileP > targets ) {
		for ( auto action = attackActions.begin(); action != attackActions.end(); action++ ) {
			switch ( *action ) {
				case 'd':
					for ( TileP trgt : targets ) {
						trgt->dealDamage( strength, direction, ranged );
					}
					break;
				case 'b':
					action++;
					switch ( *action ) {
						case 'r':
							ranged = true;
							break;
						case 'R':
							ranged = false;
							break;
					}
					break;
				case 'a':
					parent.lock()->destroy( true );
					break;
			}
		}
	}

	void Tile::Modifier::modifyTiles( std::list< TileP > targets ) {
		for ( auto action = modifyActions.begin(); action != modifyActions.end(); action++ ) {
			switch ( *action ) {
				case 'w':
					for ( TileP trgt : targets ) {
						trgt->web();
					}
					break;
				case 'i': {
															action++;
															int amount = *action - '0';
															action++;
															switch ( *action ) {
																case 'i':
																	for ( TileP trgt : targets ) {
																		trgt->changeInitiative( amount );
																	}
																	break;
																case 'm':
																	for ( TileP trgt : targets ) {
																		trgt->changeMelee( amount );
																	}
																	break;
																case 'r':
																	for ( TileP trgt : targets ) {
																		trgt->changeRanged( amount );
																	}
																	break;
															}
															break;
														}
				case 's': {
															Tile::Ability savior = createSavior( parent.lock() );
															for ( TileP trgt : targets ) {
																trgt->addDefensiveAbility( savior );
															}
															break;
														}
				case 'm':
														for ( TileP trgt : targets ) {
															trgt->motivate();
														}
														break;
			}
		}
	}

	void Tile::setParents( TileP thisTile ) {
		thisP = thisTile;
		placing.setParent( thisTile );
		for ( auto atk : attacks ) {
			atk.setParent( thisTile );
		}
		for ( auto mdfr : modifiers ) {
			mdfr.setParent( thisTile );
		}
	}

	int Tile::getHighestInitiative() const {
		if ( webbed ) {
			return -1;
		}
		return initiative.getHighestInitiative();
	}

	void Tile::dealDamage( int strength, int direction, bool ranged ) {
		for ( Ability & def : defensiveAbilities ) {
			if ( def.getDirection() == ( ( direction + 3 ) % 6 ) || def.getDirection() == -1 ) {
				def.useDefensiveAbility( strength, ranged );
			}
		}
		if ( strength > 0 ) {
			life.dealDamage( strength );
		}
	}

	void Tile::destroy( bool noRedirect ) {
		const int infiniteDamage = 100;
		if ( noRedirect ) {
			life.dealDamage( infiniteDamage ); // Pretty much infinity.
		} else {
			dealDamage( infiniteDamage ); // Let the dealing function handle defenses.
		}
	}

	void Tile::move() {
		//TODO: This is a stub.
	}

	void Tile::push( TileP source ) {
		//TODO: This is a stub.
	}

	void Tile::startBattle() {
		battle = true;
	}

	void Tile::web() {
		webbed = true;
	}

	void Tile::motivate() {
		initiative.motivate();
	}

	void Tile::changeInitiative( int amount, bool fix ) {
		initiative.changeInitiative( amount, fix );
	}

	void Tile::changeMelee( int amount ) {
		for ( Attack & atk : attacks ) {
			if ( atk.isMelee() ) {
				atk.modifyStrength( amount );
			}
		}
	}

	void Tile::changeRanged( int amount ) {
		for ( Attack & atk : attacks ) {
			if ( atk.isRanged() ) {
				atk.modifyStrength( amount );
			}
		}
	}

	void Tile::addDefensiveAbility( Ability def ) {
		defensiveAbilities.push_back(def);
	}

}
