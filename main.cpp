#pragma once
#include <SDL.h>
#include "menu.h"
#include "train.h"
#include "town.h"
#include <time.h>
#include "battle.h"
#include "rooms.h"
#include "notifications.h"
#include "save.h"
#include "intro.h"
#include <stdio.h>
#include "Settings.h"

#define START_FOOD 1500
#define START_HUMANS 1150
#define START_RESOURSES 1200
//размеры , куда крепились текстурки
#define WINSIZE_H = 600
#define WINSIZE_W = 800
#undef main

//Загружаем глобальные переменные из town.cpp
extern float g_humans,g_resourses,g_food;
extern int lever1_pulled, lever2_pulled;
extern int difficulty;
extern float one_second;
extern float g_time_before_raid;
extern int g_rooms[2][3][2];
extern float g_income_food, g_income_res, g_income_hum;
extern int GAME_OVER;
extern bool g_have_open_town_before;
extern bool g_was_game_loaded;

extern int g_music_volume;
extern int g_sound_volume;

extern bool QUIT;

//Глобальные размеры окна
int winsize_w = 800, winsize_h = 600;
int user_winsize_w = 800, user_winsize_h = 600;
bool fullscreen = false;//флаг полного экрана


//Проверяет, существует ли файл в папке с игрой.
//Возвразает false, если нет, и true, если да.
bool check_file(char* filename) {
	FILE *read_file;
	fopen_s(&read_file, filename, "r");
	if (read_file != NULL) { //Если файл загружен
		fclose(read_file);
		return true; //Возвращаем true
	}
	return false;  //Возврващаем flase
}


int get_settings(char* filename)
{
	if (!check_file(filename))
	{
		printf("HE MOGU, Иду стандартом\n");
		set_settings_in_file();
	}	
	get_settings_of_file();
	return 1;
}

//menu(WINDOW, renderer, winsize_w, winsize_h)
//train_game(WINDOW, renderer, winsize_w, winsize_h)
//town_game(WINDOW, renderer, winsize_w, winsize_h)

//Проверяет значение переменной QUIT, и если она истинна, то выходит из программы.
//bool CheckForQUIT() {
//}

int main() {
	//ищем фаил настройками
	get_settings((char *)"setting.cfg");

	//Создаём окно
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* WINDOW;
	if (fullscreen)
		WINDOW = SDL_CreateWindow("Main window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			winsize_w, winsize_h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	else 
		WINDOW = SDL_CreateWindow("Main window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			winsize_w, winsize_h, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	

	//SDL_RenderSetScale(renderer,1.5,1.5);
	SDL_RenderSetLogicalSize(renderer, winsize_w, winsize_h);
	
	SDL_SetWindowSize(WINDOW, user_winsize_w, user_winsize_h);
	SDL_SetWindowPosition(WINDOW, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	//Тестовые начальные значения ресурсов:
	
	
	int menu_flag = 1;
	extern int GAMESTARTTIME;
	extern float TIMEUNTILTRAIN;

	while (menu_flag != 0)
	{
		g_humans = START_HUMANS;
		g_resourses = START_RESOURSES;
		g_food = START_FOOD;
		g_income_food = 0;
		g_income_res = 0;
		g_income_hum = 0;
		int train_flag = 1;
		int town_flag = 1;
		int battle_flag = 1;

		//Делаем все комнаты пустыми
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 3; j++) {
				g_rooms[i][j][0] = 0;
				g_rooms[i][j][1] = 0;
			}
		}
			   
		//Запускаем меню
		lever1_pulled = 0;
		lever2_pulled = 0;
		difficulty = 1; //Сложность игры
		one_second = 1;
		TIMEUNTILTRAIN = 30; //30 секунд до первого поезда
		g_time_before_raid = 60; //60 секунд до первого рейда
		GAME_OVER = 0;
		int quit = 0;
		initialize_rooms(); //Инициализируем массивы для создания комнат
		//TODO: доделать перезапуск из меню
		menu_flag = menu(WINDOW, renderer, winsize_w, winsize_h);
		if (menu_flag == 1) {
			if (!g_was_game_loaded) { //Если игра была продолжена, то НЕ нужно воспроизводить интро.
				Intro(WINDOW, renderer);
			}
			GAMESTARTTIME = int(time(NULL)); // время старта игры записываем
			while (town_flag != 0 && !QUIT) { //Если не вышли из программы
				if (town_flag == -1 || train_flag == -1) { //Если проиграли
					break;//TODO: проигрыш(вывести что-нубидь
				}
				town_flag = town_game(WINDOW, renderer, winsize_w, winsize_h); //Запускаем город
				if (town_flag == 1) { //Если перешли к поезду
					train_flag = train_game(WINDOW, renderer, winsize_w, winsize_h); //Запускаем поезд
					if (train_flag == 0) { //Если вышли из поезда
						continue; //Переходим к городу
					}
					else if (train_flag == 1) { //Если на убежище напали
						//Переходим к битве
						battle_flag = battle_game(WINDOW, renderer, winsize_w, winsize_h);
						if (battle_flag == 0) { //Если вышли из битвы
							continue; //Переходим к городу
						}
					}
					else { //Иначе...
						break; //Выходим из игры
					}
				}
				else if (town_flag == 2) { //Если перешли к битве
					battle_flag = battle_game(WINDOW, renderer, winsize_w, winsize_h);
					if (battle_flag == 0) { //Если вышли из битвы
						continue; //Переходим к городу
					}
				}
				else { //Если вышли из города
					break; //Выходим в меню
				}
			}
		}
		else if (menu_flag == 2)//если нажаты настрокйки
			launch_settings(WINDOW, renderer, winsize_w, winsize_h);
		if (QUIT) //Если нажали кнопку "закрыть" (крестик), выходим из программы.
			break;
	}
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(WINDOW);
	//if(g_have_open_town_before)save_game(); //Если игра открывалась, сохраняем её.
	Exit_Music();
	return 0;
}