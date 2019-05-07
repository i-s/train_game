#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "train.h"
#include "text.h"
#include "town.h"
#include "menu.h"
//#include "main.h"
//#include "textures.h"
#include <stdlib.h>
#include <SDL_mixer.h>
#include <time.h>
#define NUMBER_OF_TEXTURES 4
#define INCOME_FROM_FACTORY_1 0.2
#define INCOME_FROM_FACTORY_2 0.4
#define INCOME_FROM_FERM_1 1
#define MAX_ROOM_LEVEL 3
#define COUNT_ROOMS 6

int GAME_OVER = 0;

//Объявление глобальных ресурсов:
//глобальные люди
float g_humans;
//глобальные материалы
float g_resourses;
//глобальная еда
float g_food;
// глобальное состояние рычагов
int lever1_pulled, lever2_pulled;
// таймер 1 секунды
float one_second;
//таймер до прибытия рейда\зомби
float g_time_before_raid;
// улучшения комнат
int g_rooms[2][3][2]; //2 ряда по 3 колонки по 2 числа в ячейке: тип и уровень
//доход от комнат
float g_income_food;
float g_income_res;
float g_income_hum;

float g_humans_cap = 100; //Максимальное количество людей, проживающих в бункере
float g_resourses_cap = 60; //Максимальное количество ресурсов, находящихся в бункере
float g_food_cap = 60; //Максимальное количество еды, находящейся в бункере


//Загрузка глобальных ресурсов
// подсчёт времени
extern int LASTTICKTIME;
extern int DELTA; //Разница во времени
extern int GAMESTARTTIME;
extern int GAMETIME; //Время со старта программы
extern float TIMEUNTILTRAIN;//время до поезда
extern bool QUIT;

//Запускался ли экран "город" до этого момента
bool g_have_open_town_before = false;

//Громкость музыки в игре. Должна принимать значения от 0 до 128.
int g_music_volume= 10;

//Глобальная переменная, отвечающая за сложность приезда поезда. Чем больше: тем хуже игроку
int difficulty;

//Объявление глобальных SDL_Rect для ресурсов:
SDL_Rect g_recthumans = { 66,2,197,44 };
SDL_Rect g_rectfood = { 66,47,197,51 };
SDL_Rect g_rectresourses = { 66,99,197,49 };
SDL_Rect rect_cost_humans = { 31,573,60,20 };
SDL_Rect rect_cost_food = { 111,573,60,20 };
SDL_Rect rect_cost_resourses = { 191,573,60,20 };

extern struct Background;

//Рисует фон
void draw_background(SDL_Renderer* renderer, Background background) {
	SDL_RenderCopy(renderer, background.texture, NULL, &background.rectangle);
}

//Рисует обводку окна
void draw_room_selecting(SDL_Renderer* renderer, Room_Selecting room_selecting) {
	SDL_RenderCopy(renderer, room_selecting.texture, NULL, &room_selecting.rectangle);
}

//Рисует комнату
void draw_room(SDL_Renderer* renderer, Room room) {
	SDL_RenderCopy(renderer, room.texture, NULL, &room.rectangle);
}

//Определяет текстуру комнаты
void Define_Rooms_Textures(Room* room, SDL_Texture* room_textures[]) {
	switch ((*room).type) {
	case 0: (*room).texture = room_textures[0]; break;
	case 1: { //Ферма
		switch ((*room).level) { //Уровни фермы
		case 1: (*room).texture = room_textures[1]; break;
		case 2:  (*room).texture = room_textures[2]; break;
		case 3:  (*room).texture = room_textures[3]; break;
		}
		break; }
	case 2: { //Фабрика
		switch ((*room).level) { //Уровни фабрики
		case 1: (*room).texture = room_textures[4]; break;
		case 2: (*room).texture = room_textures[5]; break;
		case 3: (*room).texture = room_textures[6]; break;
		}
		break; }
	case 3: { //Жилая комната
		switch ((*room).level) { //Уровни жилой комната
		case 1: (*room).texture = room_textures[7]; break;
		case 2: (*room).texture = room_textures[8]; break;
		case 3: (*room).texture = room_textures[9]; break;
		}
		break; }
	case 4: { //Оружейная
		switch ((*room).level) { //Уровни жилой комната
		case 1: (*room).texture = room_textures[10]; break;
		case 2: (*room).texture = room_textures[11]; break;
		case 3: (*room).texture = room_textures[12]; break;
		}
		break; }
	case 5: { //склад
		switch ((*room).level) { //Уровни жилой комната
		case 1: (*room).texture = room_textures[13]; break;
		case 2: (*room).texture = room_textures[14]; break;
		case 3: (*room).texture = room_textures[15]; break;
		}
		break; }
	case 6: { //Радиоточка
		switch ((*room).level) { //Уровни жилой комната
		case 1: (*room).texture = room_textures[16]; break;
		case 2: (*room).texture = room_textures[17]; break;
		case 3: (*room).texture = room_textures[18]; break;
		}
		break; }
	default: break;
	}
}

//Определяет время до следующего рейда
int get_new_battle_time() {
	return 10;
	//TODO: Придумать что-то с временем до рейда
}

//Рисует иконку комнаты
void draw_room_icon(SDL_Renderer* renderer, Room_icon room_icon) {
	SDL_RenderCopy(renderer, room_icon.texture, NULL, &room_icon.rectangle);
}

//Рисует сирену
void draw_alert(SDL_Renderer* renderer, Alert alert) {
	SDL_RenderCopy(renderer, alert.texture, NULL, &alert.rectangle);
}

//Изменяет количество ресурсов по формулам
void Update_resourses() {
	float delta_humans, delta_food, delta_resourses; //Хранят изменение количества ресурсов


	delta_food = g_humans / 100 * fmaxf((g_food/100),1); //Трата еды за 1 секунду равна:
	/*
	Количество людей        количество еды
	---------------- * max(---------------- , 1)
	      100                    100
	*/
	delta_resourses = g_humans / 500; //Трата ресурсов за секунду.

	g_food -= delta_food;
	g_food += g_income_food;
	g_resourses -= delta_resourses;
	g_resourses += g_income_res; 
	
	delta_humans = 0;

	if (g_food < 0) { //Если еды меньше 0...
		delta_humans = log(g_humans) + 0.1; //уменьшаем количество людей на
		/*
		( ln(количество людей) )^2
		*/
		g_food = 0;
	}
	if (g_humans >= 0.99)
		g_humans -= delta_humans;
	else
		GameOver();
	
	if (g_resourses < 0)
		g_resourses = 0;


	if (g_food / g_humans > 3)//если на одного человека еды больше чем 3
		g_humans += log(g_humans)*0.5;
	g_humans += g_income_hum;

	//проверка на ограничения по максимуму
	if (g_food > g_food_cap)
		g_food = g_food_cap;
	if (g_resourses > g_resourses_cap)
		g_resourses = g_resourses_cap;
	if (g_humans > g_humans_cap)
		g_humans = g_humans_cap;
}

//Заканчивает игру.
//В планах: выводит максимальные успехи игрока, количество убитых зомби, даёт сохранить картинку базы
void GameOver() {
	QUIT = true;
	printf_s("You are dead! Not big surprise.\n");
}


//Изменяет сложность игры по формулам
void Update_difficulty() {
	difficulty = int(g_resourses / 25) + int(g_humans / 50) + 1; //Пересчёт сложности игры
}

//Отрисовывает все изображения на экран
void Update(SDL_Window* window,SDL_Renderer* renderer, char* texts[], Background background, Room_Selecting room_selecting,
			Alert alert, Room_icon room_icons[], Room rooms[], 
			bool draw_cost = false) {
	draw_background(renderer, background);

	//Рисуем все комнаты
	for (int i = 0; i < 6; i++) {
		draw_room(renderer, rooms[i]);
	}

	if (alert.active) {
		draw_alert(renderer, alert);
	}

	//Рисуем рамку выделения комнаты
	if (room_selecting.shown) {
		draw_room_selecting(renderer, room_selecting);
	}

	//Рисуем все иконки из массива сразу
	for (int i = 0; i <= 7; i++) {
		draw_room_icon(renderer, room_icons[i]);
	}

	//рисуем рессурсы
	draw_text(window, renderer, texts[0], g_recthumans);
	draw_text(window, renderer, texts[1], g_rectfood);
	draw_text(window, renderer, texts[2], g_rectresourses);
	//рисует кол-во рессурсов нужное для апгрейта/строительства комнаты
	if (draw_cost) {
		draw_text(window, renderer, texts[3], rect_cost_humans);
		draw_text(window, renderer, texts[4], rect_cost_food);
		draw_text(window, renderer, texts[5], rect_cost_resourses);
	}
	SDL_RenderPresent(renderer);
}

//Экран "город".
//Возарат 2 -> переход к экрану "сражение"
//Возврат 1 -> переход к экрану "поезд"
//Возврат 0 -> завершение программы
int town_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h) {

	//Попытка красиво загружать текстуры в память
	/*
	const char textures_path[NUMBER_OF_TEXTURES][100] = { "resourses/textures/background_town_normal.bmp","resourses/textures/background_town_train_selected.bmp",
		"resourses/textures/room_selecting.bmp","resourses/textures/alert.bmp" }; //NUMBER_OF_TEXTURES расположений текстур
	const char textures_name[NUMBER_OF_TEXTURES][50] = {"background_texture","background_train_selected_texture",
	"room_selecting_texture","alert_texture"}; //NUMBER_OF_TEXTURES имён текстур
	const bool textures_chroma_keys[NUMBER_OF_TEXTURES] = {0,0,1,1};
	Texture textures[NUMBER_OF_TEXTURES];
	for (int i = 0; i < NUMBER_OF_TEXTURES; i++) {
		LoadTexture(renderer, textures[i],textures_name[i],textures_path[i],textures_chroma_keys[i]);
	}
	*/
	//Загружаем текстуры
			//Фона
	SDL_Surface* background_surf = SDL_LoadBMP("resourses/textures/background_town_normal.bmp");
	SDL_Texture* background_texture = SDL_CreateTextureFromSurface(renderer, background_surf);
	SDL_FreeSurface(background_surf);
	SDL_Surface* background_train_selected_surf = SDL_LoadBMP("resourses/textures/background_town_train_selected.bmp");
	SDL_Texture* background_train_selected_texture = SDL_CreateTextureFromSurface(renderer, background_train_selected_surf);
	SDL_FreeSurface(background_train_selected_surf);
	//Выделения комнаты
	SDL_Surface* room_selecting_surf = SDL_LoadBMP("resourses/textures/room_selecting.bmp");
	SDL_SetColorKey(room_selecting_surf, 1, SDL_MapRGB(room_selecting_surf->format, 0, 255, 0));
	SDL_Texture* room_selecting_texture = SDL_CreateTextureFromSurface(renderer, room_selecting_surf);
	SDL_FreeSurface(room_selecting_surf);
	//Сирены оповещения
	SDL_Surface* alert_surf = SDL_LoadBMP("resourses/textures/alert.bmp");
	SDL_SetColorKey(alert_surf, 1, SDL_MapRGB(alert_surf->format, 0, 255, 0));
	SDL_Texture* alert_texture = SDL_CreateTextureFromSurface(renderer, alert_surf);
	SDL_FreeSurface(alert_surf);
	//Комнат
	//обычная(пустая)
	SDL_Surface* stock_surf = SDL_LoadBMP("resourses/textures/rooms/stock.bmp");
	SDL_Texture* stock_texture = SDL_CreateTextureFromSurface(renderer, stock_surf);
	SDL_FreeSurface(stock_surf);
	//фермы
	SDL_Surface* farm1_surf = SDL_LoadBMP("resourses/textures/rooms/farm_1.bmp");
	SDL_Texture* farm1_texture = SDL_CreateTextureFromSurface(renderer, farm1_surf);
	SDL_FreeSurface(farm1_surf);
	SDL_Surface* farm2_surf = SDL_LoadBMP("resourses/textures/rooms/farm_2.bmp");
	SDL_Texture* farm2_texture = SDL_CreateTextureFromSurface(renderer, farm2_surf);
	SDL_FreeSurface(farm2_surf);
	SDL_Surface* farm3_surf = SDL_LoadBMP("resourses/textures/rooms/farm_3.bmp");
	SDL_Texture* farm3_texture = SDL_CreateTextureFromSurface(renderer, farm3_surf);
	SDL_FreeSurface(farm3_surf);

	//иконки фермы
	SDL_Surface* farm1_icon_surf = SDL_LoadBMP("resourses/textures/rooms/farm_1_icon.bmp");
	SDL_Texture* farm1_icon_texture = SDL_CreateTextureFromSurface(renderer, farm1_icon_surf);
	SDL_FreeSurface(farm1_icon_surf);

	//фабрика
	SDL_Surface* factory1_surf = SDL_LoadBMP("resourses/textures/rooms/factory_1.bmp");
	SDL_Texture* factory1_texture = SDL_CreateTextureFromSurface(renderer, factory1_surf);
	SDL_FreeSurface(factory1_surf);
	SDL_Surface* factory2_surf = SDL_LoadBMP("resourses/textures/rooms/factory_2.bmp");
	SDL_Texture* factory2_texture = SDL_CreateTextureFromSurface(renderer, factory2_surf);
	SDL_FreeSurface(factory2_surf);
	SDL_Surface* factory3_surf = SDL_LoadBMP("resourses/textures/rooms/factory_3.bmp");
	SDL_Texture* factory3_texture = SDL_CreateTextureFromSurface(renderer, factory3_surf);
	SDL_FreeSurface(factory3_surf);

	//иконки фабрики
	SDL_Surface* factory1_icon_surf = SDL_LoadBMP("resourses/textures/rooms/factory_1_icon.bmp");//тут поменять
	SDL_Texture* factory1_icon_texture = SDL_CreateTextureFromSurface(renderer, factory1_icon_surf);
	SDL_FreeSurface(factory1_icon_surf);

	//Жилые комнаты
	SDL_Surface* living1_surf = SDL_LoadBMP("resourses/textures/rooms/living_1.bmp");
	SDL_Texture* living1_texture = SDL_CreateTextureFromSurface(renderer, living1_surf);
	SDL_FreeSurface(living1_surf);
	SDL_Surface* living2_surf = SDL_LoadBMP("resourses/textures/rooms/living_2.bmp");
	SDL_Texture* living2_texture = SDL_CreateTextureFromSurface(renderer, living2_surf);
	SDL_FreeSurface(living2_surf);
	SDL_Surface* living3_surf = SDL_LoadBMP("resourses/textures/rooms/living_3.bmp");
	SDL_Texture* living3_texture = SDL_CreateTextureFromSurface(renderer, living3_surf);
	SDL_FreeSurface(living3_surf);

	//Иконка жилой комнаты
	SDL_Surface* living1_icon_surf = SDL_LoadBMP("resourses/textures/rooms/living_1_icon.bmp");//тут поменять
	SDL_Texture* living1_icon_texture = SDL_CreateTextureFromSurface(renderer, living1_icon_surf);
	SDL_FreeSurface(living1_icon_surf);

	//Оружейная
	SDL_Surface* weapony1_surf = SDL_LoadBMP("resourses/textures/rooms/weapony_1.bmp");
	SDL_Texture* weapony1_texture = SDL_CreateTextureFromSurface(renderer, weapony1_surf);
	SDL_FreeSurface(weapony1_surf);
	SDL_Surface* weapony2_surf = SDL_LoadBMP("resourses/textures/rooms/weapony_2.bmp");
	SDL_Texture* weapony2_texture = SDL_CreateTextureFromSurface(renderer, weapony2_surf);
	SDL_FreeSurface(weapony2_surf);
	SDL_Surface* weapony3_surf = SDL_LoadBMP("resourses/textures/rooms/weapony_3.bmp");
	SDL_Texture* weapony3_texture = SDL_CreateTextureFromSurface(renderer, weapony3_surf);
	SDL_FreeSurface(weapony3_surf);

	//Иконка оружейной
	SDL_Surface* weapony1_icon_surf = SDL_LoadBMP("resourses/textures/rooms/weapony_1_icon.bmp");//тут поменять
	SDL_Texture* weapony1_icon_texture = SDL_CreateTextureFromSurface(renderer, weapony1_icon_surf);
	SDL_FreeSurface(weapony1_icon_surf);

	//Кнопка update
	SDL_Surface* room_update_surf = SDL_LoadBMP("resourses/textures/rooms/update.bmp");//тут поменять
	SDL_Texture* room_update_texture = SDL_CreateTextureFromSurface(renderer, room_update_surf);
	SDL_FreeSurface(room_update_surf);

	//Кнопка delete
	SDL_Surface* room_delete_surf = SDL_LoadBMP("resourses/textures/rooms/delete.bmp");//тут поменять
	SDL_Texture* room_delete_texture = SDL_CreateTextureFromSurface(renderer, room_delete_surf);
	SDL_FreeSurface(room_delete_surf);

	//Cклад
	SDL_Surface* storage1_surf = SDL_LoadBMP("resourses/textures/rooms/storage_1.bmp");
	SDL_Texture* storage1_texture = SDL_CreateTextureFromSurface(renderer, storage1_surf);
	SDL_FreeSurface(storage1_surf);
	SDL_Surface* storage2_surf = SDL_LoadBMP("resourses/textures/rooms/storage_2.bmp");
	SDL_Texture* storage2_texture = SDL_CreateTextureFromSurface(renderer, storage2_surf);
	SDL_FreeSurface(storage2_surf);
	SDL_Surface* storage3_surf = SDL_LoadBMP("resourses/textures/rooms/storage_3.bmp");
	SDL_Texture* storage3_texture = SDL_CreateTextureFromSurface(renderer, storage3_surf);
	SDL_FreeSurface(storage3_surf);

	//Иконка склада
	SDL_Surface* storage1_icon_surf = SDL_LoadBMP("resourses/textures/rooms/storage_1_icon.bmp");//тут поменять
	SDL_Texture* storage1_icon_texture = SDL_CreateTextureFromSurface(renderer, storage1_icon_surf);
	SDL_FreeSurface(storage1_icon_surf);

	//Радио
	SDL_Surface* radio1_surf = SDL_LoadBMP("resourses/textures/rooms/radio_1.bmp");
	SDL_Texture* radio1_texture = SDL_CreateTextureFromSurface(renderer, radio1_surf);
	SDL_FreeSurface(radio1_surf);
	SDL_Surface* radio2_surf = SDL_LoadBMP("resourses/textures/rooms/radio_2.bmp");
	SDL_Texture* radio2_texture = SDL_CreateTextureFromSurface(renderer, radio2_surf);
	SDL_FreeSurface(radio2_surf);
	SDL_Surface* radio3_surf = SDL_LoadBMP("resourses/textures/rooms/radio_3.bmp");
	SDL_Texture* radio3_texture = SDL_CreateTextureFromSurface(renderer, radio3_surf);
	SDL_FreeSurface(radio3_surf);

	//Иконка радио
	SDL_Surface* radio1_icon_surf = SDL_LoadBMP("resourses/textures/rooms/radio_1_icon.bmp");//тут поменять
	SDL_Texture* radio1_icon_texture = SDL_CreateTextureFromSurface(renderer, radio1_icon_surf);
	SDL_FreeSurface(radio1_icon_surf);


	//Массив текстур комнат
	SDL_Texture* room_textures[19] = { stock_texture,farm1_texture, farm2_texture, farm3_texture, factory1_texture,
										factory2_texture , factory3_texture, living1_texture,
										living2_texture , living3_texture, weapony1_texture,
										weapony2_texture , weapony3_texture, storage1_texture, storage2_texture,
										storage3_texture, radio1_texture, radio2_texture, radio3_texture };
	
	//Рисуем задний план
	SDL_Rect bground_rectangle = { 0,0,winsize_w,winsize_h };
	Background background = { background_texture, bground_rectangle };
	draw_background(renderer, background);

	//Выделение комнаты
	SDL_Rect room_selecting_rect = { 0,0,231,150 };
	Room_Selecting room_selecting;
	room_selecting.rectangle = room_selecting_rect;
	room_selecting.texture = room_selecting_texture;
	room_selecting.shown = false;

	//Сирена оповещения
	SDL_Rect alert_rect = { 296,39,21,29 };
	Alert alert;
	alert.rectangle = alert_rect;
	alert.texture = alert_texture;
	alert.active = false;

	//Создаём ивент и переменную для отслеживания закрытия окна
	SDL_Event event;
	bool quit = false;

	//Строки для хранения числовых значений людей, еды и ресурсов.
	char* text_humans = new char[10];
	char* text_food = new char[10];
	char* text_resourses = new char[10];
	char* text_cost_humans = new char[10];
	char* text_cost_food = new char[10];
	char* text_cost_resourses = new char[10];

	//Массив строк для удобной передачи в ф-ию
	char* texts[6] = { text_humans, text_food, text_resourses, text_cost_humans, text_cost_food, text_cost_resourses };

	//Описание всех кнопок
	SDL_Rect rect_button_train = { 289,68,52,74 };
	SDL_Rect rect_room_0 = { 14,187,229,148 };
	SDL_Rect rect_room_1 = { 287,187,229,148 };
	SDL_Rect rect_room_2 = { 555,187,229,148 };
	SDL_Rect rect_room_3 = { 14,342,229,148 };
	SDL_Rect rect_room_4 = { 287,342,229,148 };
	SDL_Rect rect_room_5 = { 555,342,229,148 };

	SDL_Rect rect_room_icon_0 = { 29,504,56,56 };
	SDL_Rect rect_room_icon_1 = { 96,504,56,56 };
	SDL_Rect rect_room_icon_2 = { 163,504,56,56 };
	SDL_Rect rect_room_icon_3 = { 230,504,56,56 };
	SDL_Rect rect_room_icon_4 = { 297,504,56,56 };
	SDL_Rect rect_room_icon_5 = { 363,504,56,56 };

	SDL_Rect rect_room_update = { 430,504,56,56 };
	SDL_Rect rect_room_delete = { 497,504,56,56 };

	

	SDL_Rect buttons[15] = { rect_button_train,rect_room_0,rect_room_1,
		rect_room_2, rect_room_3,rect_room_4,rect_room_5,
		rect_room_icon_0,rect_room_icon_1,rect_room_icon_2,rect_room_icon_3,rect_room_icon_4,rect_room_icon_5,
		rect_room_update, rect_room_delete};
	//Создаём иконки
	Room_icon room_icons[8];
	{
		room_icons[0].rectangle = rect_room_icon_0;
		room_icons[0].texture = farm1_icon_texture;
		room_icons[1].rectangle = rect_room_icon_1;
		room_icons[1].texture = factory1_icon_texture;
		room_icons[2].rectangle = rect_room_icon_2;
		room_icons[2].texture = living1_icon_texture;
		room_icons[3].rectangle = rect_room_icon_3;
		room_icons[3].texture = weapony1_icon_texture;
		room_icons[4].rectangle = rect_room_icon_4;
		room_icons[4].texture = storage1_icon_texture;
		room_icons[5].rectangle = rect_room_icon_5;
		room_icons[5].texture = radio1_icon_texture;
		room_icons[6].rectangle = rect_room_update;
		room_icons[6].texture = room_update_texture;
		room_icons[7].rectangle = rect_room_delete;
		room_icons[7].texture = room_delete_texture;
	}
/* загрузка других комнат
	
	
	room_icons[3].rectangle = rect_room_icon_3;
	room_icons[3].texture = farm1_icon_texture;
*/
	//Массив комнат
	Room rooms[6];

	//доход от комнаты [тип][уровень][0.hum,1.food,2.res]
	float income_from_rooms[COUNT_ROOMS][MAX_ROOM_LEVEL][3];

	//инициализация дохода
	{
		//ферма
		{
			income_from_rooms[0][0][0] = 0;
			income_from_rooms[0][0][1] = 1;
			income_from_rooms[0][0][2] = 0;
	
			income_from_rooms[0][1][0] = 0;
			income_from_rooms[0][1][1] = 2.5;
			income_from_rooms[0][1][2] = 0;
	
			income_from_rooms[0][2][0] = 0;
			income_from_rooms[0][2][1] = 7.5;
			income_from_rooms[0][2][2] = 0;
		}
		//фабрика
		{
			income_from_rooms[1][0][0] = 0;
			income_from_rooms[1][0][1] = 0;
			income_from_rooms[1][0][2] = 1;
	
			income_from_rooms[1][1][0] = 0;
			income_from_rooms[1][1][1] = 0;
			income_from_rooms[1][1][2] = 2.45;
	
			income_from_rooms[1][2][0] = 0;
			income_from_rooms[1][2][1] = 0;
			income_from_rooms[1][2][2] = 4;
		}
		//Жилая комната
		{
			income_from_rooms[2][0][0] = 0.001;
			income_from_rooms[2][0][1] = 0;
			income_from_rooms[2][0][2] = 0;

			income_from_rooms[2][1][0] = 0.005;
			income_from_rooms[2][1][1] = 0;
			income_from_rooms[2][1][2] = 0;

			income_from_rooms[2][2][0] = 0.01;
			income_from_rooms[2][2][1] = 0;
			income_from_rooms[2][2][2] = 0;
		}
		//Оружейная
		{
			income_from_rooms[3][0][0] = 0;
			income_from_rooms[3][0][1] = 0;
			income_from_rooms[3][0][2] = -0.05;

			income_from_rooms[3][1][0] = 0;
			income_from_rooms[3][1][1] = 0;
			income_from_rooms[3][1][2] = -0.15;

			income_from_rooms[3][2][0] = 0;
			income_from_rooms[3][2][1] = 0;
			income_from_rooms[3][2][2] = -0.35;
		}
		//Cклад
		{
			income_from_rooms[4][0][0] = 0;
			income_from_rooms[4][0][1] = 0;
			income_from_rooms[4][0][2] = -0.05;

			income_from_rooms[4][1][0] = 0;
			income_from_rooms[4][1][1] = 0;
			income_from_rooms[4][1][2] = -0.15;

			income_from_rooms[4][2][0] = 0;
			income_from_rooms[4][2][1] = 0;
			income_from_rooms[4][2][2] = -0.35;
		}
		//Радио
		{
			income_from_rooms[5][0][0] = 0;
			income_from_rooms[5][0][1] = 0;
			income_from_rooms[5][0][2] = -0.05;

			income_from_rooms[5][1][0] = 0;
			income_from_rooms[5][1][1] = 0;
			income_from_rooms[5][1][2] = -0.15;

			income_from_rooms[5][2][0] = 0;
			income_from_rooms[5][2][1] = 0;
			income_from_rooms[5][2][2] = -0.35;
		}
	}
	//цена комнаты [тип][уровень][0.minhum,1.food,2.res]
	int cost_rooms[COUNT_ROOMS][MAX_ROOM_LEVEL][3];
	//инициализация цены
	{
		//ферма
		{
			cost_rooms[0][0][0] = 5;
			cost_rooms[0][0][1] = 10;
			cost_rooms[0][0][2] = 1;

			cost_rooms[0][1][0] = 50;
			cost_rooms[0][1][1] = 50;
			cost_rooms[0][1][2] = 3;

			cost_rooms[0][2][0] = 75;
			cost_rooms[0][2][1] = 100;
			cost_rooms[0][2][2] = 6;
		}
		//фабрика
		{
			cost_rooms[1][0][0] = 20;
			cost_rooms[1][0][1] = 50;
			cost_rooms[1][0][2] = 10;

			cost_rooms[1][1][0] = 40;
			cost_rooms[1][1][1] = 75;
			cost_rooms[1][1][2] = 20;

			cost_rooms[1][2][0] = 50;
			cost_rooms[1][2][1] = 100;
			cost_rooms[1][2][2] = 30;
		}
		//Жилая комната
		{
			cost_rooms[2][0][0] = 50;
			cost_rooms[2][0][1] = 100;
			cost_rooms[2][0][2] = 35;

			cost_rooms[2][1][0] = 100;
			cost_rooms[2][1][1] = 200;
			cost_rooms[2][1][2] = 50;

			cost_rooms[2][2][0] = 150;
			cost_rooms[2][2][1] = 250;
			cost_rooms[2][2][2] = 80;
		}
		//Оружейная
		{
			cost_rooms[3][0][0] = 25;
			cost_rooms[3][0][1] = 50;
			cost_rooms[3][0][2] = 50;

			cost_rooms[3][1][0] = 75;
			cost_rooms[3][1][1] = 50;
			cost_rooms[3][1][2] = 200;

			cost_rooms[3][2][0] = 125;
			cost_rooms[3][2][1] = 100;
			cost_rooms[3][2][2] = 300;
		}
		//склад
		{
			cost_rooms[4][0][0] = 25;
			cost_rooms[4][0][1] = 50;
			cost_rooms[4][0][2] = 25;

			cost_rooms[4][1][0] = 75;
			cost_rooms[4][1][1] = 50;
			cost_rooms[4][1][2] = 50;

			cost_rooms[4][2][0] = 125;
			cost_rooms[4][2][1] = 100;
			cost_rooms[4][2][2] = 75;
		}
		//радио
		{
			cost_rooms[5][0][0] = 25;
			cost_rooms[5][0][1] = 50;
			cost_rooms[5][0][2] = 150;

			cost_rooms[5][1][0] = 75;
			cost_rooms[5][1][1] = 50;
			cost_rooms[5][1][2] = 200;

			cost_rooms[5][2][0] = 125;
			cost_rooms[5][2][1] = 100;
			cost_rooms[5][2][2] = 300;
		}
	}
	
	//изменение комнатой ограничений [тип][уровень][0.maxhum,1.maxfood,2.maxres]
	int change_cap_rooms[COUNT_ROOMS][MAX_ROOM_LEVEL][3];

	//инициализация изменений ограничений
	{
		//ферма
		{
			change_cap_rooms[0][0][0] = 0;
			change_cap_rooms[0][0][1] = 0;
			change_cap_rooms[0][0][2] = 0;

			change_cap_rooms[0][1][0] = 0;
			change_cap_rooms[0][1][1] = 0;
			change_cap_rooms[0][1][2] = 0;

			change_cap_rooms[0][2][0] = 0;
			change_cap_rooms[0][2][1] = 0;
			change_cap_rooms[0][2][2] = 0;
		}
		//фабрика
		{
			change_cap_rooms[1][0][0] = 0;
			change_cap_rooms[1][0][1] = 0;
			change_cap_rooms[1][0][2] = 0;

			change_cap_rooms[1][1][0] = 0;
			change_cap_rooms[1][1][1] = 0;
			change_cap_rooms[1][1][2] = 0;

			change_cap_rooms[1][2][0] = 0;
			change_cap_rooms[1][2][1] = 0;
			change_cap_rooms[1][2][2] = 0;
		}
		//жилая комната
		{
			change_cap_rooms[2][0][0] = 20;
			change_cap_rooms[2][0][1] = 0;
			change_cap_rooms[2][0][2] = 0;

			change_cap_rooms[2][1][0] = 50;
			change_cap_rooms[2][1][1] = 0;
			change_cap_rooms[2][1][2] = 0;

			change_cap_rooms[2][2][0] = 100;
			change_cap_rooms[2][2][1] = 0;
			change_cap_rooms[2][2][2] = 0;
		}
		//оружейная
		{
			change_cap_rooms[3][0][0] = 00;
			change_cap_rooms[3][0][1] = 0;
			change_cap_rooms[3][0][2] = 0;

			change_cap_rooms[3][1][0] = 00;
			change_cap_rooms[3][1][1] = 0;
			change_cap_rooms[3][1][2] = 0;

			change_cap_rooms[3][2][0] = 0;
			change_cap_rooms[3][2][1] = 0;
			change_cap_rooms[3][2][2] = 0;
		}
		//склад
		{
			change_cap_rooms[4][0][0] = 0;
			change_cap_rooms[4][0][1] = 50;
			change_cap_rooms[4][0][2] = 50;

			change_cap_rooms[4][1][0] = 0;
			change_cap_rooms[4][1][1] = 150;
			change_cap_rooms[4][1][2] = 110;

			change_cap_rooms[4][2][0] = 0;
			change_cap_rooms[4][2][1] = 250;
			change_cap_rooms[4][2][2] = 175;
		}
		//радио
		{
			change_cap_rooms[5][0][0] = 0;
			change_cap_rooms[5][0][1] = 0;
			change_cap_rooms[5][0][2] = 0;

			change_cap_rooms[5][1][0] = 0;
			change_cap_rooms[5][1][1] = 0;
			change_cap_rooms[5][1][2] = 0;

			change_cap_rooms[5][2][0] = 0;
			change_cap_rooms[5][2][1] = 0;
			change_cap_rooms[5][2][2] = 0;
		}
		
	}
	
	//Заполняем массив комнат из глобального массива комнат
	for (int i = 0; i < 6; i++) {
		if (i < 3) {
			rooms[i].type = g_rooms[0][i][0];
			rooms[i].level = g_rooms[0][i][1];
		}
		else {
			rooms[i].type = g_rooms[1][i-3][0];
			rooms[i].level = g_rooms[1][i-3][1];
		}
		rooms[i].rectangle = buttons[1+i];
		rooms[i].texture = stock_texture;
		Define_Rooms_Textures(&rooms[i], room_textures);
	}

	int button_flag; //Перменная, хранящая номер нажатой кнопки

	//Запуск SDL_Mixer-а, инициализация музыки и её запуск
	Mix_Init(MIX_INIT_MP3);
	if (g_have_open_town_before == false) {
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
			printf("Mix_OpenAudio: %s\n", Mix_GetError());
			// Обработка ошибки
		}
		Mix_Music *music;
		music = Mix_LoadMUS("resourses/music/music_test.MP3");
		if (music == NULL) {
			printf("Mix_LoadMUS: %s\n", Mix_GetError());
			// Обработка ошибки
		}
		if (Mix_PlayMusic(music, -1) == -1) {
			printf("Mix_PlayMusic: %s\n", Mix_GetError());
			// Обработка ошибки
		}
		Mix_VolumeMusic(g_music_volume);
	}

	bool raid_started = false; //Флаг, отвечающий за начало рейда

	int go_to_train = false; //Флаг, отвечающий за переход к экрану "поезд"
	
	int choosed_room = -1; //Какая комната выбрана в данный момент
	bool draw_cost = false; // флаг вывода цены комнаты 1 лвла
	bool draw_update_cost = false; // флаг вывода цены комнаты 2-3 лвла
	int room_type = 0;//тип комнаты , для которой выводить цену
	bool draw_cost_yes = false;//рисовать ли цену комнаты сейчас
	
	//время последнего апдейта
	int time_last_update = time(NULL);

	//int time_of_last_click = GAMESTARTTIME; //Время предыдущего клика
	
	//ГЛАВНЫЙ ЦИКЛ
	while (!quit && !go_to_train && !raid_started && !QUIT) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			QUIT = true;
			quit = true;
		}

		bool was_background_changed = false; //Был ли фон изменён?
		bool was_button_room_pressed = false; //Была ли нажата какая-либо комната?
		bool was_rooms_changed = false; //Была ли хоть одна комната изменена?

		for (int i = 0; i <= 14; i++) {
			button_flag = CheckIfMouseOnButton(event, i, buttons);

			switch (button_flag) {
			case -1: break;
			//Нажатие на комнаты
			case 0: {
				background.texture = background_train_selected_texture;
				was_background_changed = true;
				if (LKMPressed(event)) {
					go_to_train = true; //Переход к экрану "поезд"
				}
				break;
			}
			case 1: {
				if (LKMPressed(event)) {
					choosed_room = button_flag;
					was_button_room_pressed = true;
				}
				break;
			}
			case 2: {
				if (LKMPressed(event)) {
					choosed_room = button_flag;
					was_button_room_pressed = true;
				}
				break;
			}
			case 3: {
				if (LKMPressed(event)) {
					choosed_room = button_flag;
					was_button_room_pressed = true;
				}
				break;
			}
			case 4: {
				if (LKMPressed(event)) {
					choosed_room = button_flag;
					was_button_room_pressed = true;
				}
				break;
			}
			case 5: {
				if (LKMPressed(event)) {
					choosed_room = button_flag;
					was_button_room_pressed = true;
				}
				break;
			}
			case 6: {
				if (LKMPressed(event)) {
					choosed_room = button_flag;
					was_button_room_pressed = true;
				}
				break;
			}
			case 7: { //мышка на кнопке фермы
				draw_cost = true;
				room_type = 1;
				if (LKMPressed(event)) {//Нажатие на иконку фермы
					if (choosed_room > -1 && rooms[choosed_room - 1].type == 0 && 
						g_food - cost_rooms[0][0][1] > 0 &&
						g_humans >= cost_rooms[0][0][0] &&
						g_resourses - cost_rooms[0][0][2] > 0) {

						rooms[choosed_room - 1].type = 1;
						rooms[choosed_room - 1].texture = farm1_texture;
						rooms[choosed_room - 1].level = 1;
						if (choosed_room - 1 < 3) {
							g_rooms[0][choosed_room - 1][0] = 1;
							g_rooms[0][choosed_room - 1][1] = 1;
						}
						else {
							g_rooms[1][choosed_room - 4][0] = 1;
							g_rooms[1][choosed_room - 4][1] = 1;
						}
						//вычитаеем цену комнаты
						g_food -= cost_rooms[0][0][1];
						g_resourses -= cost_rooms[0][0][2];

						was_rooms_changed = true;
						g_income_food += income_from_rooms[rooms[choosed_room - 1].type-1][rooms[choosed_room - 1].level-1][1];
					}
				}
				break;
			}
			case 8: { //мышка на кнопке фабрики
				draw_cost = true;
				room_type = 2;
				if (LKMPressed(event)) {//Нажатие на иконку фaбрики
					if (choosed_room > -1 && rooms[choosed_room - 1].type == 0 &&
						g_food - cost_rooms[1][0][1] >= 0 &&
						g_humans >= cost_rooms[1][0][0] &&
						g_resourses - cost_rooms[1][0][2] >= 0) {

						rooms[choosed_room - 1].type = 2;
						rooms[choosed_room - 1].texture = factory1_texture;
						rooms[choosed_room - 1].level = 1;
						if (choosed_room - 1 < 3) {
							g_rooms[0][choosed_room - 1][0] = 2;
							g_rooms[0][choosed_room - 1][1] = 1;
						}
						else {
							g_rooms[1][choosed_room - 4][0] = 2;
							g_rooms[1][choosed_room - 4][1] = 1;
						}
						//вычитаеем цену комнаты
						g_food -= cost_rooms[1][0][1];
						g_resourses -= cost_rooms[1][0][2];

						was_rooms_changed = true;
						g_income_res += income_from_rooms[rooms[choosed_room - 1].type-1][rooms[choosed_room - 1].level-1][2];
					}
				}
				break;
			}
			case 9: { //мышка на кнопке жил комнат
				draw_cost = true;
				room_type = 3;
				if (LKMPressed(event)) {//Нажатие на иконку жилых комнат
					if (choosed_room > -1 && rooms[choosed_room - 1].type == 0 &&
						g_food - cost_rooms[2][0][1] >= 0 &&
						g_humans >= cost_rooms[2][0][0] &&
						g_resourses - cost_rooms[2][0][2] >= 0) {

						rooms[choosed_room - 1].type = 3;
						rooms[choosed_room - 1].texture = living1_texture;
						rooms[choosed_room - 1].level = 1;
						if (choosed_room - 1 < 3) {
							g_rooms[0][choosed_room - 1][0] = 3;
							g_rooms[0][choosed_room - 1][1] = 1;
						}
						else {
							g_rooms[1][choosed_room - 4][0] = 3;
							g_rooms[1][choosed_room - 4][1] = 1;
						}
						//вычитаеем цену комнаты
						g_food -= cost_rooms[2][0][1];
						g_resourses -= cost_rooms[2][0][2];

						was_rooms_changed = true;
						g_humans_cap += change_cap_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][0];
					}
				}
				break;
			}
			case 10: { //мышка на кнопке оружйной
				draw_cost = true;
				room_type = 4;
				if (LKMPressed(event)) {//Нажатие на иконку оружейной
					if (choosed_room > -1 && rooms[choosed_room - 1].type == 0 &&
						g_food - cost_rooms[3][0][1] >= 0 &&
						g_humans >= cost_rooms[3][0][0] &&
						g_resourses - cost_rooms[3][0][2] >= 0) {

						rooms[choosed_room - 1].type = 4;
						rooms[choosed_room - 1].texture = weapony1_texture;
						rooms[choosed_room - 1].level = 1;
						if (choosed_room - 1 < 3) {
							g_rooms[0][choosed_room - 1][0] = 4;
							g_rooms[0][choosed_room - 1][1] = 1;
						}
						else {
							g_rooms[1][choosed_room - 4][0] = 4;
							g_rooms[1][choosed_room - 4][1] = 1;
						}
						//вычитаеем цену комнаты
						g_food -= cost_rooms[3][0][1];
						g_resourses -= cost_rooms[3][0][2];

						was_rooms_changed = true;
						g_income_res += income_from_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][2];
					}
				}
				break;
			}
			case 11: { //мышка на кнопке склада
				draw_cost = true;
				room_type = 5;
				if (LKMPressed(event)) {//Нажатие на иконку склада
					if (choosed_room > -1 && rooms[choosed_room - 1].type == 0 &&
						g_food - cost_rooms[4][0][1] >= 0 &&
						g_humans >= cost_rooms[4][0][0] &&
						g_resourses - cost_rooms[4][0][2] >= 0) {

						rooms[choosed_room - 1].type = 5;
						rooms[choosed_room - 1].texture = storage1_texture;
						rooms[choosed_room - 1].level = 1;
						if (choosed_room - 1 < 3) {
							g_rooms[0][choosed_room - 1][0] = 5;
							g_rooms[0][choosed_room - 1][1] = 1;
						}
						else {
							g_rooms[1][choosed_room - 4][0] = 5;
							g_rooms[1][choosed_room - 4][1] = 1;
						}
						//вычитаеем цену комнаты
						g_food -= cost_rooms[4][0][1];
						g_resourses -= cost_rooms[4][0][2];

						was_rooms_changed = true;
						g_resourses_cap += change_cap_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][2];
						g_food_cap += change_cap_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][1];
					}
				}
				break;
			}
			case 12: { //мышка на кнопке радио
				draw_cost = true;
				room_type = 6;
				if (LKMPressed(event)) {//Нажатие на иконку радио
					if (choosed_room > -1 && rooms[choosed_room - 1].type == 0 &&
						g_food - cost_rooms[5][0][1] >= 0 &&
						g_humans >= cost_rooms[5][0][0] &&
						g_resourses - cost_rooms[5][0][2] >= 0) {

						rooms[choosed_room - 1].type = 6;
						rooms[choosed_room - 1].texture = radio1_texture;
						rooms[choosed_room - 1].level = 1;
						if (choosed_room - 1 < 3) {
							g_rooms[0][choosed_room - 1][0] = 6;
							g_rooms[0][choosed_room - 1][1] = 1;
						}
						else {
							g_rooms[1][choosed_room - 4][0] = 6;
							g_rooms[1][choosed_room - 4][1] = 1;
						}
						//вычитаеем цену комнаты
						g_food -= cost_rooms[5][0][1];
						g_resourses -= cost_rooms[5][0][2];

						was_rooms_changed = true;
						//TODO: доделать радио

					}
				}
				break;
			}
			case 13: 
			{ 
				draw_update_cost = true;
				if (LKMPressed(event)) {//Нажатие на иконку апгрейда
					if (choosed_room > -1  //Команта выбрана
						&& rooms[choosed_room - 1].type != 0 //Комната построена
						&& time(NULL) - time_last_update + 0.5 > 1 //Не чаще раза в секунду
						&& rooms[choosed_room - 1].level < MAX_ROOM_LEVEL //Если уровень меньше 3-его
						&& g_food - cost_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level][1] > 0 //Если хвататет ресурсов
						&& g_humans >= cost_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level][0]
						&& g_resourses - cost_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level][2] > 0) {

						//rooms[choosed_room - 1].texture = 
						Define_Rooms_Textures(&rooms[choosed_room - 1], room_textures);

						//изменение инкома до апгрейта
						g_income_hum -= income_from_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][0];
						g_income_food -= income_from_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][1];
						g_income_res -= income_from_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][2];
						//изменение ограничений до 
						g_humans_cap -= change_cap_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][0];
						g_food_cap -= change_cap_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][1];
						g_resourses_cap -= change_cap_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][2];


						rooms[choosed_room - 1].level += 1;
						//вычитаем цену за апдейт
						g_food -= cost_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][1];
						g_resourses -= cost_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][2];

						//изменение инкома после апгрейта
						g_income_hum += income_from_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][0];
						g_income_food += income_from_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][1];
						g_income_res += income_from_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][2];
						//изменение ограничений после
						g_humans_cap += change_cap_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][0];
						g_food_cap += change_cap_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][1];
						g_resourses_cap += change_cap_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][2];


						if (choosed_room - 1 < 3) {
							g_rooms[0][choosed_room - 1][1] = rooms[choosed_room - 1].level;
						}
						else {
							g_rooms[1][choosed_room - 4][1] = rooms[choosed_room - 1].level;
						}
						was_rooms_changed = true;
						time_last_update = time(NULL);
					}
				}
				break;
			}
			case 14:
			{ //Нажатие на иконку удаления
				if (LKMPressed(event)) {
					if (choosed_room > -1  //Команта выбрана
						&& rooms[choosed_room - 1].type != 0 //Комната построена
						&& time(NULL) - time_last_update + 0.5 > 1 //Не чаще раза в секунду
						) {

						//изменение инкома  
						g_income_hum -= income_from_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][0];
						g_income_food -= income_from_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][1];
						g_income_res -= income_from_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][2];
						//изменение макс ограничений
						g_humans_cap -= change_cap_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][0];
						g_food_cap -= change_cap_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][1];
						g_resourses_cap -= change_cap_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][2];
						//возврат небольшого кол-ва ресурсов
						g_resourses += cost_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level - 1][2] * 0.25;

						rooms[choosed_room - 1].type = 0;
						rooms[choosed_room - 1].level = 0;
						Define_Rooms_Textures(&rooms[choosed_room - 1], room_textures);


						if (choosed_room - 1 < 3) {
							g_rooms[0][choosed_room - 1][1] = rooms[choosed_room - 1].level;
							g_rooms[0][choosed_room - 1][0] = rooms[choosed_room - 1].type;
						}
						else {
							g_rooms[1][choosed_room - 4][1] = rooms[choosed_room - 1].level;
							g_rooms[0][choosed_room - 1][0] = rooms[choosed_room - 1].type;
						}
						was_rooms_changed = true;
						time_last_update = time(NULL);
					}
				}
				break;
			}
			default: printf_s("There is a problem with a buttons!\n"); break;
			}
		}
		


		if (was_rooms_changed) {
			for (int i = 0; i < 6; i++) {
				Define_Rooms_Textures(&rooms[i], room_textures); //Изменение текстуры
			}
		}

		//Если комната выделена, нарисовать рамку.
		if (choosed_room != -1 && was_button_room_pressed == true) {
			room_selecting.rectangle = buttons[choosed_room];
			room_selecting.shown = true;
		}

		//Подготавливаем текст для вывода
		_itoa_s(int(g_humans), text_humans, 10, 10);
		_itoa_s(int(g_food), text_food, 10, 10);
		_itoa_s(int(g_resourses), text_resourses, 10, 10);
		
		if (draw_cost) {//цена комнаты 1 лвл
			_itoa_s(int(cost_rooms[room_type - 1][0][0]), text_cost_humans, 10, 10);
			_itoa_s(int(cost_rooms[room_type - 1][0][1]), text_cost_food, 10, 10);
			_itoa_s(int(cost_rooms[room_type - 1][0][2]), text_cost_resourses, 10, 10);
			draw_cost_yes = true;
		}
		if (draw_update_cost && choosed_room>0 && rooms[choosed_room - 1].type != 0 && rooms[choosed_room - 1].level != 3 && choosed_room != -1) {//цена апгрейда
			_itoa_s(int(cost_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level][0]), text_cost_humans, 10, 10);
			_itoa_s(int(cost_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level][1]), text_cost_food, 10, 10);
			_itoa_s(int(cost_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level][2]), text_cost_resourses, 10, 10);
			draw_cost_yes = true;
		}

		//Отрисовываем кадр
		Update(window,renderer, texts, background, room_selecting, alert,room_icons,rooms, 
			draw_cost=draw_cost_yes);

		//возвращаем флаги рисования цен назад
		draw_cost = false;
		draw_update_cost = false;
		draw_cost_yes = false;
		
		//Возвращаем нормальную текстуру фона, если никакая кнопка не нажата и фон изменялся
		if (was_background_changed == true) {
			background.texture = background_texture;
			was_background_changed = false;
		}

		//Работа с временем
		
			SDL_Delay(10);
			GAMETIME = time(NULL) - GAMESTARTTIME;
			int tick_time = SDL_GetTicks();
			DELTA = tick_time - LASTTICKTIME;
			LASTTICKTIME = tick_time;

			one_second -= DELTA * 0.001;
			if (one_second < 0) { //Каждую секунду
				Update_resourses(); //Пересчёт количества ресурсов
				if (GAME_OVER)
					return -1;
				Update_difficulty(); //Пересчёт сложности игры
				one_second = 1;
			}

			//Если время до прибытия поезда > 0, отнимаем из него прошедшее время за цикл
			if (TIMEUNTILTRAIN > 0) {
				TIMEUNTILTRAIN -= DELTA * 0.001;
				if (TIMEUNTILTRAIN < 5) { //Время до получения уведомления о поезде
					alert.active = true;
				}
			}
			else {
				TIMEUNTILTRAIN = get_new_train_time();
				alert.active = false;
			}

			//Если время до прибытия рейда > 0, отнимаем от него прошедшее время за цикл
			if (g_time_before_raid > 0) {
				g_time_before_raid -= DELTA * 0.001;
				if (g_time_before_raid < 5) {
					//TODO: Сделать сигнализацию о приближающемся рейде (звуки, текстуры)
				}
			}
			else { //Если наступило время рейда
				g_time_before_raid = get_new_battle_time();
				raid_started = true;
				//Отмена сигнализации
			}
		
		printf_s("time = %d, tut= %.1f, raid = %.1f\n", GAMETIME, TIMEUNTILTRAIN, g_time_before_raid);
	}

	SDL_DestroyTexture(background_texture);
	SDL_DestroyTexture(background_train_selected_texture);
	SDL_DestroyTexture(room_selecting_texture);
	SDL_DestroyTexture(alert_texture);
	SDL_DestroyTexture(room_update_texture);
	SDL_DestroyTexture(room_delete_texture);

	for (int i = 0; i < 19; i++) {
		SDL_DestroyTexture(room_textures[i]);
	}
	SDL_DestroyTexture(farm1_icon_texture);
	SDL_DestroyTexture(factory1_icon_texture);
	SDL_DestroyTexture(living1_icon_texture);
	SDL_DestroyTexture(weapony1_icon_texture);
	SDL_DestroyTexture(storage1_icon_texture);
	SDL_DestroyTexture(radio1_icon_texture);

	// Попытка красиво выгружать текстуры из памяти
	/*
	for (int i = 0; i < NUMBER_OF_TEXTURES; i++) {
		UnloadTexture(textures[i]);
	}
	*/

	//Так НУЖНО выходить из Mix_Init-а. Серьёзно. Сказано в Вики.
	while (Mix_Init(0))
		Mix_Quit();

	g_have_open_town_before = true; //Указываем, что все следующие запуски town-а будут НЕ первыми.

	if (raid_started) {
		raid_started = false;
		return 2;
	}

	if (go_to_train) {
		return 1;
	}
	else {
		return 0;
	}
}