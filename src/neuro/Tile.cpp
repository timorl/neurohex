#include"neuro/Tile.hpp"

namespace neuro {

	int Tile::terrorEndOnPlayer = -1;
	bool Tile::battle = false;

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

	void Tile::setParents( TileP thisTile ) {
		placing.setParent( thisTile );
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

	void Tile::move() {
		//TODO: This is a stub.
	}

	void Tile::push( TileP source ) {
		//TODO: This is a stub.
	}

	void Tile::startBattle() {
		battle = true;
	}

}
