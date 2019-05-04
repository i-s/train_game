#pragma once
#include <SDL.h>
#include "menu.h"
#include "town.h"
#include "text.h"
#include "main.h"
#include "train.h"
#include <stdlib.h>

//Загружаем глобальные переменные из town.cpp
extern float g_humans, g_resourses, g_food;
extern SDL_Rect g_recthumans, g_rectfood, g_rectresourses;
extern int DELTA;
extern int LASTTICKTIME;

void Update(SDL_Window* window, SDL_Renderer* renderer, Background background, char* texts[], Town_block button_block) {
	draw_background(renderer, background);

	//Рисуем ресурсы
	draw_text(window, renderer, texts[0], g_recthumans);
	draw_text(window, renderer, texts[1], g_rectfood);
	draw_text(window, renderer, texts[2], g_rectresourses);

	draw_town_block(renderer, button_block);

	SDL_RenderPresent(renderer);
}

//Экран "сражение".
//Возврат 0 -> окно закрыто
int battle_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h) {
	//Загружаем текстуры
	//Текстуры фона
	SDL_Surface* background_surf = SDL_LoadBMP("resourses/textures/background_attack.bmp");
	SDL_Texture* background_texture = SDL_CreateTextureFromSurface(renderer, background_surf);
	SDL_FreeSurface(background_surf);
	//Текстуры блокиратора кнопки
	SDL_Surface* block_surf = SDL_LoadBMP("resourses/textures/town_block.bmp");
	SDL_SetColorKey(block_surf, 1, SDL_MapRGB(block_surf->format, 0, 255, 0));
	SDL_Texture* block_texture = SDL_CreateTextureFromSurface(renderer, block_surf);
	SDL_FreeSurface(block_surf);

	//Заполняем структуру заднего фона
	SDL_Rect background_rect = {0,0,winsize_w,winsize_h};
	Background background = {background_texture,background_rect};
	
	//Строки для хранения числовых значений людей, еды и ресурсов.
	char* text_humans = new char[10];
	char* text_food = new char[10];
	char* text_resourses = new char[10];
	//Массив строк для удобной передачи в ф-ию
	char* texts[6] = { text_humans, text_food, text_resourses};

	//Описание кнопок
	SDL_Rect return_button = {}; //Кнопка для возвращения к экрану "город"

	//Массив кнопок
	SDL_Rect buttons[1] = { return_button };

	//Блокиратор кнопки "назад"
	Town_block button_block;
	button_block.rectangle = return_button;
	button_block.texture = block_texture;
	button_block.shown = false;

	SDL_Event event;

	bool quit = false; //Выход из программы
	bool goback_to_train = false; //Флаг возврата к экрану "город"
	bool raid_is_over = false; //Флаг окончания рейда
	//TODO: Написать условия окончания рейда

	while (!quit) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			QUIT = true;
			quit = true;
		}

		//TODO: Доработать боёвку

		//Каждый раз в цикле говорим, что мышь не наведена ни на одну из кнопок
		int button_flag = -1;
		if (LKMPressed(event))
			for (int i = 0; i <= 0; i++) {
				button_flag = CheckIfMouseOnButton(event, i, buttons);
				if (button_flag == 0 && raid_is_over) { //Если нажали на кнопку "назад", но рейд ещё не окончен
					button_block.shown = true; //...рисуем блок на кнопке.
				}
				else if (button_flag == 0 && raid_is_over) {//Если нажали на кнопку "назад", и рейд окончен
					goback_to_train = true; //...возвращаемся к "городу"
				}
			}

		//Подготавливаем текст для вывода
		_itoa_s(int(g_humans), text_humans, 10, 10);
		_itoa_s(int(g_food), text_food, 10, 10);
		_itoa_s(int(g_resourses), text_resourses, 10, 10);


		Update(window, renderer, background, texts, button_block);

		SDL_Delay(10);
	}

	//Считаем время
	int tick_time = SDL_GetTicks();
	DELTA = tick_time - LASTTICKTIME;
	LASTTICKTIME = tick_time;

	SDL_DestroyTexture(background_texture);
	SDL_DestroyTexture(block_texture);
	return 0;
}