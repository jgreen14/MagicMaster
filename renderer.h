#include <sstream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "engine.h"

void draw_unit(UnitPos*, SDL_Surface*);

void draw_troops(UnitPos*, SDL_Surface*);

void draw_cursor(UnitPos*, SDL_Surface*);

void draw_panel(SDL_Surface*);

int text_width(TTF_Font*, string);

int text_height(TTF_Font*, string);

int print_unit_info(Unit*, TTF_Font*, SDL_Surface*);

void print_battle_summary(Unit*, Unit*, Hits*, TTF_Font*, SDL_Surface*);

void update_display(UnitPos**, UnitPos**, int, int, State, Hits*, TTF_Font*, SDL_Surface*);
