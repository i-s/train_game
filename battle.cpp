#include <SDL.h>
//#include "main.h"
#include "town.h"

extern int DELTA;
extern int LASTTICKTIME;

/*
struct Background {
	SDL_Texture* texture;
	SDL_Rect rectangle;
};
*/

/*
void draw_background(SDL_Renderer* renderer, Background background) {
	SDL_RenderCopy(renderer, background.texture, NULL, &background.rectangle);
}
*/

void Update(SDL_Window* window, SDL_Renderer* renderer, Background background) {
	draw_background(renderer, background);

	SDL_RenderPresent(renderer);
}

//Экран "сражение".
//Возврат 0 -> окно закрыто
int battle_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h) {
	//Загружаем текстуры
	SDL_Surface* background_surf = SDL_LoadBMP("resourses/textures/background_attack.bmp");
	SDL_Texture* background_texture = SDL_CreateTextureFromSurface(renderer, background_surf);
	SDL_FreeSurface(background_surf);

	//Заполняем структуру заднего фона
	SDL_Rect background_rect = {0,0,winsize_w,winsize_h};
	Background background = {background_texture,background_rect};

	SDL_Event event;

	bool quit = false; //Выход из программы

	while (!quit) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			quit = true;

		//TODO: Доработать боёвку

		Update(window, renderer, background);

		SDL_Delay(10);
	}

	//Считаем время
	int tick_time = SDL_GetTicks();
	DELTA = tick_time - LASTTICKTIME;
	LASTTICKTIME = tick_time;

	SDL_DestroyTexture(background_texture);

	return 0;
}