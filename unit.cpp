#include "unit.h"

Unit::Unit ()
{
	name = "";
	attack = 0;
	ranged = 0;
	defense = 0;
	resist = 0;
	hits = 0;
	moves = 0;
	movetype = 0;
	troops = 0;
	total_hits = 0;
}

Unit::Unit (string name, int attack, int ranged, int defense, int resist, int hits, int moves, int movetype, int troops)
{
	this->name = name;
	this->attack = attack;
	this->ranged = ranged;
	this->defense = defense;
	this->resist = resist;
	this->hits = hits;
	this->moves = moves;
	this->movetype = movetype;
	this->troops = troops;
	total_hits = this->troops * this->hits;
}

Unit::Unit (string name)
{
	this->name = name;
}

/*Unit::~Unit()
{
	delete name;
}*/

void Unit::reset_total_hits()
{
	total_hits = troops * hits;
}
