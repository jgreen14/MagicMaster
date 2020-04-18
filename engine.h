#ifndef ENGINE_H
#define ENGINE_H

#include <cstdlib>
#include <cmath>
#include "unitpos.h"
#include "sqlite3.h"

#define NUM_ATTACKERS 2
#define NUM_DEFENDERS 3
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define PANEL_Y 352

using namespace std;

typedef struct 
{
	int attacker;
	int defender;
} Hits;

typedef enum
{
	NONE,
	START,
	NEXT,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	ATTACK,
	DEFEND,
	CHANGE,
	SPAWN,
	NUM_STATES
} State;

int create_callback(void*, int, char**, char**);

void create_unit (Unit*, sqlite3*);

void melee_attack(Unit*, Unit*, Hits* hits);

void reset_troops(Unit*, sqlite3*);

int troops_callback(void*, int, char**, char**);

void reset_moves(Unit*, sqlite3*);

int moves_callback(void*, int, char**, char**);

int change_unit(UnitPos**, int);

bool check_battle(UnitPos*, UnitPos*, State, Hits*);

void init_unit(UnitPos*, int, int, int, int, int, int);

void unit_move(UnitPos*, State);

void defender_move(UnitPos*, UnitPos**, State*, Hits*);

#endif
