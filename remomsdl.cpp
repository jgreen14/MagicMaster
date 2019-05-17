#include <cstdlib>
#include <ctime>
#include <iostream>
#include "renderer.h"

void init_game(UnitPos** attackers, UnitPos** defenders, Hits* hits, sqlite3* sqlite)
{
	UnitPos* sword1 = new UnitPos("SWORDSMEN");
	UnitPos* halb2 = new UnitPos("HALBERDIERS");
	UnitPos* spear1 = new UnitPos("SPEARMEN"); 
	UnitPos* shaman1 = new UnitPos("SHAMAN"); 
	UnitPos* halb1 = new UnitPos("HALBERDIERS"); 
	
	//Attackers
	create_unit(sword1->get_unit(), sqlite);
	init_unit(sword1, 32, 64, 255, 0, 0, 255);
	attackers[0] = sword1;

	create_unit(halb2->get_unit(), sqlite);
	init_unit(halb2, 32, 128, 255, 0, 0, 255);
	attackers[1] = halb2;

	//Defenders
	create_unit(spear1->get_unit(), sqlite);
	init_unit(spear1, 352, 256, 0, 0, 255, 255);
	defenders[0] = spear1;

	create_unit(shaman1->get_unit(), sqlite);
	init_unit(shaman1, 320, 192, 0, 0, 255, 255);
	defenders[1] = shaman1;

	create_unit(halb1->get_unit(), sqlite);
	init_unit(halb1, 512, 288, 0, 0, 255, 255);
	defenders[2] = halb1;

	hits->attacker = 0;
	hits->defender = 0;
}//init_game

int main (char* ARGV[], int ARGC)
{
	/* Declarations */
	int i;
	sqlite3* sqlite;
	UnitPos* attackers[NUM_ATTACKERS];
	UnitPos* defenders[NUM_DEFENDERS];
	Hits hits;
	int curr_attack = 0;
	int curr_defend = 0;
	bool running = true;
	State state = START;

	/* Initialization */
	srand((unsigned)time(0));

	sqlite3_open("chardata.db", &sqlite);

	init_game(attackers, defenders, &hits, sqlite);
	
	/*************************
	* SDL code goes here
	*************************/
	SDL_Init(SDL_INIT_VIDEO);
	atexit(SDL_Quit);
	SDL_Surface* screen = SDL_SetVideoMode(640, 480, 0, SDL_DOUBLEBUF);
	SDL_Rect dest = {0, 355, 0, 0};
	SDL_Event event;
	SDL_keysym keysym;
	TTF_Init();
	atexit(TTF_Quit);
	TTF_Font* font = TTF_OpenFont("Times_New_Roman.ttf", 12);

	while(running)
	{
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					keysym = event.key.keysym;
					if (keysym.sym == SDLK_LEFT)
						state = LEFT;
					else if (keysym.sym == SDLK_RIGHT)
						state = RIGHT;
					else if (keysym.sym == SDLK_UP)
						state = UP;
					else if (keysym.sym == SDLK_DOWN)
						state = DOWN;
					else if (keysym.sym == SDLK_s)
						state = SPAWN;
					else if (keysym.sym == SDLK_SPACE)
						state = NEXT;
					else if (keysym.sym == SDLK_TAB)
						state = CHANGE;
					else if (keysym.sym == SDLK_q)
						running = false;
					break;
			}//switch
		}//while

		switch(state)
		{
			case LEFT:
			case RIGHT:
			case UP:
			case DOWN:
				unit_move(attackers[curr_attack], state);
				hits.attacker = 0;
				hits.defender = 0;
				for (i = 0; i < NUM_DEFENDERS; i++)
					if (check_battle(attackers[curr_attack], defenders[i], state, &hits))
					{
						curr_defend = i;
						state = ATTACK;

						if (!attackers[curr_attack]->is_alive())
							change_unit(attackers, curr_attack);
					}//if
				update_display(attackers, defenders, curr_attack, curr_defend, state, &hits, font, screen);
				state = NONE;
				break;

			case SPAWN:
				for (i = 0; i < NUM_DEFENDERS; i++)
				{
					if (defenders[i]->is_alive() == false)
					{
						defenders[i]->set_alive(true);
						defenders[i]->reset_position();
						reset_troops(defenders[i]->get_unit(), sqlite);
					}//if
				}//for
				update_display(attackers, defenders, curr_attack, curr_defend, state, &hits, font, screen);
				state = NONE;
				break;

			//Do enemy AI here
			case NEXT:
				for (i = 0; i < NUM_ATTACKERS; i++)
					reset_moves(attackers[i]->get_unit(), sqlite);
				for (curr_defend = 0; curr_defend < NUM_DEFENDERS; curr_defend++)
				{
					if (defenders[curr_defend]->is_alive())
					{
						defender_move(defenders[curr_defend], attackers, &state, &hits);	
						if (state == DEFEND)
						{
							update_display(attackers, defenders, curr_attack, curr_defend, state, &hits, font, screen);
							SDL_Delay(1000);
						}
						else
							SDL_Delay(100);
						state = NONE;
						update_display(attackers, defenders, curr_attack, curr_defend, state, &hits, font, screen);
					}//if
					reset_moves(defenders[curr_defend]->get_unit(), sqlite);
				}//for
				break;

			case CHANGE:
				curr_attack = change_unit(attackers, curr_attack);
				update_display(attackers, defenders, curr_attack, curr_defend, state, &hits, font, screen);
				state = NONE;
				break;

			case START:
				update_display(attackers, defenders, curr_attack, curr_defend, state, &hits, font, screen);
				break;

		}//switch
	}//while

	SDL_FreeSurface(screen);
	TTF_CloseFont(font);
	/*************************
	//End SDL code
	*************************/
	
	sqlite3_close(sqlite);

	exit(0);
}//main
