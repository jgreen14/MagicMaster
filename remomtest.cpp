#include <cstdlib>
#include <ctime>
#include <iostream>
#include "engine.h"

int main (char* ARGV[], int ARGC)
{
	sqlite3* sqlite;
	Unit* sword1 = new Unit("SWORDSMEN");
	Unit* spear1 = new Unit("SPEARMEN");
	Unit* shaman1 = new Unit("SHAMAN");
	Unit* halb1 = new Unit("HALBERDIERS");

	srand((unsigned)time(0));
	
	sqlite3_open("chardata.db", &sqlite);
	create_unit(sword1, sqlite);
	create_unit(spear1, sqlite);
	create_unit(shaman1, sqlite);
	create_unit(halb1, sqlite);

	cout << sword1->get_name() << endl;
	cout << sword1->get_troops() << endl;
	cout << spear1->get_name() << endl;
	cout << spear1->get_troops() << endl;

	cout << endl << sword1->get_name() << " attack " << spear1->get_name() << "!" << endl;
	melee_attack(sword1, spear1);

	cout << sword1->get_name() << endl;
	cout << sword1->get_troops() << endl;
	cout << spear1->get_name() << endl;
	cout << spear1->get_troops() << endl;

	cout << endl << sword1->get_name() << " attack " << shaman1->get_name() << "!" << endl;
	melee_attack(sword1, shaman1);

	cout << sword1->get_name() << endl;
	cout << sword1->get_troops() << endl;
	cout << shaman1->get_name() << endl;
	cout << shaman1->get_troops() << endl;

	cout << endl << sword1->get_name() << " attack " << halb1->get_name() << "!" << endl;
	melee_attack(sword1, halb1);
	
	cout << sword1->get_name() << endl;
	cout << sword1->get_troops() << " " << sword1->get_total_hits() << endl;
	cout << halb1->get_name() << endl;
	cout << halb1->get_troops() << " " << halb1->get_total_hits() << endl;

	sqlite3_close(sqlite);

	return 0;
}//main
