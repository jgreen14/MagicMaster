#ifndef UNITPOS_H
#define UNITPOS_H
#include "unit.h"

//Class for holding a Unit and its position information
class UnitPos
{
	private:
		int start_x, start_y;
		int x, y;
		int r, g, b, a;
		Unit* unit;
		bool alive;

	public:
		UnitPos(string);
		UnitPos();

		int get_x();
		int get_y();
		int get_r();
		int get_g();
		int get_b();
		int get_a();
		Unit* get_unit();
		bool is_alive();
		void set_alive(bool);
		void set_RGBA(int, int, int, int);
		void reset_position();
		void set_position(int, int);
		void set_x(int);
		void inc_x(int);
		void dec_x(int);
		void set_y(int);
		void inc_y(int);
		void dec_y(int);
		void set_start_position(int, int);
};

#endif
