#include"neuro/Tile.hpp"

namespace neuro {

	int Tile::terrorEndOnPlayer = -1;
	bool Tile::battle = false;

	bool isMelee( Tile::Ability atk ) {
		return ( atk.getActionString()[0] == 'm' );
	}

	bool isRanged( Tile::Ability atk ) {
		return ( atk.getActionString()[0] == 'r' );
	}

	Tile::Ability createSavior( TileP sacrifice, TileP recipient ) {
		const std::string name = "Being saved";
		const std::string description = "This tile is being saved from damage. If anything tries to damage it, another will die instead.";
		//TODO: This is a stub.
		Targetting targetting;
		std::string actions;
		int saviorId = recipient->getDefensiveAbilities().size();
		AbilityGroup	group = AbilityGroup::DEFENSIVE;
		return Tile::Ability( name, description, -1, targetting, 0, actions, saviorId, group );
	}

	bool AbilityIdentifier::operator<( const AbilityIdentifier & other ) const {
		if ( tile != other.tile ) {
			return tile < other.tile;
		}
		if ( group != other.group ) {
			return group < other.group;
		}
		if ( id != other.id ) {
			return id < other.id;
		}
		return false;
	}

	const Tile::Abilities & Tile::getOnBattleStart() const {
		return onBattleStart;
	}

	const Tile::Abilities & Tile::getAttacks() const {
		return attacks;
	}

	const Tile::Abilities & Tile::getModifiers() const {
		return modifiers;
	}

	const Tile::Abilities & Tile::getActiveAbilities() const {
		return activeAbilities;
	}

	const Tile::Abilities & Tile::getDefensiveAbilities() const {
		return defensiveAbilities;
	}

	void Tile::Initiative::motivate() {
		if ( modifiable && !initiative.empty() ) {
			initiative.insert( *( initiative.begin() ) - 1 );
		}
	}

	void Tile::Initiative::demotivate() {
		initiative.erase( initiative.begin() );
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

	AbilityIdentifier	Tile::Ability::getIdentifier() const {
		AbilityIdentifier	ai;
		ai.tile = parent.lock();
		ai.id = id;
		ai.group = group;
		return ai;
	}

	bool Tile::Ability::placeTile( std::list< TileP > targets ) {
		bool stayOnBoard = false;
		for ( auto action = abilityActions.begin(); action != abilityActions.end(); action++ ) {
			switch ( *action ) {
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

	bool Tile::Ability::useAbility( std::list< TileP > targets ) {
		bool placeTile = false;
		for ( auto action = abilityActions.begin(); action != abilityActions.end(); action++ ) {
			switch ( *action ) {
				//TODO: Add abilities as we go along.
			}
		}
		return placeTile;
	}

	void Tile::Ability::useDefensiveAbility( int & damage, bool ranged ) {
		for ( auto action = abilityActions.begin(); action != abilityActions.end(); action++ ) {
			switch ( *action ) {
				case 'r':
					int amount = strength;
					if ( ranged ) {
						damage -= amount;
					}
					break;
			}
		}
	}

	void Tile::Ability::executeAttack( std::list< TileP > targets ) {
		for ( auto action = abilityActions.begin(); action != abilityActions.end(); action++ ) {
			switch ( *action ) {
				case 'r':
					for ( TileP trgt : targets ) {
						trgt->dealDamage( strength, direction, true );
					}
					break;
				case 'g': //For Gauss gun -- ranged damage, but not ranged attack.
					for ( TileP trgt : targets ) {
						trgt->dealDamage( strength, direction, true );
					}
					break;
				case 'd':
					for ( TileP trgt : targets ) {
						trgt->dealDamage( strength, direction, false );
					}
					break;
				case 'm':
					for ( TileP trgt : targets ) {
						trgt->dealDamage( strength, direction, false );
					}
					break;
				case 'a':
					parent.lock()->destroy( true );
					break;
			}
		}
	}

	void Tile::Ability::modifyTiles( std::list< TileP > targets ) {
		AbilityIdentifier ai = getIdentifier();
		for ( auto action = abilityActions.begin(); action != abilityActions.end(); action++ ) {
			switch ( *action ) {
				case 'w':
					for ( TileP trgt : targets ) {
						ai.tile->addModified( trgt );
						trgt->web( ai );
					}
					break;
				case 'i':
					for ( TileP trgt : targets ) {
						ai.tile->addModified( trgt );
						trgt->changeInitiative( ai, strength );
					}
					break;
				case 'm':
					for ( TileP trgt : targets ) {
						ai.tile->addModified( trgt );
						trgt->changeMelee( ai, strength );
					}
					break;
				case 'r':
					for ( TileP trgt : targets ) {
						ai.tile->addModified( trgt );
						trgt->changeRanged( ai, strength );
					}
					break;
				case 's':
					{
						for ( TileP trgt : targets ) {
							ai.tile->addModified( trgt );
							trgt->addDefensiveAbility( ai, createSavior( parent.lock(), trgt ) );
						}
						break;
					}
				case 'M':
					for ( TileP trgt : targets ) {
						ai.tile->addModified( trgt );
						trgt->motivate( ai );
					}
					break;
			}
		}
	}

	void Tile::Ability::demodifyTiles( std::list< TileP > targets ) {
		AbilityIdentifier ai = getIdentifier();
		for ( auto action = abilityActions.begin(); action != abilityActions.end(); action++ ) {
			switch ( *action ) {
				case 'w':
					for ( TileP trgt : targets ) {
						ai.tile->delModified( trgt );
						trgt->deweb( ai );
					}
					break;
				case 'i':
					for ( TileP trgt : targets ) {
						ai.tile->delModified( trgt );
						trgt->dechangeInitiative( ai, strength );
					}
					break;
				case 'm':
					for ( TileP trgt : targets ) {
						ai.tile->delModified( trgt );
						trgt->dechangeMelee( ai, strength );
					}
					break;
				case 'r':
					for ( TileP trgt : targets ) {
						ai.tile->delModified( trgt );
						trgt->dechangeRanged( ai, strength );
					}
					break;
				case 's':
					{
						for ( TileP trgt : targets ) {
							ai.tile->delModified( trgt );
							trgt->delDefensiveAbility( ai );
						}
						break;
					}
				case 'M':
					for ( TileP trgt : targets ) {
						ai.tile->delModified( trgt );
						trgt->demotivate( ai );
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

	void Tile::clearModifications() {
		std::list< TileP > toDemod;
		toDemod.push_back( thisP.lock() );
		for ( auto modif : modifications ) {
			modif.first.tile->getModifier( modif.first.id ).demodifyTiles( toDemod );
		}
	}

	void Tile::stopModifying() {
		std::list< TileP > toDemod( modifieds.begin(), modifieds.end() );
		for ( Ability & mod : modifiers ) {
			mod.demodifyTiles( toDemod );
		}
	}

	void Tile::addModified( TileP modified ) {
		modifieds.insert( modified );
	}

	void Tile::delModified( TileP modified ) {
		modifieds.erase( modified );
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

	void Tile::web( AbilityIdentifier ai ) {
		AbilityIdentifier	target;
		target.tile = thisP.lock();
		target.id = -1;
		modifications[ai].insert(target);
		webbed++;
	}

	void Tile::deweb( AbilityIdentifier ai ) {
		AbilityIdentifier	target;
		target.tile = thisP.lock();
		target.id = -1;
		if ( modifications.count( ai ) > 0 ) {
			modifications[ai].erase( target );
			webbed--;
		}
	}

	void Tile::motivate( AbilityIdentifier ai ) {
		AbilityIdentifier	target;
		target.tile = thisP.lock();
		target.id = -1;
		modifications[ai].insert(target);
		initiative.motivate();
	}

	void Tile::demotivate( AbilityIdentifier ai ) {
		AbilityIdentifier	target;
		target.tile = thisP.lock();
		target.id = -1;
		if ( modifications.count( ai ) > 0 ) {
			modifications[ai].erase( target );
			initiative.demotivate();
		}
	}

	void Tile::changeInitiative( AbilityIdentifier ai, int amount, bool fix ) {
		AbilityIdentifier	target;
		target.tile = thisP.lock();
		target.id = -1;
		modifications[ai].insert(target);
		initiative.changeInitiative( amount, fix );
	}

	void Tile::dechangeInitiative( AbilityIdentifier ai, int amount, bool fix ) {
		AbilityIdentifier	target;
		target.tile = thisP.lock();
		target.id = -1;
		if ( modifications.count( ai ) > 0 ) {
			modifications[ai].erase( target );
			initiative.changeInitiative( -amount, fix );
		}
	}

	void Tile::changeMelee( AbilityIdentifier ai, int amount ) {
		for ( Ability & atk : attacks ) {
			if ( isMelee( atk ) ) {
				AbilityIdentifier	target = atk.getIdentifier();
				modifications[ai].insert(target);
				atk.modifyStrength( amount );
			}
		}
	}

	void Tile::dechangeMelee( AbilityIdentifier ai, int amount ) {
		for ( AbilityIdentifier ident : modifications[ai] ) {
			Ability & atk = getAttack( ident.id );
			if ( isMelee( atk ) ) {
				atk.modifyStrength( -amount );
				modifications[ai].erase( ident );
			}
		}
	}

	void Tile::changeRanged( AbilityIdentifier ai, int amount ) {
		for ( Ability & atk : attacks ) {
			if ( isRanged( atk ) ) {
				AbilityIdentifier	target = atk.getIdentifier();
				modifications[ai].insert(target);
				atk.modifyStrength( amount );
			}
		}
	}

	void Tile::dechangeRanged( AbilityIdentifier ai, int amount ) {
		for ( AbilityIdentifier ident : modifications[ai] ) {
			Ability & atk = getAttack( ident.id );
			if ( isRanged( atk ) ) {
				atk.modifyStrength( -amount );
				modifications[ai].erase( ident );
			}
		}
	}

	void Tile::addDefensiveAbility( AbilityIdentifier ai, Ability def ) {
		AbilityIdentifier	target = def.getIdentifier();
		modifications[ai].insert(target);
		defensiveAbilities.push_back(def);
	}

	void Tile::delDefensiveAbility( AbilityIdentifier ai ) {
		for ( AbilityIdentifier ident : modifications[ai] ) {
			defensiveAbilities[ident.id].neutralize();
		}
		modifications[ai].clear();
	}

}
