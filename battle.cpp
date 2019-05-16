#pragma once
#include <stdio.h>
#include <SDL.h>
#include "menu.h"
#include <time.h>
#include "town.h"
#include "notifications.h"
#include "main.h"
#include "train.h"
#include "battle.h"
#include <stdlib.h>
#define NUMBER_OF_ENEMIES 3
#define NUMBER_OF_TYPES_OF_ENEMIES 1

//bool draw = false; // рисоваить ее или нет
//int sound_number; номер звука
//SDL_Rect init_rect; // где свавнится
//SDL_Rect current_rect; // где сейчас
//SDL_Texture* texture; // текстура
struct Grenade 
{
	bool draw = false;
	int sound_number;
	SDL_Rect init_rect;
	SDL_Rect current_rect;
	SDL_Texture* texture;
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

//рисует поверх кнопки красивое затемнение
void draw_blackout(SDL_Window* window, SDL_Renderer* renderer, SDL_Rect rect, 
	float percentage_of_recharge = 0, int blackout_level = 128)
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); //Разрешаем смешивание цветов
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, blackout_level);

	SDL_Rect blackout = rect; //затемнение, размерами с rect)
	blackout.h = rect.h - rect.h * percentage_of_recharge; //пересчет высоты только для перезарядки
	SDL_RenderFillRect(renderer, &blackout); //Отрисовываем затемнение окна

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE); //Хватит смешивать цвета!
}


void Update(SDL_Window* window, SDL_Renderer* renderer, Background background, char* texts[], Town_block button_block,
	Enemy enemies[], SDL_Rect rect_zombi_text, SDL_Rect rectzombicount, Gun guns[],int wearony_level) {
	draw_background(renderer, background);


	//Рисуем ресурсы
	draw_number_text(window, renderer, g_humans, g_recthumans, check_full_stock(0) + 2, true);
	draw_number_text(window, renderer, g_food, g_rectfood, check_full_stock(1) + 2, true);
	draw_number_text(window, renderer, g_resourses, g_rectresourses, check_full_stock(2) + 2, true);
	draw_text(window, renderer, texts[3], rect_zombi_text,0,true);
	draw_text(window, renderer, texts[4], rectzombicount,0,true);

	draw_town_block(renderer, button_block);

	for (int i = 0; i < NUMBER_OF_ENEMIES; i++)
		draw_enemy(renderer, enemies[i]);

	for (int gun_type = 1; gun_type < 5; gun_type++)// пройдемся по всему оружию
	{
		if (wearony_level < gun_type-1)
			draw_blackout(window, renderer, guns[gun_type - 1].button_rect, 0, 160);


		if (guns[gun_type - 1].time_end_reload <= guns[gun_type - 1].cooldown) {//если не перезарядилось
			//считаем насколько проц перезарядилось
			float percentage_of_recharge = guns[gun_type - 1].time_end_reload / guns[gun_type - 1].cooldown;
			//рисуем затемнение на такой процент
			draw_blackout(window, renderer, guns[gun_type - 1].button_rect, percentage_of_recharge);
		}
	}
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

//проверяет нахождение мышки в прямоугольнике, возвр 1, если находится, и 0 , если нет
int check_mouse_on_rect(SDL_Rect rectangle, SDL_Event event)
{
	if (event.button.x >= rectangle.x && event.button.x <= rectangle.x + rectangle.w &&
		event.button.y >= rectangle.y && event.button.y <= rectangle.y + rectangle.h)
		return 1;
	return 0;
}

//атакуем зомби из оружия возвращает 1 , если зомби не умер после атаки, 
// -1 , если умер, 
// 0, если промах
int Attack_Enemy(Gun *gun, Enemy *enemy = NULL)
{
	gun->time_end_reload = 0;
	g_resourses -= gun->cost;
	play_sound(gun->sound_number);
	if (enemy == NULL)//промах
		return 0;
	if (cause_damage_enemy(enemy, gun->damage))
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

//Экран "сражение". Если передать custom_difficulty != -1 ,то для ЭТОГО сражения будет использована специальная сложность
//Возврат 0 -> окно закрыто
int battle_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h, int custom_difficulty) {
	//Вызываем оповещение о начале рейда
	play_sound(101);
	draw_blackout(window, renderer, { 0, 0, winsize_w, winsize_h }, 0, 128);
	call_notificaton(window, renderer, (char*)(u8"Мы под атакой!")); 
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
	//текстура гранаты
	SDL_Surface* grenade_surf = SDL_LoadBMP("resourses/textures/grenade.bmp");
	SDL_SetColorKey(grenade_surf, 1, SDL_MapRGB(grenade_surf->format, 0, 255, 0));
	SDL_Texture* grenade_texture = SDL_CreateTextureFromSurface(renderer, grenade_surf);
	SDL_FreeSurface(grenade_surf);
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
	//строки для хран инфо о кол-ве зомби
	char* text_zombitext = (char*)(u8"Врагов осталось: ");
	char* text_zombicount = new char[10];

	////строки для хран числ знач перезарядки
	//char* text_cooldown1 = new char[10];
	//char* text_cooldown2 = new char[10];
	//char* text_cooldown3 = new char[10];
	//char* text_cooldown4 = new char[10];
	

	//Массив строк для удобной передачи в ф-ию
	char* texts[5] = { NULL, NULL, NULL, text_zombitext, text_zombicount};

	//Описание кнопок
	SDL_Rect return_button = { 304,5,118,46 }; //Кнопка для возвращения к экрану "город"
	SDL_Rect gun_button1 = { 297,54,124,95 }; //Кнопка пистолета
	SDL_Rect gun_button2 = { 422,54,124,95 }; //Кнопка автомата
	SDL_Rect gun_button3 = { 547,54,124,95 }; //Кнопка пулемета
	SDL_Rect gun_button4 = { 672,54,124,95 }; //Кнопка гранаты

	SDL_Rect rect_zombi_text = { 32,197,125,21 };
	SDL_Rect rect_zombi_count = { 160,197,17,21 };
	
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

	//Блокиратор кнопки "назад"
	Town_block button_block;
	button_block.rectangle = return_button;
	button_block.texture = block_texture;
	button_block.shown = false;

	SDL_Event event;

	bool quit = false; //Выход из программы
	bool goback_to_train = false; //Флаг возврата к экрану "город"
	bool raid_is_over = false; //Флаг окончания рейда
	
	//вся инфа об оружии
	Gun guns[4];
	{
		//пистолет
		guns[0].cooldown = 2;
		guns[0].cost = 2;
		guns[0].damage = 2;
		guns[0].cooldown_rect = { 303,135,34,10 };
		guns[0].button_rect = gun_button1;
		guns[0].sound_number = 1;

		//автомат
		guns[1].cooldown = 3;
		guns[1].cost = 3;
		guns[1].damage = 3;
		guns[1].cooldown_rect = { 426,135,34,10 };
		guns[1].button_rect = gun_button2;
		guns[1].sound_number = 2;
		//пулемет
		guns[2].cooldown = 0.3;
		guns[2].cost = 1;
		guns[2].damage = 1;
		guns[2].cooldown_rect = { 551,135,34,10 };
		guns[2].button_rect = gun_button3;
		guns[2].sound_number = 3;
		//граната
		guns[3].cooldown = 10;
		guns[3].cost = 10;
		guns[3].damage = 10;
		guns[3].cooldown_rect = { 676,135,34,10 };
		guns[3].button_rect = gun_button4;
		guns[3].sound_number = 4;
	}
	//Текущее оружее 1 - пистолет, 2 - автомат, 3 - пулемет , 4 - граната
	int gun_type = 1;
	int weapony_level = get_building_level(4);//уровень оружейной
	Grenade grenage;
	grenage.init_rect = {};
	grenage.texture = grenade_texture;

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

	//Играем боевую музыку
	play_music(1);

	while (!quit && !QUIT) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			QUIT = true;
			quit = true;
		}

		//TODO: Доработать боёвку
		for (int i = 0; i < NUMBER_OF_ENEMIES; i++) {
			if (enemies[i].active == false && time(NULL) - time_last_spawn > spawn_cooldown)
			{
				//type - тип врага
				//Перебираем все типы врагов в массиве enemies_wave
				for (int type = 0; type < NUMBER_OF_TYPES_OF_ENEMIES; type++)
				{
					//Если врагов типа type осталось больше 0
					if (enemies_wave[type] > 0) 
					{
						//Пытаемся заспавнить врага
						if (Spawn(true, type, spawn_places, &enemies[i], enemy_textures, 
							enemies_hp, enemies_resourses_loss))//если спавн удачный
						{
							enemies_wave[type]--; //Вычитаем одного врага из данного типа врагов
							time_last_spawn = time(NULL); //Устанавливаем время последнего спавна
							spawn_cooldown = type * 2 + rand() % 3 + 1; //Устанавливаем время до следующего появления врагов
							//TODO: как-то задать время спавна
						}
						
					}
				}
			}
		}

		//Каждый раз в цикле говорим, что мышь не наведена ни на одну из кнопок
		int button_flag = -1;
		int zombie_flag = -1;

		if (LKMPressed(event))//обработка нажатий
		{
			//кнопки
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
					break;
				}
				case 2://кнопка автомата
				{
					if(weapony_level >= 1)//если построкна оруж 1 уровня
						gun_type = 2;
					break;
				}
				case 3://кнопка пулемета
				{
					if (weapony_level >= 2)//если построкна оруж 2 уровня
						gun_type = 3;
					break;
				}
				case 4://кнопка гранаты
				{
					if (weapony_level >= 3)//если построкна оруж 3 уровня
						gun_type = 4;
					break;
				}
				default:
					break;
				}
			}
			
			//если перезарядилось оружие и есть ресурсы на выстрел и вообще стреляем где-то в поле
			if (guns[gun_type - 1].active && g_resourses-guns[gun_type-1].cost >= 0 
				&& check_mouse_on_rect(battle_rect, event))
			{
				//стрельнули по зомби или нет
				bool gun_is_shot = false;
				
				for (int i = 0; i <= NUMBER_OF_ENEMIES; i++) //проверим зомби
				{
					zombie_flag = check_mouse_on_enemy(enemies, i, event);
					if (zombie_flag != -1)//если попали
					{
						gun_is_shot = true;
						//нанесли урон и если зомби умер
						if (Attack_Enemy(&guns[gun_type - 1], &enemies[zombie_flag]) == -1)
						{
							enemies[zombie_flag].active = false;
							spawn_places[enemies[i].spawn_point].open = true;
							enemies_count_in_wave--;
							printf_s("DEAD!\n");
						}
						//выйдем из цикла, чтобы выстрел был только по одному зомби, 
						//даже если их несколько, винтовка и граната пробьет всех
						if (gun_type == 1 || gun_type == 3)// если пистолет или пулемет
							break;
					}
				}
				if (!gun_is_shot)//если еще не стрельнули, тогда стреляем в воздух
					Attack_Enemy(&guns[gun_type - 1]);
				guns[gun_type - 1].active = false;//оружие перезаряжется
			}		
		}		
		//Подготавливаем текст для вывода
		_itoa_s(int(enemies_count_in_wave), text_zombicount, 10, 10);


		Update_EnemiesPosition(enemies, battle_rect, spawn_places, enemies_count_in_wave);

		Update(window, renderer, background, texts, button_block, enemies, rect_zombi_text, rect_zombi_count, guns, weapony_level);
		//TODO: баг с наложением

		SDL_Delay(10);
		

		//Считаем время
		int tick_time = SDL_GetTicks();
		DELTA = tick_time - LASTTICKTIME;
		LASTTICKTIME = tick_time;

		for (int gun_type = 1; gun_type < 5; gun_type++)// проверим кд у всего оружия
		{
			if (guns[gun_type - 1].time_end_reload < guns[gun_type - 1].cooldown)
				guns[gun_type - 1].time_end_reload += DELTA * 0.001;  // время с последнего выстрела
			else
				guns[gun_type - 1].active = true;
		}

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
	SDL_DestroyTexture(grenade_texture);
	return 0;
}