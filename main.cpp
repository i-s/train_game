#include <SDL.h>
#include "menu.h"
#include "train.h"
#include "town.h"
#include <time.h>
#undef main

//Загружаем глобальные переменные из town.cpp
extern float g_humans,g_resourses,g_food;
extern int lever1_pulled, lever2_pulled;
extern int difficulty;
extern float one_second;
extern int g_rooms[2][3][2];



/*To-do:
V Нажимаемые кнопки в меню
1) Движение поезда
2) Звук в игре
3) Экран города (переход из города в меню поезда)
*/

int main() {
	//Создаём окно
	int winsize_w = 800, winsize_h = 600;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* WINDOW = SDL_CreateWindow("Main window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		winsize_w, winsize_h, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//Тестовые начальные значения ресурсов:
	g_humans = 50;
	g_resourses = 50;
	g_food = 300;

	//Делаем все комнаты пустыми
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			g_rooms[i][j][0] = 0;
			g_rooms[i][j][1] = 0;
		}
	}

	//Пусть первая комната сразу будет фермой
	g_rooms[0][0][0] = 1;
	g_rooms[0][0][1] = 1;

	//menu(WINDOW, renderer, winsize_w, winsize_h)
	//train_game(WINDOW, renderer, winsize_w, winsize_h)
	//town_game(WINDOW, renderer, winsize_w, winsize_h)

	//Запускаем меню
	int menu_flag;
	int train_flag;
	int town_flag = -1;
	lever1_pulled = 0;
	lever2_pulled = 0;
	difficulty = 1; //Сложность игры
	one_second = 1;
	extern int GAMESTARTTIME;
	extern float TIMEUNTILTRAIN;
	TIMEUNTILTRAIN = 10;

	//Запускаем игру
	menu_flag = menu(WINDOW, renderer, winsize_w, winsize_h);
	if (menu_flag == 1) {
		GAMESTARTTIME = int(time(NULL)); // время старта игры записываем
		while (town_flag != 0) {
			town_flag = town_game(WINDOW, renderer, winsize_w, winsize_h);
			if (town_flag == 1) {
				train_flag = train_game(WINDOW, renderer, winsize_w, winsize_h);
				if (train_flag == 0) {
					continue;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}

	//if (menu_flag == 1) {
	//	//Запускаем поезд
	//	 train_flag = train_game(WINDOW, renderer, winsize_w, winsize_h); 
	//}

	//if (train_flag == 0) {
	//	town_flag = town_game(WINDOW, renderer, winsize_w, winsize_h);
	//}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(WINDOW);
	return 0;
}