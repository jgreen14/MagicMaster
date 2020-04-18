#include "renderer.h"

void draw_unit(UnitPos* unit, SDL_Surface* screen)
{
	boxRGBA(screen, unit->get_x(), unit->get_y(), unit->get_x()+32, unit->get_y()+32, unit->get_r(), unit->get_g(), unit->get_b(), unit->get_a());
	rectangleRGBA(screen, unit->get_x(), unit->get_y(), unit->get_x()+32, unit->get_y()+32, 0, 0, 0, 255);
	draw_troops(unit, screen);
}//draw_unit

void draw_troops(UnitPos* unit, SDL_Surface* screen)
{
	int i;
	int troops = unit->get_unit()->get_troops();
	int x = 6;
	int y = 5;

	for (i = 0; i < troops; i++)
	{
		if (i > 0 && (i % 2) == 1)
			x += 18;
		else if (i > 0)
		{
			x = 6;
			y += 9;

		}//else if

		boxRGBA(screen, unit->get_x()+x, unit->get_y()+y, (unit->get_x()+x)+4, (unit->get_y()+y)+4, 0, 0, 0, 255);
	}//for
}//draw_troops

void draw_cursor(UnitPos* unit, SDL_Surface* screen)
{
	rectangleRGBA(screen, unit->get_x(), unit->get_y(), unit->get_x()+32, unit->get_y()+32, 128, 128, 0, 255);
	rectangleRGBA(screen, unit->get_x()+1, unit->get_y()+1, unit->get_x()+31, unit->get_y()+31, 255, 255, 0, 255);
}//draw_cursor

void draw_panel(SDL_Surface* screen)
{
	boxRGBA(screen, 0, PANEL_Y, SCREEN_WIDTH, SCREEN_HEIGHT, 128, 128, 128, 255);
	rectangleRGBA(screen, 0, PANEL_Y, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 255);
	rectangleRGBA(screen, 1, PANEL_Y+1, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0, 0, 0, 255);
	lineRGBA(screen, SCREEN_WIDTH/2, PANEL_Y, SCREEN_WIDTH/2, SCREEN_HEIGHT, 0, 0, 0, 255); 
}//draw_panel

int text_width(TTF_Font* font, string text)
{
	int width;
	TTF_SizeText(font, text.c_str(), &width, NULL);
	return width;
}//text_width

int text_height(TTF_Font* font, string text)
{
	int height;
	TTF_SizeText(font, text.c_str(), NULL, &height);
	return height;
}//text_width

int print_unit_info(Unit* unit, TTF_Font* font, SDL_Surface* screen)
{
	const int num_headings = 6;
	int i;
	int values[num_headings] = { unit->get_attack(), unit->get_defense(), unit->get_resist(), unit->get_hits(), unit->get_troops(), unit->get_moves() };
	string headings[num_headings] = {"Attack: ", "Defense: ", "Resist: ", "Hits: ", "Troops: ", "Moves: " } ;
	string current_text;
	stringstream out;
	SDL_Rect dest = {5, 360, 0, 0};
	SDL_Color font_color = {0, 0, 0, 255};
	SDL_Surface* text;

	//Print unit type/name
	current_text = "Type: ";
	current_text.append(unit->get_name().c_str());
	text = TTF_RenderText_Blended(font, current_text.c_str(), font_color);
	SDL_BlitSurface(text, NULL, screen, &dest);

	//Print unit stats
	for (i = 0; i < num_headings; i++)
	{
		dest.y += text_height(font, current_text);
		current_text = headings[i];
		out << values[i];
		out.seekp(0);
		current_text.append(out.str());
		text = TTF_RenderText_Blended(font, current_text.c_str(), font_color);
		SDL_BlitSurface(text, NULL, screen, &dest);
	}//for

}//print_unit_info

void print_battle_summary(Unit* attacker, Unit* defender, Hits* hits, TTF_Font* font, SDL_Surface* screen)
{
	string str;
	stringstream out;
	SDL_Rect dest = {SCREEN_WIDTH/2 + 5, 360, 0, 0};
	SDL_Color font_color = {0, 0, 0, 255};
	SDL_Surface* text;

	out << attacker->get_name().c_str() << " attacks " << defender->get_name().c_str() << " for " << hits->attacker << " hits!";
	str = out.str();
	text = TTF_RenderText_Blended(font, str.c_str(), font_color);
	SDL_BlitSurface(text, NULL, screen, &dest);
	dest.y += text_height(font, out.str());

	out.seekp(0);	
	out << defender->get_name().c_str() << " counterattacks " << attacker->get_name().c_str() << " for " << hits->defender << " hits!";
	str = out.str();
	text = TTF_RenderText_Blended(font, str.c_str(), font_color);
	SDL_BlitSurface(text, NULL, screen, &dest);
}

//Do all the updating for the display
//The number of parameters is ugly but they are necessary for the functions called.
//(Look into putting some of these things into existing/new classes)
void update_display(UnitPos** attackers, UnitPos** defenders, int curr_attack, int curr_defend, State state, Hits* hits, TTF_Font* font, SDL_Surface* screen)
{
	int i;

	//Draw the background colour first
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 195, 75));

	draw_panel(screen);
	print_unit_info(attackers[curr_attack]->get_unit(), font, screen);

	if (state == ATTACK)
		print_battle_summary(attackers[curr_attack]->get_unit(), defenders[curr_defend]->get_unit(), hits, font, screen);

	if (state == DEFEND)
		print_battle_summary(defenders[curr_defend]->get_unit(), attackers[curr_attack]->get_unit(), hits, font, screen);

	//Draw the units and troops
	for (i = 0; i < NUM_ATTACKERS; i++)
		if (attackers[i]->is_alive())
		{
			draw_unit(attackers[i], screen);
			if (i == curr_attack)
				//Draw cursor on currently active attacker
				draw_cursor(attackers[i], screen);
		}//if
	for (i = 0; i < NUM_DEFENDERS; i++)
		if (defenders[i]->is_alive())
			draw_unit(defenders[i], screen);

	SDL_Flip(screen);
}//update_display
