#ifndef UNIT_H
#define UNIT_H
#include <string>

using namespace std;

class Unit 
{
	private:
		string name;
		int attack;
		int ranged;
		int defense;
		int resist;
		int hits;
		int moves;
		int movetype;
		int troops;
		int total_hits;

	public:
		Unit();
		Unit(string, int, int, int, int, int, int, int, int);
		Unit(string);
		//~Unit();

		void set_name(string name) { this->name = name; }
		void set_attack(int attack) { this->attack = attack; }
		void set_ranged(int ranged) { this->ranged = ranged; }
		void set_defense(int defense) { this->defense = defense; }
		void set_resist(int resist) {this->resist = resist; }
		void set_hits(int hits) { this->hits = hits; }
		void set_moves(int moves) { this->moves = moves; }
		void dec_moves(int moves) { this->moves -= moves; }
		void set_movetype(int movetype) { this->movetype = movetype; }
		void set_troops(int troops) { this->troops = troops; }
		void set_total_hits(int total_hits) { this->total_hits = total_hits; }

		string get_name() { return name; }
		int get_attack() { return attack; }
		int get_ranged() { return ranged; }
		int get_defense() { return defense; }
		int get_resist() { return resist; }
		int get_hits() { return hits; }
		int get_moves() { return moves; }
		int get_movetype() { return movetype; }
		int get_troops() { return troops; }
		int get_total_hits() { return total_hits; }

		void reset_total_hits();
};//Class Unit

#endif
