﻿#pragma once
#include <stdio.h>
#include <SDL.h>
#include "menu.h"
#include <time.h>
#include "town.h"
#include "text.h"
#include "main.h"
#include "train.h"
#include <stdlib.h>
#define NUMBER_OF_ENEMIES 3
#define NUMBER_OF_TYPES_OF_ENEMIES 1

//Ситруктура "Враг" SDL_Texture* texture;SDL_Rect rectangle;
//int type; //Тип врага: 0 - обычный зомби 
//bool active = false; Активен ли враг (нужно ли его отрисовывать, приближать?)
//int spawn_point;
//int hp; здоровье
//Потери ресурсов в порядке: еда, ресурсы, люди
//int loss_food;
//int loss_resourses;
//int loss_humans;
struct Enemy {
	SDL_Texture* texture;
	SDL_Rect rectangle;
	int type; //Тип врага: 0 - обычный зомби
	bool active = false; //Активен ли враг (нужно ли его отрисовывать, приближать?)
	int spawn_point;
	int hp; //Жизни врага
	//Потери ресурсов в порядке: еда, ресурсы, люди
	int loss_food;
	int loss_resourses;
	int loss_humans;
};

struct SpawnPoint {
	SDL_Point point;
	bool open = true; //Свободно ли место для спавна
};

//Загружаем глобальные переменные из town.cpp
extern float g_humans, g_resourses, g_food;
extern SDL_Rect g_recthumans, g_rectfood, g_rectresourses;
extern int DELTA , difficulty;
extern int LASTTICKTIME;
extern int g_rooms[2][3][2]; //массив комнат из города


//Отрисовывает врага
void draw_enemy(SDL_Renderer* renderer, Enemy enemy) {
	if (enemy.active) { SDL_RenderCopy(renderer, enemy.texture, NULL, &enemy.rectangle); }
}

void Update(SDL_Window* window, SDL_Renderer* renderer, Background background, char* texts[], Town_block button_block,
	Enemy enemies[]) {
	draw_background(renderer, background);

	//Рисуем ресурсы
	draw_text(window, renderer, texts[0], g_recthumans);
	draw_text(window, renderer, texts[1], g_rectfood);
	draw_text(window, renderer, texts[2], g_rectresourses);

	draw_town_block(renderer, button_block);

	for (int i = 0; i < NUMBER_OF_ENEMIES; i++)
		draw_enemy(renderer, enemies[i]);

	SDL_RenderPresent(renderer);
}

//Проверяет, не ушли ли значения ресурсов в отрицательные значения, и если ушли - вернуть их к 0.
//Если люди ушли в минус, закончить игру.
void CheckForZeroResourses() {
	if (g_food < 0)
		g_food = 0;
	if (g_resourses < 0)
		g_resourses = 0;
	if (g_humans < 0)
		g_humans = 0;
	if (g_humans == 0)
		GameOver();
}

//Обновляет позиции врагов
void Update_EnemiesPosition(Enemy enemies[], SDL_Rect battle_rect, SpawnPoint spawn_places[], int &enemies_count_in_wave) {
	for (int i = 0; i < NUMBER_OF_ENEMIES; i++)
		if (enemies[i].active && !QUIT) // Если проверяемый враг активен и игра ещё не окончилась
		{
			//Двигаем зомби
			//TODO: Зомби должны бежать медленнее
			enemies[i].rectangle.x -= 1;
			enemies[i].rectangle.w += 1;
			enemies[i].rectangle.y += 1;
			enemies[i].rectangle.h += 1;
			//Проверка на выходы врага за границы допустимой области
			if (enemies[i].rectangle.h <= battle_rect.h) { // Если размеры зомби не превышают высоту боевого окна (т.е. зомби ещё "не дошёл" до убежища)
				if (enemies[i].rectangle.y < battle_rect.y)
					enemies[i].rectangle.y = battle_rect.y;
				if ((enemies[i].rectangle.y + enemies[i].rectangle.h) > (battle_rect.y + battle_rect.h))
					enemies[i].rectangle.y = (battle_rect.y + battle_rect.h) - enemies[i].rectangle.h;
			}
			else { //Если размеры зомби стали превышать высоту боевого окна
				enemies[i].active = false;
				spawn_places[enemies[i].spawn_point].open = true;
				//Наносим урон убежищу
				enemies_count_in_wave--;
				g_food -= enemies[i].loss_food;
				g_resourses -= enemies[i].loss_resourses;
				g_humans -= enemies[i].loss_humans;
				CheckForZeroResourses();
			}
			if (enemies[i].rectangle.x < battle_rect.x) // Левая граница
				enemies[i].rectangle.x = battle_rect.x; // Выравниваем по левому краю
			if ((enemies[i].rectangle.x + enemies[i].rectangle.w) > (battle_rect.x + battle_rect.w)) // Правая граница
				enemies[i].rectangle.x = (battle_rect.x + battle_rect.w) - enemies[i].rectangle.w; //Выравниваем по правому краю
		}
}

//"Спавнит" нового врага в случайной начальной точке
//Это происходит, только при needness == true.
//Если зомби заспавнен, возвращает 1, иначе 0
int Spawn(bool needness, int type, SpawnPoint spawn_places[], Enemy* enemy, SDL_Texture* enemy_textures[], int enemies_hp[], int enemies_resourses_loss[][3]) {
	if (needness) {
		int rand_spawnposition = rand() % 3; //Случайное место для спавна
		if (spawn_places[rand_spawnposition].open == true) { //Если место для спавна свободно
			(*enemy).active = true;
			(*enemy).type = (rand() % 1); //Делаем этого врага случайным типом
			(*enemy).texture = enemy_textures[(*enemy).type]; //Присваиваем врагу текстуру в зависимости от его типа
			(*enemy).hp = enemies_hp[(*enemy).type]; //Присваиваем врагу жизни в зависимости от его типа
			//Присваиваем врагу потери ресурсов в зависимости от его типа
			(*enemy).loss_food = enemies_resourses_loss[(*enemy).type][0];
			(*enemy).loss_resourses = enemies_resourses_loss[(*enemy).type][1];
			(*enemy).loss_humans = enemies_resourses_loss[(*enemy).type][2];
			spawn_places[rand_spawnposition].open = false; // Запрещаем спавнить в этом месте ещё одного врага
			(*enemy).rectangle.w = 0; (*enemy).rectangle.h = 0;
			(*enemy).rectangle.x = spawn_places[rand_spawnposition].point.x; //Отправляем врага в точку спавна
			(*enemy).rectangle.y = spawn_places[rand_spawnposition].point.y;
			(*enemy).spawn_point = rand_spawnposition;
			return 1;
		}
	}
	return 0;
}

//наносим урон врагу, возвращает 1 , если зомби жив и 0 , если умер
int cause_damage_enemy(Enemy *enemy, int damage)
{
	int new_hp = enemy->hp - damage;
	if (new_hp <= 0)
		return 0;
	enemy->hp = new_hp;
	return 1;
}

//проверяет нахождение мышки на зомби, возвр номер зомби, если находится, и -1 , если нет
int check_mouse_on_enemy(Enemy enemies[], int i, SDL_Event event)
{
	if (event.button.x >= enemies[i].rectangle.x && event.button.x <= enemies[i].rectangle.x + enemies[i].rectangle.w &&
		event.button.y >= enemies[i].rectangle.y && event.button.y <= enemies[i].rectangle.y + enemies[i].rectangle.h
		&& enemies[i].active == true)
		return i;
	return -1;
}

//атакуем зомби из оружия возвращает 1 , если зомби не умер после атаки, и -1 , если умер
int Attack_Enemy(Enemy *enemy, int gun_type)
{
	int damage;
	switch (gun_type)//TODO: как-то определить урон и звук выстрела какой-нибудь
	{
	case 1://пистолет
	{
		damage = 1;
		break;
	}
	case 2://дробовик
	{
		damage = 3;
		break;
	}
	case 3://автомат
	{
		damage = 3;
		break;
	}
	case 4://граната
	{
		damage = 5;
		break;
	}
		
	default:
		damage = 0;
		break;
	}
	
	if (cause_damage_enemy(enemy, damage))
		return 1;
	return -1;
}

//создаем волну зомби по какой-то формуле от сложности , возвращает кол-во зобми в волне
int generate_wave(int difficulty, int *enemies_wave)
{
	//TODO: Продумать волну зомби

	int range_min = 1;
	int range_max = fmin(double(difficulty % 10), double(3));
	int enemies = 0;

	for (int i = 0; i < NUMBER_OF_TYPES_OF_ENEMIES; i++)
	{
		enemies_wave[i] =  rand() % (range_max - range_min + 1) + range_min;
		enemies += enemies_wave[i];
	}

	return enemies;
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
	//Текстуры врагов
	SDL_Surface* zombie1_surf = SDL_LoadBMP("resourses/textures/enemies/zombie1.bmp");
	SDL_SetColorKey(zombie1_surf, 1, SDL_MapRGB(zombie1_surf->format, 0, 255, 0));
	SDL_Texture* zombie1_texture = SDL_CreateTextureFromSurface(renderer, zombie1_surf);
	SDL_FreeSurface(zombie1_surf);

	SDL_Texture* enemy_textures[1] = { zombie1_texture };

	//Заполняем структуру заднего фона
	SDL_Rect background_rect = {0,0,winsize_w,winsize_h};
	Background background = {background_texture,background_rect};
	
	SDL_Rect battle_rect = {19,184,758,400}; //Прямоугольник места сражения

	//Строки для хранения числовых значений людей, еды и ресурсов.
	char* text_humans = new char[10];
	char* text_food = new char[10];
	char* text_resourses = new char[10];
	//Массив строк для удобной передачи в ф-ию
	char* texts[6] = { text_humans, text_food, text_resourses};

	//Описание кнопок
	SDL_Rect return_button = { 304,5,118,46 }; //Кнопка для возвращения к экрану "город"
	SDL_Rect gun_button1 = { 304,51,124,95 }; //Кнопка пистолета
	SDL_Rect gun_button2 = { 429,51,124,95 }; //Кнопка автомата
	SDL_Rect gun_button3 = { 553,51,124,95 }; //Кнопка пулемета
	SDL_Rect gun_button4 = { 677,51,124,95 }; //Кнопка гранаты

	//Массив кнопок
	SDL_Rect buttons[5] = { return_button, gun_button1 , gun_button2, gun_button3, gun_button4};

	//Описание мест появление врагов
	//TODO: Выбрать места для спавна зомби получше и побольше.
	SpawnPoint spawn_point1 = { {75,424} };
	SpawnPoint spawn_point2 = { {379,424} };
	SpawnPoint spawn_point3 = { {645,424} };

	//Массив мест для появления врагов
	SpawnPoint spawn_places[3] = { spawn_point1 ,spawn_point2 ,spawn_point3 };

	//Массив врагов
	Enemy enemies[NUMBER_OF_ENEMIES] = { {},{},{} };

	//Массив здоровья врагов
	//0-ой тип - первое значение, 1-ый тип - второе значение и т.д.
	int enemies_hp[NUMBER_OF_TYPES_OF_ENEMIES] = {
		1, //0-ой тип имеет 1 здоровье
	};

	//Массив урона ресурсам, которые наносит зомби, когда доходит до убежища
	//0-ой тип - первый элемент массива, в котором расположены: урон_еде, урон_ресурсам, урон_людям
	int enemies_resourses_loss[NUMBER_OF_TYPES_OF_ENEMIES][3] = {
		{0,0,33}, //0-ой тип наносит 0, 0, 1 урона соответственно еде, ресурсам и людям
	};

	//[тип] - кол-во зомби
	int enemies_wave[NUMBER_OF_TYPES_OF_ENEMIES];

	//Временно присваиваем всем врагам текстуры и тип обычного зомбо
	/*
	for (int i = 0; i <= NUMBER_OF_ENEMIES; i++)
	{
		enemies[i].texture = zombie1_texture; 
		enemies[i].type = 0;
	}
	*/

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

	int time_last_shot = 0;//время с выстрела для перезарядки
	int gun_cooldown_list[4] = { 1, 3, 3, 10 };
	//Текущее оружее 1 - пистолет, 2 - дробовик, 3 - автомат , 4 - граната
	int gun_type = 1;
	int weapony_level = 0;//уровень оружейной

	for (int i = 0; i < 2; i++)//обходим все комнаты
	{
		for (int j = 0; j < 3; j++)
		{
			if (g_rooms[i][j][0] == 4)//если построена оружейная
				weapony_level = g_rooms[i][j][1];
		}
	}

	//здесь хранится кол-во оставшихся в волне зомби 
	int enemies_count_in_wave;
	//создаем волну
	enemies_count_in_wave = generate_wave(difficulty, enemies_wave);
	printf_s("In wave :");
	for (int i = 0; i < NUMBER_OF_TYPES_OF_ENEMIES; i++)
	{
		printf_s("%d", enemies_wave[i]);
	}
	printf_s("Count: %d\n", enemies_count_in_wave);


	//зомби кончились или нет
	bool all_enemies_spawned = false;
	int time_last_spawn = 0;
	int spawn_cooldown = 0;


	while (!quit && !QUIT) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			QUIT = true;
			quit = true;
		}

		//TODO: Доработать боёвку
		
		
		//TODO: выбор оружия и вывод времени перезарядки 
		for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
			if (enemies[i].active == false && time(NULL) - time_last_spawn > spawn_cooldown)
			{
				for (int type = 0; type < NUMBER_OF_TYPES_OF_ENEMIES; type++)
				{
					if (enemies_wave[type] > 0) 
					{
						if (Spawn(true, type, spawn_places, &enemies[i], enemy_textures, 
							enemies_hp, enemies_resourses_loss))//если спавн удачный
						{
							enemies_wave[type]--;
							time_last_spawn = time(NULL);
							spawn_cooldown = type * 2 + rand() % 3 + 1;//TODO: как-то задать время спавна
						}
						
					}
				}
			}
		}

		//Каждый раз в цикле говорим, что мышь не наведена ни на одну из кнопок
		int button_flag = -1;
		if (LKMPressed(event))//обработка нажатий
		{
			for (int i = 0; i <= 4; i++) 
			{
				button_flag = CheckIfMouseOnButton(event, i, buttons);
				switch(button_flag)
				{
				case 0://кнопка возврата
				{
					if (!raid_is_over)  //Если нажали на кнопку "назад", но рейд ещё не окончен
						button_block.shown = true; //...рисуем блок на кнопке.
					else //Если нажали на кнопку "назад", и рейд окончен
						goback_to_train = true; //...возвращаемся к "городу"
					break;
				}
				case 1://кнопка пистолета
				{
					gun_type = 1;
					//TODO: выбор оружия ( рамочку около выбранного и затемнение недоступного)
					break;
				}
				case 2://кнопка автомата
				{
					if(weapony_level >= 1)//если построкна оруж 1 уровня
					{
						gun_type = 2;
					}
					break;
				}
				case 3://кнопка пулемета
				{
					if (weapony_level >= 2)//если построкна оруж 2 уровня
					{
						gun_type = 3;
					}
					break;
				}
				case 4://кнопка гранаты
				{
					if (weapony_level >= 3)//если построкна оруж 3 уровня
					{
						gun_type = 4;
					}
					break;
				}
				default:
					break;
				}

			}
			for (int i = 0; i <= NUMBER_OF_ENEMIES; i++) //проверим зомби
			{
				button_flag = check_mouse_on_enemy(enemies, i, event);
				if (button_flag != -1)//если наведена на зомби, тогда стреляем
				{
					printf_s("Z1: %d Z2: %d Z3: %d \n", enemies[0].hp, enemies[1].hp, enemies[2].hp);
					//проверка на время перезарядки
					int time_now = time(NULL);
					if (time_now - time_last_shot > gun_cooldown_list[gun_type - 1])//перезарядилось или нет
					{
						time_last_shot = time_now;//время для отсчета перезарядки
						if (Attack_Enemy(&enemies[button_flag], gun_type) == -1)//если зомби умер
						{
							enemies[button_flag].active = false;
							spawn_places[enemies[i].spawn_point].open = true;
							enemies_count_in_wave--;
							printf_s("DEAD!\n");
						}
						if (gun_type == 1 || gun_type == 2)// если пистолет или дробовик
							break;//выйдем из цикла, чтобы выстрел был только по одному зомби, даже если их несколько, винтовка и граната пробьет всех
					}
					
				}
			}
		}		
		//Подготавливаем текст для вывода
		_itoa_s(int(g_humans), text_humans, 10, 10);
		_itoa_s(int(g_food), text_food, 10, 10);
		_itoa_s(int(g_resourses), text_resourses, 10, 10);

		Update_EnemiesPosition(enemies, battle_rect, spawn_places, enemies_count_in_wave);

		Update(window, renderer, background, texts, button_block, enemies);
		//TODO: баг с наложением

		SDL_Delay(10);
		

		//Считаем время
		int tick_time = SDL_GetTicks();
		DELTA = tick_time - LASTTICKTIME;
		LASTTICKTIME = tick_time;

		if (enemies_count_in_wave <= 0 && raid_is_over == false)
		{
			button_block.shown = false;
			raid_is_over = true;
		}
			

		if (raid_is_over && goback_to_train)// когда кончится рейд вернемся в город
			break;
	}	

	SDL_DestroyTexture(background_texture);
	SDL_DestroyTexture(block_texture);
	SDL_DestroyTexture(zombie1_texture);
	return 0;
}