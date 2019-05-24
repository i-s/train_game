#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "train.h"
#include "notifications.h"
#include "town.h"
#include "menu.h"
//#include "main.h"
//#include "textures.h"
#include <stdlib.h>
#include <SDL_mixer.h>
#include <time.h>
#include "rooms.h"
#define NUMBER_OF_TEXTURES 4
#define INCOME_FROM_FACTORY_1 0.2
#define INCOME_FROM_FACTORY_2 0.4
#define INCOME_FROM_FERM_1 1
//#define MAX_ROOM_LEVEL 3
//#define COUNT_ROOMS 6
//extern const int MAX_ROOM_LEVEL;
//extern const int COUNT_ROOMS;

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
extern float income_from_rooms[COUNT_ROOMS][MAX_ROOM_LEVEL][3];
extern int cost_rooms[COUNT_ROOMS][MAX_ROOM_LEVEL][3];
extern int change_cap_rooms[COUNT_ROOMS][MAX_ROOM_LEVEL][3];
extern Room_info room_info[6];

//Запускался ли экран "город" до этого момента
bool g_have_open_town_before = false;

//Громкость музыки и звука в игре. Должна принимать значения от 0 до 128.
int g_music_volume = 128;
int g_sound_volume = 128;

//Позиция фоновой музыки в игре
double g_music_position = 0;

//Глобальная переменная, отвечающая за сложность приезда поезда. Чем больше: тем хуже игроку
int difficulty;

//Объявление глобальных SDL_Rect для ресурсов:
SDL_Rect g_recthumans = { 66,2,197,44 };
SDL_Rect g_rectfood = { 66,47,197,51 };
SDL_Rect g_rectresourses = { 66,99,197,49 };
SDL_Rect rect_cost_humans = { 36,573,60,20 };
SDL_Rect rect_cost_food = { 116,573,60,20 };
SDL_Rect rect_cost_resourses = { 196,573,60,20 };

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

//работает с глобальной переменной g_rooms и ресурсами
//строим комнату:
//отнимаются ресурсы от глоб перем, и увел инком
//если комната построена вернет true;
bool build_room(Room *room, int room_type, int choosed_room)
{
	room->type = room_type;
	room->level = 1;
	if (choosed_room - 1 < 3) {
		g_rooms[0][choosed_room - 1][0] = room_type;
		g_rooms[0][choosed_room - 1][1] = 1;
	}
	else {
		g_rooms[1][choosed_room - 4][0] = room_type;
		g_rooms[1][choosed_room - 4][1] = 1;
	}
	//вычитаеем цену комнаты

	g_food -= cost_rooms[room_type-1][0][1];
	g_resourses -= cost_rooms[room_type-1][0][2];

	g_income_hum += income_from_rooms[room_type-1][0][0];
	g_income_food += income_from_rooms[room_type-1][0][1];
	g_income_res += income_from_rooms[room_type-1][0][2];
	//изменение ограничений после
	g_humans_cap += change_cap_rooms[room_type-1][0][0];
	g_food_cap += change_cap_rooms[room_type-1][0][1];
	g_resourses_cap += change_cap_rooms[room_type-1][0][2];
	return true;
}

//работает с глобальной переменной g_rooms
//улучшает выбранную комнату
//возвращает : 
// false - комната уже макс лвла
// true- если комната улучшена
bool upgrade_room(Room *room, int choosed_room)
{
	if (room->level == MAX_ROOM_LEVEL)
		return false;
	int room_type = room->type;
	//изменение инкома до апгрейта
	g_income_hum -= income_from_rooms[room->type-1][room->level - 1][0];
	g_income_food -= income_from_rooms[room->type - 1][room->level - 1][1];
	g_income_res -= income_from_rooms[room->type - 1][room->level - 1][2];
	//изменение ограничений до 
	g_humans_cap -= change_cap_rooms[room->type - 1][room->level - 1][0];
	g_food_cap -= change_cap_rooms[room->type - 1][room->level - 1][1];
	g_resourses_cap -= change_cap_rooms[room->type - 1][room->level - 1][2];

	room->level += 1;
	//вычитаем цену за апдейт
	g_food -= cost_rooms[room->type - 1][room->level - 1][1];
	g_resourses -= cost_rooms[room->type - 1][room->level - 1][2];

	//изменение инкома после апгрейта
	g_income_hum += income_from_rooms[room->type - 1][room->level - 1][0];
	g_income_food += income_from_rooms[room->type - 1][room->level - 1][1];
	g_income_res += income_from_rooms[room->type - 1][room->level - 1][2];
	//изменение ограничений после
	g_humans_cap += change_cap_rooms[room->type - 1][room->level - 1][0];
	g_food_cap += change_cap_rooms[room->type - 1][room->level - 1][1];
	g_resourses_cap += change_cap_rooms[room->type - 1][room->level - 1][2];


	if (choosed_room - 1 < 3) {
		g_rooms[0][choosed_room - 1][1] = room->level;
	}
	else {
		g_rooms[1][choosed_room - 4][1] = room->level;
	}
	return true;
}

//работает с глобальной переменной g_rooms и ресурсами
//уничтожает выбранную комнату
//возвращает : 
// true- если комната уничтожена
bool destroy_room(Room *room, int choosed_room, Room *rooms)
{
	//изменение инкома
	g_income_hum -= income_from_rooms[room->type - 1][room->level - 1][0];
	g_income_food -= income_from_rooms[room->type - 1][room->level - 1][1];
	g_income_res -= income_from_rooms[room->type - 1][room->level - 1][2];
	//изменение ограничений
	g_humans_cap -= change_cap_rooms[room->type - 1][room->level - 1][0];
	g_food_cap -= change_cap_rooms[room->type - 1][room->level - 1][1];
	g_resourses_cap -= change_cap_rooms[room->type - 1][room->level - 1][2];
	//возврат небольшого кол-ва ресурсов
	g_resourses += cost_rooms[room->type - 1][room->level - 1][2] * 0.25;

	rooms[choosed_room - 1].type = 0;
	rooms[choosed_room - 1].level = 0;

	if (choosed_room - 1 < 3) {
		g_rooms[0][choosed_room - 1][1] = 0;
		g_rooms[0][choosed_room - 1][0] = 0;
	}
	else {
		g_rooms[1][choosed_room - 4][1] = 0;
		g_rooms[1][choosed_room - 4][0] = 0;
	}
	return true;
}


int get_type_room(Room rooms[], int button_flag)
{
	return rooms[button_flag - 1].type;
}


int get_level_room(Room rooms[], int button_flag)
{
	return rooms[button_flag - 1].level;
}

//работает с глобальной переменной g_rooms и ресурсами
//возвращает:
// уровень уникального(оружейная , связной) здания , если оно построено 
// 0, если нет  
int get_building_level(int building_type)
{
	for (int i = 0; i < 2; i++)//обходим все комнаты
		for (int j = 0; j < 3; j++)
			if (g_rooms[i][j][0] == building_type)//если построено это здание
				return g_rooms[i][j][1]; // вернем уровень этого здания
	return 0;
}

//проверяем хватает ли ресурсов на комнату опред уровня(изнач 1)
//возвращает : 0 - хватает
// 1- нехватает людей
// 2- нехватает еды
// 3- нехватает ресурсов
int check_availability_res(int room_type, int room_level = 1)
{
	if (g_humans - cost_rooms[room_type - 1][room_level - 1][0] <= 0)
		return 1;
	if (g_food - cost_rooms[room_type - 1][room_level - 1][1] < 0)
		return 2;
	if (g_resourses - cost_rooms[room_type - 1][room_level - 1][2] < 0)
		return 3;
	return 0;
}

//проверяет склад на переполнение, принимает номер рес-а 
//если он забит ,возвращает 1, иначе -2
int check_full_stock(int res)
{
	if (res == 0 && g_humans >= g_humans_cap) {
		return 1;
	}
	else if (res == 1 && g_food >= g_food_cap) {
		return 1;
	}
	else if (res == 2 && g_resourses >= g_resourses_cap){
		return 1;
	}
	return -2;
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

//рисует окно инфо комнаты
void draw_rooms_info(SDL_Renderer* renderer, SDL_Texture* rooms_info[], int room_type, SDL_Rect rect) {
	rect.x += 67 *( room_type-1);//сдвиг места рисования
	SDL_RenderCopy(renderer, rooms_info[room_type-1], NULL, &rect);
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
void Update(SDL_Window* window, SDL_Renderer* renderer, char* texts[], Background background, Room_Selecting room_selecting,
	Alert alert, Room_icon room_icons[], SDL_Texture* rooms_info[], Room rooms[], SDL_Rect room_name_rect, SDL_Rect room_info_rect,
	SDL_Texture* resorses_textures[], SDL_Rect resorses_textures_rects[],
	int draw_room_name_level = -1, int draw_room_name_type = -1, bool draw_cost = false,
	int draw_nores = 0,int draw_room_info = -1) {
	draw_background(renderer, background);

	//Рисуем все комнаты
	for (int i = 0; i < 6; i++) 
		draw_room(renderer, rooms[i]);
	
	if (alert.active) 
		draw_alert(renderer, alert);
	//Рисуем рамку выделения комнаты
	if (room_selecting.shown) 
		draw_room_selecting(renderer, room_selecting);

	//Рисуем все иконки из массива сразу
	for (int i = 0; i <= 7; i++) 
		draw_room_icon(renderer, room_icons[i]);

	//рисуем имя выделенной комнаты
	if (draw_room_name_level != -1 && draw_room_name_type != -1)
	{
		draw_text(window, renderer, texts[6 + draw_room_name_type], room_name_rect, 0, true);
	}
	if (draw_room_info != -1)
	{
		draw_rooms_info(renderer, rooms_info, draw_room_info, room_info_rect);
	}

	//рисуем рессурсы
	draw_number_text(window, renderer, g_humans, g_recthumans, check_full_stock(0) + 2, true);
	draw_number_text(window, renderer, g_food, g_rectfood, check_full_stock(1) + 2, true);
	draw_number_text(window, renderer, g_resourses, g_rectresourses, check_full_stock(2) +2, true);
	//рисует кол-во рессурсов нужное для апгрейта/строительства комнаты
	if (draw_cost) {
		if(draw_nores)
			draw_text(window, renderer, texts[13 + draw_nores], room_name_rect, 0, true);
		for(int i=0; i<3;i++)
			draw_res(window,renderer, resorses_textures[i], resorses_textures_rects[i]);

		draw_text(window, renderer, texts[3], rect_cost_humans, 0,true);
		draw_text(window, renderer, texts[4], rect_cost_food, 0,true);
		draw_text(window, renderer, texts[5], rect_cost_resourses, 0,true);
	}
	SDL_RenderPresent(renderer);
}

//Экран "город".
//Возарат 2 -> переход к экрану "сражение"
//Возврат 1 -> переход к экрану "поезд"
//Возврат 0 -> завершение программы
int town_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h) {

	init_music();
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
	SDL_Surface* farm_info_surf = SDL_LoadBMP("resourses/textures/rooms/farm_info.bmp");
	SDL_Texture* farm_info_texture = SDL_CreateTextureFromSurface(renderer, farm_info_surf);
	SDL_FreeSurface(farm_info_surf);
	//Картиночки ресурсов
	SDL_Surface* people_surf = SDL_LoadBMP("resourses/textures/people.bmp");
	SDL_SetColorKey(people_surf, 1, SDL_MapRGB(people_surf->format, 0, 255, 0));
	SDL_Texture* people_texture = SDL_CreateTextureFromSurface(renderer, people_surf);
	SDL_FreeSurface(people_surf);
	SDL_Surface* food_surf = SDL_LoadBMP("resourses/textures/food.bmp");
	SDL_SetColorKey(food_surf, 1, SDL_MapRGB(food_surf->format, 0, 255, 0));
	SDL_Texture* food_texture = SDL_CreateTextureFromSurface(renderer, food_surf);
	SDL_FreeSurface(food_surf);
	SDL_Surface* res_surf = SDL_LoadBMP("resourses/textures/res.bmp");
	SDL_SetColorKey(res_surf, 1, SDL_MapRGB(res_surf->format, 0, 255, 0));
	SDL_Texture* res_texture = SDL_CreateTextureFromSurface(renderer, res_surf);
	SDL_FreeSurface(res_surf);
	//Массив картиночек ресурсов
	SDL_Texture* resorses_textures[3] = {people_texture,food_texture,res_texture};


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
	SDL_Surface* factory_info_surf = SDL_LoadBMP("resourses/textures/rooms/factory_info.bmp");
	SDL_Texture* factory_info_texture = SDL_CreateTextureFromSurface(renderer, factory_info_surf);
	SDL_FreeSurface(factory_info_surf);

	//иконки фабрики
	SDL_Surface* factory1_icon_surf = SDL_LoadBMP("resourses/textures/rooms/factory_1_icon.bmp");
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
	SDL_Surface* living_info_surf = SDL_LoadBMP("resourses/textures/rooms/living_info.bmp");
	SDL_Texture* living_info_texture = SDL_CreateTextureFromSurface(renderer, living_info_surf);
	SDL_FreeSurface(living_info_surf);

	//Иконка жилой комнаты
	SDL_Surface* living1_icon_surf = SDL_LoadBMP("resourses/textures/rooms/living_1_icon.bmp");
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
	SDL_Surface* weapony_info_surf = SDL_LoadBMP("resourses/textures/rooms/weapony_info.bmp");
	SDL_Texture* weapony_info_texture = SDL_CreateTextureFromSurface(renderer, weapony_info_surf);
	SDL_FreeSurface(weapony_info_surf);

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
	SDL_Surface* storage_info_surf = SDL_LoadBMP("resourses/textures/rooms/storage_info.bmp");
	SDL_Texture* storage_info_texture = SDL_CreateTextureFromSurface(renderer, storage_info_surf);
	SDL_FreeSurface(storage_info_surf);

	//Иконка склада
	SDL_Surface* storage1_icon_surf = SDL_LoadBMP("resourses/textures/rooms/storage_1_icon.bmp");
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
	SDL_Surface* radio_info_surf = SDL_LoadBMP("resourses/textures/rooms/radio_info.bmp");
	SDL_Texture* radio_info_texture = SDL_CreateTextureFromSurface(renderer, radio_info_surf);
	SDL_FreeSurface(radio_info_surf);

	//Иконка радио
	SDL_Surface* radio1_icon_surf = SDL_LoadBMP("resourses/textures/rooms/radio_1_icon.bmp");
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
	SDL_Rect room_selecting_name_rect = {574, 563, 165, 34};
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
	char* text_room_level = new char[10];

	char* farm_name = (char*)"Ферма";
	char* factory_name = (char*)"Мастерская";
	char* live_name = (char*)"Общага";
	char* wearony_name = (char*)"Оружейная";
	char* stock_name = (char*)"Склад";
	char* radio_name = (char*)"Радио";
	char* empty_name = (char*)"Пустая";

	char* text_nofood = (char*)"Мало еды";
	char* text_nopeople = (char*)"Мало людей";
	char* text_nores = (char*)"Мало рес-ов";

	//Массив строк для удобной передачи в ф-ию
	char* texts[17] = { text_humans, text_food, text_resourses, text_cost_humans, text_cost_food, text_cost_resourses,
		empty_name, farm_name, factory_name, live_name, wearony_name, stock_name, radio_name, text_room_level,
		text_nopeople, text_nofood, text_nores};

	//Rect-ы для иконок ресурсов возле иконок строительства
	SDL_Rect Near_icon_humans_rect = {16,568,30,30};
	SDL_Rect Near_icon_food_rect = {91,568,30,30};
	SDL_Rect Near_icon_res_rect = {173,568,30,30};

	SDL_Rect resorses_textures_rect[3] = {Near_icon_humans_rect,Near_icon_food_rect,Near_icon_res_rect};

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

	SDL_Rect rect_room_update = { 428,504,56,56 };
	SDL_Rect rect_room_delete = { 494,504,56,56 };

	SDL_Rect rect_room_info = { 30, 438, 325, 60 };

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

	//Массив комнат
	Room rooms[6];

	SDL_Texture* rooms_info[6] = { farm_info_texture, factory_info_texture, living_info_texture,
									weapony_info_texture, storage_info_texture, radio_info_texture};


	
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

	//Запуск фоновой музыки
	if (g_have_open_town_before == false) { //Если город запускается впервые, то
		play_music(0); //Сначала
	}
	else {
		play_music(0, g_music_position); //С определённого момента
	}

	bool was_alarm_called = false; //Флаг, отвечающий за единократное выведение предупреждения о надвигающемся рейде
	bool raid_started = false; //Флаг, отвечающий за начало рейда
	int go_to_train = false; //Флаг, отвечающий за переход к экрану "поезд"
	
	int choosed_room = -1; //Какая комната выбрана в данный момент
	int draw_room_info = -1;//для вывода инфо о комнта опре типа
	bool draw_cost = false; // флаг вывода цены комнаты 1 лвла
	int draw_room_name = -1; // какое имя комнаты печатать
	int draw_room_name_type = -1;
	int draw_room_name_level = -1;
	bool draw_update_cost = false; // флаг вывода цены комнаты 2-3 лвла
	int room_type = 0;//тип комнаты , для которой выводить цену
	bool draw_cost_yes = false;//рисовать ли цену комнаты сейчас
	int draw_nores = -1; //флаг нехватки какого-то ресурса для отрисовки его на экран

	int radio_level = get_building_level(6);
	
	//время последнего апдейта
	int time_last_update = time(NULL);
	
	//ГЛАВНЫЙ ЦИКЛ
	while (!quit && !go_to_train && !raid_started && !QUIT) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			QUIT = true;
			quit = true;
		}
		bool was_game_paused = false; //Была ли игра поставлена на паузу.
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
			Escape_menu(window,renderer,&event);
			was_game_paused = true;
		}

		bool was_background_changed = false; //Был ли фон изменён?
		bool was_button_room_pressed = false; //Была ли нажата какая-либо комната?
		bool was_rooms_changed = false; //Была ли хоть одна комната изменена?

		for (int i = 0; i <= 14; i++) {
			button_flag = CheckIfMouseOnButton(event, i, buttons);
			if (button_flag != -1)
				break;
		}

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
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6: {
				draw_room_name_type = get_type_room(rooms, button_flag);
				draw_room_name_level = get_level_room(rooms, button_flag);
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
						check_availability_res(room_type) == 0) {

						was_rooms_changed = build_room(&rooms[choosed_room - 1], room_type, choosed_room);
						Define_Rooms_Textures(&rooms[choosed_room - 1], room_textures);
					}
				}
				break;
			}
			case 8: { //мышка на кнопке фабрики
				draw_cost = true;
				room_type = 2;
				if (LKMPressed(event)) {//Нажатие на иконку фaбрики
					if (choosed_room > -1 && rooms[choosed_room - 1].type == 0 &&
						check_availability_res(room_type) == 0) {

						was_rooms_changed = build_room(&rooms[choosed_room - 1], room_type, choosed_room);
						Define_Rooms_Textures(&rooms[choosed_room - 1], room_textures);
					}
				}
				break;
			}
			case 9: { //мышка на кнопке жил комнат
				draw_cost = true;
				room_type = 3;
				if (LKMPressed(event)) {//Нажатие на иконку жилых комнат
					if (choosed_room > -1 && rooms[choosed_room - 1].type == 0 &&
						check_availability_res(room_type) == 0) {

						was_rooms_changed = build_room(&rooms[choosed_room - 1], room_type, choosed_room);
						Define_Rooms_Textures(&rooms[choosed_room - 1], room_textures);
					}
				}
				break;
			}
			case 10: { //мышка на кнопке оружйной
				draw_cost = true;
				room_type = 4;
				if (LKMPressed(event)) {//Нажатие на иконку оружейной
					if (choosed_room > -1 && rooms[choosed_room - 1].type == 0 &&
						check_availability_res(room_type) == 0) {

						if (get_building_level(4) > 0)// если уже построено
						{
							//TODO: Оповещение о постройке
							break;
						}
						was_rooms_changed = build_room(&rooms[choosed_room - 1], room_type, choosed_room);
						Define_Rooms_Textures(&rooms[choosed_room - 1], room_textures);
					}
				}
				break;
			}
			case 11: { //мышка на кнопке склада
				draw_cost = true;
				room_type = 5;
				if (LKMPressed(event)) {//Нажатие на иконку склада
					if (choosed_room > -1 && rooms[choosed_room - 1].type == 0 &&
						check_availability_res(room_type) == 0) {

						was_rooms_changed = build_room(&rooms[choosed_room - 1], room_type, choosed_room);
						Define_Rooms_Textures(&rooms[choosed_room - 1], room_textures);
					}
				}
				break;
			}
			case 12: { //мышка на кнопке радио
				draw_cost = true;
				room_type = 6;
				if (LKMPressed(event)) {//Нажатие на иконку радио
					if (choosed_room > -1 && rooms[choosed_room - 1].type == 0 &&
						check_availability_res(room_type) == 0) {

						if (get_building_level(6) > 0)// если уже построено
						{
							//TODO: Оповещение о постройке
							break;
						}

						was_rooms_changed = build_room(&rooms[choosed_room - 1], room_type, choosed_room);
						Define_Rooms_Textures(&rooms[choosed_room - 1], room_textures);
					}
				}
				break;
			}
			case 13:
			{
				room_type = rooms[choosed_room - 1].type;
				draw_update_cost = true;
				if (LKMPressed(event)) {//Нажатие на иконку апгрейда
					if (choosed_room > -1  //Команта выбрана
						&& rooms[choosed_room - 1].type != 0 //Комната построена
						&& time(NULL) - time_last_update + 0.5 > 1 //Не чаще раза в секунду
						&& check_availability_res(rooms[choosed_room - 1].type, rooms[choosed_room - 1].level + 1) == 0) {

						was_rooms_changed = upgrade_room(&rooms[choosed_room - 1], choosed_room);
						Define_Rooms_Textures(&rooms[choosed_room - 1], room_textures);
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

						was_rooms_changed = destroy_room(&rooms[choosed_room - 1], choosed_room, rooms);
						Define_Rooms_Textures(&rooms[choosed_room - 1], room_textures);
						time_last_update = time(NULL);
					}
				}
				break;
			}
			default: printf_s("There is a problem with a buttons!\n"); break;
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

		//Работа с временем		
		SDL_Delay(10);
		GAMETIME = time(NULL) - GAMESTARTTIME;
		int tick_time = SDL_GetTicks(); //Время с момента инициализации SDL (запуска программы)
		DELTA = tick_time - LASTTICKTIME;
		if (g_have_open_town_before == false) { //Если город запускается впервые, то не нужно изменять время.
			DELTA = 0;
		}
		if (was_game_paused) { //Если игра ставилась на паузу, то не нужно изменять время
			DELTA = 0;
		}
		LASTTICKTIME = tick_time;

		g_music_position += DELTA * 0.001; //Сдвигаем сохранённую позицию музыки на DELTA

		one_second -= DELTA * 0.001;
		if (g_have_open_town_before == false) { //Если город запускается впервые
			one_second = -1;
		}
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
			if (g_time_before_raid < 3.2 && was_alarm_called == false && get_building_level(4) >= 1) {
				play_sound(101);
				was_alarm_called = true;
			}
		}
		else { //Если наступило время рейда
			g_time_before_raid = get_new_battle_time();
			raid_started = true;
			//Отмена сигнализации
		}

		//Подготавливаем текст для вывода
		//_itoa_s(int(g_humans), text_humans, 10, 10);
		//_itoa_s(int(g_food), text_food, 10, 10);
		//_itoa_s(int(g_resourses), text_resourses, 10, 10);

		if (draw_cost) {//цена комнаты 1 лвл
			draw_room_info = room_type;//если просят цену покажеи и другую инфу о комнате
			_itoa_s(int(cost_rooms[room_type - 1][0][0]), text_cost_humans, 10, 10);
			_itoa_s(int(cost_rooms[room_type - 1][0][1]), text_cost_food, 10, 10);
			_itoa_s(int(cost_rooms[room_type - 1][0][2]), text_cost_resourses, 10, 10);
			draw_cost_yes = true;
			//если ресурсов не хватает
			draw_nores = check_availability_res(room_type);
		}
		if (draw_update_cost && choosed_room > 0 && rooms[choosed_room - 1].type != 0 && rooms[choosed_room - 1].level != 3 && choosed_room != -1) {//цена апгрейда
			_itoa_s(int(cost_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level][0]), text_cost_humans, 10, 10);
			_itoa_s(int(cost_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level][1]), text_cost_food, 10, 10);
			_itoa_s(int(cost_rooms[rooms[choosed_room - 1].type - 1][rooms[choosed_room - 1].level][2]), text_cost_resourses, 10, 10);
			draw_cost_yes = true;
			draw_nores = check_availability_res((room_type), rooms[choosed_room - 1].level + 1);
		}
		

		//Отрисовываем кадр
		Update(window,renderer, texts, background, room_selecting, alert,room_icons, rooms_info, rooms, room_selecting_name_rect,
			rect_room_info, resorses_textures,resorses_textures_rect, draw_room_name_level, draw_room_name_type, draw_cost=draw_cost_yes, draw_nores, draw_room_info);

		//возвращаем флаги назад
		draw_cost = false;
		draw_room_info = -1;
		draw_update_cost = false;
		draw_cost_yes = false;
		draw_room_name = -1;
		draw_room_name_type = -1;
		draw_room_name_level = -1;
		
		//Возвращаем нормальную текстуру фона, если никакая кнопка не нажата и фон изменялся
		if (was_background_changed == true) {
			background.texture = background_texture;
			was_background_changed = false;
		}
		
		printf_s("time = %d, tut= %.1f, raid = %.1f\n", GAMETIME, TIMEUNTILTRAIN, g_time_before_raid);
		g_have_open_town_before = true; //Указываем, что все следующие запуски town-а будут НЕ первыми.
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
	for (int i = 0; i < 6; i++) {
		SDL_DestroyTexture(rooms_info[i]);
	}
	SDL_DestroyTexture(farm1_icon_texture);
	SDL_DestroyTexture(factory1_icon_texture);
	SDL_DestroyTexture(living1_icon_texture);
	SDL_DestroyTexture(weapony1_icon_texture);
	SDL_DestroyTexture(storage1_icon_texture);
	SDL_DestroyTexture(radio1_icon_texture);
	SDL_DestroyTexture(food_texture);
	SDL_DestroyTexture(res_texture);
	SDL_DestroyTexture(people_texture);


	// Попытка красиво выгружать текстуры из памяти
	/*
	for (int i = 0; i < NUMBER_OF_TEXTURES; i++) {
		UnloadTexture(textures[i]);
	}
	*/

	Empty_Music();

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