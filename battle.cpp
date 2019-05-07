#pragma once
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
extern int DELTA;
extern int LASTTICKTIME;

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
void Update_EnemiesPosition(Enemy enemies[], SDL_Rect battle_rect, SpawnPoint spawn_places[]) {
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
void Spawn(bool needness, int type, SpawnPoint spawn_places[], Enemy* enemy, SDL_Texture* enemy_textures[], int enemies_hp[], int enemies_resourses_loss[][3]) {
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
		}
	}
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
		event.button.y >= enemies[i].rectangle.y && event.button.y <= enemies[i].rectangle.y + enemies[i].rectangle.h)
		return i;
	return -1;
}

//атакуем зомби из оружия возвращает 1 , если зомби не умер после атаки, и -1 , если умер
int Attack_Enemy(Enemy *enemy, int gun_type)
{
	int damage;
	switch (gun_type)//TODO: как-то определить урон и отнять ресы
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
	SDL_Rect return_button = {304,5,118,46}; //Кнопка для возвращения к экрану "город"

	//Массив кнопок
	SDL_Rect buttons[1] = { return_button};

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

	while (!quit && !QUIT) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			QUIT = true;
			quit = true;
		}

		//TODO: Доработать боёвку

		for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
			if(enemies[i].active == false) Spawn(true, 0, spawn_places, &enemies[i], enemy_textures,enemies_hp,enemies_resourses_loss);
		}

		//Каждый раз в цикле говорим, что мышь не наведена ни на одну из кнопок
		int button_flag = -1;
		if (LKMPressed(event))//обработка нажатий
		{
			printf_s("Z1: %d Z2: %d Z3: %d \n", enemies[0].hp, enemies[1].hp, enemies[2].hp);
			for (int i = 0; i <= 0; i++) 
			{
				button_flag = CheckIfMouseOnButton(event, i, buttons);
				if (button_flag == 0 && !raid_is_over) { //Если нажали на кнопку "назад", но рейд ещё не окончен
					button_block.shown = true; //...рисуем блок на кнопке.
				}
				else if (button_flag == 0 && raid_is_over) {//Если нажали на кнопку "назад", и рейд окончен
					goback_to_train = true; //...возвращаемся к "городу"
				}
			}
			for (int i = 0; i <= NUMBER_OF_ENEMIES; i++) //проверим зомби
			{
				button_flag = check_mouse_on_enemy(enemies, i, event);
				if (button_flag != -1)//если наведена на зомби, тогда стреляем
				{
					//проверка на время перезарядки
					int time_now = time(NULL);
					if (time_now - time_last_shot > gun_cooldown_list[gun_type - 1])
					{
						printf_s("пиупиу %d\n", time_now - time_last_shot);//время для отсчета перезарядки
						time_last_shot = time_now;//время для отсчета перезарядки
						if (Attack_Enemy(&enemies[button_flag], gun_type) == -1)//если зомби умер
						{
							enemies[button_flag].active = false;
							spawn_places[enemies[i].spawn_point].open = true;
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

		Update_EnemiesPosition(enemies, battle_rect, spawn_places);

		Update(window, renderer, background, texts, button_block, enemies);

		SDL_Delay(10);
		

		//Считаем время
		int tick_time = SDL_GetTicks();
		DELTA = tick_time - LASTTICKTIME;
		LASTTICKTIME = tick_time;

	}	

	SDL_DestroyTexture(background_texture);
	SDL_DestroyTexture(block_texture);
	SDL_DestroyTexture(zombie1_texture);
	return 0;
}