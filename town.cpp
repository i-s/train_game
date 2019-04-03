#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "train.h"
#include "text.h"
#include <stdlib.h>
//#include "main.h"

//Объявление глобальных ресурсов:
//глобальные люди
int g_humans;
//глобальные материалы
int g_resourses;
//глобальная еда
int g_food;

//Объявление глобальных SDL_Rect для ресурсов:
SDL_Rect g_recthumans = { 66,2,197,44 };
SDL_Rect g_rectfood = { 66,47,197,51 };
SDL_Rect g_rectresourses = { 66,99,197,49 };

//Глобальный шрифт программы
//TTF_Font* my_font = TTF_OpenFont("resourses/Text.ttf", 100);

void Update(SDL_Window* window,SDL_Renderer* renderer, char* texts[]) {
	draw_text(window, renderer, texts[0], g_recthumans);
	draw_text(window, renderer, texts[1], g_rectfood);
	draw_text(window, renderer, texts[2], g_rectresourses);

	SDL_RenderPresent(renderer);
}

//Экран "город".
//Возврат 0 -> 
int town_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h) {
	
	//Загружаем текстуры
	//Фона
	SDL_Surface* background_surf = SDL_LoadBMP("resourses/textures/background_town_normal.bmp");
	SDL_Texture* background_texture = SDL_CreateTextureFromSurface(renderer, background_surf);
	SDL_FreeSurface(background_surf);

	//Рисуем задний план
	SDL_Rect bground_rectangle = { 0,0,winsize_w,winsize_h };
	Background background = { background_texture, bground_rectangle };
	draw_background(renderer, background);

	//Создаём ивент и переменную для отслеживания закрытия окна
	SDL_Event event;
	bool quit = false;

	//Строки для хранения числовых значений людей, еды и ресурсов.
	char* text_humans = new char[10];
	char* text_food = new char[10];
	char* text_resourses = new char[10];

	//Массив строк для удобной передачи в ф-ию
	char* texts[3] = { text_humans, text_food, text_resourses };
	
	while (!quit) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			quit = true;

		_itoa_s(g_humans, text_humans, 10, 10);
		_itoa_s(g_food, text_food, 10, 10);
		_itoa_s(g_resourses, text_resourses, 10, 10);

		Update(window,renderer, texts);

		SDL_Delay(10);
	}

	printf_s("hum = %d, res = %d, food = %d\n",g_humans,g_resourses,g_food);

	SDL_DestroyTexture(background_texture);
	return 0;
}