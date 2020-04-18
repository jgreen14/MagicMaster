#include <iostream>
#include "engine.h"

//Create a unit based on the passed unit's name.
//This function simply sets up and executes the query.
//The actual setting of the attributes is done in unit_callback.
void create_unit (Unit *unit, sqlite3* sqlite)
{
	string query = "select * from CHARACTERS where name='";
	query.append(unit->get_name().c_str());
	query.append("';");
	sqlite3_exec(sqlite, query.c_str(), create_callback, static_cast<void*>(unit), NULL);
}//create_unit

//Callback for the sqlite3_exec in create_unit.
//The first parameter is a pointer to the unit we want to create, so we cast it from
//void* to Unit*.
//Then we set all unit attributes based on the order of the columns from our query.
int create_callback(void *new_unit, int argc, char **argv, char **azColName)
{
	Unit* unit = static_cast<Unit*>(new_unit);

	/* The order of the columns returned by the query is:
	 * id, name, attack, ranged, defense, resist, hits, moves, movetype, troops
	 *
	 * We don't care about the id and the name is already set so we just worry
	 * about the remaining 8 attributes
	 */
	unit->set_attack(atoi(argv[2]));
	unit->set_ranged(atoi(argv[3]));
	unit->set_defense(atoi(argv[4]));
	unit->set_resist(atoi(argv[5]));
	unit->set_hits(atoi(argv[6]));
	unit->set_moves(atoi(argv[7]));
	unit->set_movetype(atoi(argv[8]));
	unit->set_troops(atoi(argv[9]));
	unit->set_total_hits(unit->get_hits()*unit->get_troops());

	return 0;
}//create_callback

//This is a basic function for determining the outcome of one unit attacking
//another (the first parameter is the attacking unit and the second is the
//defending unit).
void melee_attack(Unit* attacker, Unit* defender, Hits* hits)
{
	int i, j;
	double attack, defense;
	int attacker_troops, defender_troops;
	int attacker_attack, defender_attack;
	int attacker_defense, defender_defense;
	int attacker_total_hits, defender_total_hits;
	
	attacker_troops = attacker->get_troops();
	attacker_attack = attacker->get_attack();
	attacker_defense = attacker->get_defense();

	defender_troops = defender->get_troops();
	defender_attack = defender->get_attack();
	defender_defense = defender->get_defense();

	attacker_total_hits = attacker->get_total_hits();
	defender_total_hits = defender->get_total_hits();

	for(i = 0; i < attacker_troops && defender_total_hits > 0 && attacker_total_hits > 0; i++)
	{
		//Attacker goes first
		attack = attacker_attack * ((double)rand()/RAND_MAX);
		defense = defender_defense * ((double)rand()/RAND_MAX);
		if (attack > defense)
		{
			defender_total_hits -= 1;
			hits->attacker += 1;
		}//if

		//The defenders could all be destroyed or there are fewer defenders than attackers
		if (defender_total_hits > 0 && i < defender_troops)
		{
			//Defender counterattacks
			attack = defender_attack * ((double)rand()/RAND_MAX);
			defense = attacker_defense * ((double)rand()/RAND_MAX);
			if (attack > defense)
			{
				attacker_total_hits -= 1;
				hits->defender += 1;
			}//if
		}//if
	}//for

	attacker->set_troops(static_cast<int>(ceil(attacker_total_hits/(double)attacker->get_hits())));
	defender->set_troops(static_cast<int>(ceil(defender_total_hits/(double)defender->get_hits())));
	attacker->set_total_hits(attacker_total_hits);
	defender->set_total_hits(defender_total_hits);
}//melee_attack

void reset_troops(Unit* unit, sqlite3* sqlite)
{
	string query = "select troops from CHARACTERS where name='";
	query.append(unit->get_name().c_str());
	query.append("';");
	sqlite3_exec(sqlite, query.c_str(), troops_callback, static_cast<void*>(unit), NULL);
}//reset_troops

int troops_callback(void *new_unit, int argc, char **argv, char **azColName)
{
	Unit* unit = static_cast<Unit*>(new_unit);

	unit->set_troops(atoi(argv[0]));
	unit->set_total_hits(unit->get_hits()*unit->get_troops());

	return 0;
}//reset_callback

void reset_moves(Unit* unit, sqlite3* sqlite)
{
	string query = "select moves from CHARACTERS where name='";
	query.append(unit->get_name().c_str());
	query.append("';");
	sqlite3_exec(sqlite, query.c_str(), moves_callback, static_cast<void*>(unit), NULL);
}

int moves_callback(void *new_unit, int argc, char **argv, char **azColName)
{
	Unit* unit = static_cast<Unit*>(new_unit);

	unit->set_moves(atoi(argv[0]));

	return 0;
}//reset_callback

int change_unit(UnitPos** units, int curr_attack)
{
	int i;
	int new_attacker = curr_attack;

	new_attacker = (new_attacker + 1) % NUM_ATTACKERS;
	while(!units[new_attacker]->is_alive())
	{
		new_attacker = (new_attacker + 1) % NUM_ATTACKERS;

		if (new_attacker == curr_attack) //If we cycle through all units without finding anyone else alive we stop
			return new_attacker;
	}//while

	return new_attacker;
}//change_unit

bool check_battle(UnitPos* attacker, UnitPos* defender, State state, Hits* hits)
{
	if (attacker->get_x() == defender->get_x() && attacker->get_y() == defender->get_y())
	{
		melee_attack(attacker->get_unit(), defender->get_unit(), hits);
		if (attacker->get_unit()->get_troops() == 0)
		{
			attacker->set_alive(false);
			attacker->set_x(-1); //Take the unit "off" the battlefield
		}//if
		if (defender->get_unit()->get_troops() == 0)
		{
			defender->set_alive(false);
			defender->set_x(-1); //Take the unit "off" the battlefield
		}//if
		
		//Use the key pressed to determine where to put the attacker back after combat
		if (state == RIGHT)
			attacker->dec_x(32);
		else if (state == LEFT)
			attacker->inc_x(32);
		else if (state == UP)
			attacker->inc_y(32);
		else if (state == DOWN)
			attacker->dec_y(32);

		return true;
	}//if
	else
		return false;
}//check_battle

void init_unit(UnitPos* unit, int x, int y, int r, int g, int b, int a)
{
	unit->set_start_position(x, y);
	unit->reset_position();
	unit->set_RGBA(r, g, b, a);
}//init_unit

void unit_move(UnitPos* unit, State state)
{
	bool move = false;

	if (state == LEFT)
	{
		if (unit->get_x() - 32 >= 0 && unit->get_unit()->get_moves() > 0)
		{
			unit->dec_x(32);
			move = true;
		}//if
	}//if
	else if (state == RIGHT)
	{
		if (unit->get_x() + 32 < SCREEN_WIDTH && unit->get_unit()->get_moves() > 0)
		{
			unit->inc_x(32);
			move = true;
		}//if
	}//else if
	else if (state == UP)
	{
		if (unit->get_y() - 32 >= 0 && unit->get_unit()->get_moves() > 0)
		{
			unit->dec_y(32);
			move = true;
		}//if
	}//else if
	else if (state == DOWN)
	{
		if (unit->get_y() + 32 < PANEL_Y && unit->get_unit()->get_moves() > 0)
		{
			unit->inc_y(32);
			move = true;
		}//if
	}//else if
	
	if (move)
		unit->get_unit()->dec_moves(1);
}//unit_move

void defender_move(UnitPos* unit, UnitPos** attackers, State* state, Hits* hits)
{
	int i;
	State move_state = LEFT;

	unit_move(unit, move_state);
	hits->attacker = 0;
	hits->defender = 0;

	for (i = 0; i < NUM_ATTACKERS; i++)
		if (check_battle(unit, attackers[i], move_state, hits))
			*state = DEFEND;
}//defender_move
