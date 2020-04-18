#include "unitpos.h"

UnitPos::UnitPos()
{
	unit = new Unit();
	start_x = 0;
	start_y = 0;
	set_RGBA(0, 0, 0, 255);
	set_position(0, 0);
	alive = true;
}

UnitPos::UnitPos(string name)
{
	unit = new Unit(name.c_str());
	start_x = 0;
	start_y = 0;
	set_RGBA(0, 0, 0, 255);
	set_position(0, 0);
	alive = true;
}

int UnitPos::get_x()
{
	return x;
}

int UnitPos::get_y()
{
	return y;
}

int UnitPos::get_r()
{
	return r;
}

int UnitPos::get_g()
{
	return g;
}

int UnitPos::get_b()
{
	return b;
}

int UnitPos::get_a()
{
	return a;
}

Unit* UnitPos::get_unit()
{
	return unit;
}

bool UnitPos::is_alive()
{
	return alive;
}

void UnitPos::set_RGBA(int r, int g, int b, int a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

void UnitPos::set_position(int x, int y)
{
	this->x = x;
	this->y = y;
}

void UnitPos::reset_position()
{
	x = start_x;
	y = start_y;
}

void UnitPos::set_start_position(int x, int y)
{
	start_x = x;
	start_y = y;
}

void UnitPos::set_alive(bool alive)
{
	this->alive = alive;
}

void UnitPos::set_x(int x)
{
	this->x = x;
}

void UnitPos::inc_x(int inc)
{
	x += inc;
}

void UnitPos::dec_x(int dec)
{
	x -= dec;
}

void UnitPos::set_y(int y)
{
	this->y = y;
}

void UnitPos::inc_y(int inc)
{
	y += inc;
}

void UnitPos::dec_y(int dec)
{
	y -= dec;
}
