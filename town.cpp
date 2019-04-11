#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "train.h"
#include "text.h"
#include "town.h"
#include "menu.h"
//#include "textures.h"
#include <stdlib.h>
#include <SDL_mixer.h>
#include <time.h>
#define NUMBER_OF_TEXTURES 4

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
// улучшения комнат
int g_rooms[2][3][2]; //2 ряда по 3 колонки по 2 числа в ячейке: тип и уровень

//Загрузка глобальных ресурсов
// подсчёт времени
extern int LASTTICKTIME;
extern int DELTA; //Разница во времени
extern int GAMESTARTTIME;
extern int GAMETIME; //Время со старта программы
extern float TIMEUNTILTRAIN;//время до поезда

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

extern struct Background;

//SDL_Texture* texture;SDL_Rect rectangle;bool shown;
struct Room_Selecting {
	SDL_Texture* texture;
	SDL_Rect rectangle;
	bool shown;
};

//SDL_Texture* texture;SDL_Rect rectangle;bool active;
struct Alert {
	SDL_Texture* texture;
	SDL_Rect rectangle;
	bool active;
};

struct Room_icon {
	SDL_Texture* texture;
	SDL_Rect rectangle;
};

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

void Define_Rooms_Textures(Room* room, SDL_Texture* room_textures[]) {
	switch ((*room).type) {
	case 0: (*room).texture = room_textures[0]; break;
	case 1: { //Ферма
		switch ((*room).level) { //Уровни фермы
		case 1: (*room).texture = room_textures[1]; break;
		case 2: break;
		case 3: break;
		}
		break; }
	case 2: break;
	case 3: break;
	case 4: break;
	case 5: break;
	default: break;
	}
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
	g_resourses -= delta_resourses;
	
	delta_humans = 0;

	if (g_food < 0) { //Если еды меньше 0...
		delta_humans = fminf(1,-1 * g_food/10); //уменьшаем количество людей на
		/*
		            количество отсутствующей еды
		min(1, -1 * ----------------------------)
		                        10
		*/
		g_food = 0;
	}

	g_humans -= delta_humans;
}

//Изменяет сложность игры по формулам
void Update_difficulty() {
	difficulty = int(g_resourses / 25) + int(g_humans / 50) + 1; //Пересчёт сложности игры
}

//Отрисовывает все изображения на экран
void Update(SDL_Window* window,SDL_Renderer* renderer, char* texts[], Background background, Room_Selecting room_selecting, Alert alert, Room_icon room_icons[], Room rooms[]) {
	draw_background(renderer, background);

	for (int i = 0; i < 6; i++) {
		draw_room(renderer, rooms[i]);
	}
	//draw_room(renderer, rooms[0]); //ВРЕМЕННО

	if (alert.active) {
		draw_alert(renderer, alert);
	}

	if (room_selecting.shown) {
		draw_room_selecting(renderer, room_selecting);
	}

	draw_room_icon(renderer, room_icons[0]); //ВРЕМЕННО

	draw_text(window, renderer, texts[0], g_recthumans);
	draw_text(window, renderer, texts[1], g_rectfood);
	draw_text(window, renderer, texts[2], g_rectresourses);

	SDL_RenderPresent(renderer);
}

//Экран "город".
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
	//иконки фермы
	SDL_Surface* farm1_icon_surf = SDL_LoadBMP("resourses/textures/rooms/farm_1_icon.bmp");
	SDL_Texture* farm1_icon_texture = SDL_CreateTextureFromSurface(renderer, farm1_icon_surf);
	SDL_FreeSurface(farm1_icon_surf);

	//Массив текстур комнат
	SDL_Texture* room_textures[19] = {stock_texture,farm1_texture};

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

	//Массив строк для удобной передачи в ф-ию
	char* texts[3] = { text_humans, text_food, text_resourses };

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

	SDL_Rect buttons[13] = { rect_button_train,rect_room_0,rect_room_1,
		rect_room_2, rect_room_3,rect_room_4,rect_room_5,
		rect_room_icon_0,rect_room_icon_1,rect_room_icon_2,rect_room_icon_3,rect_room_icon_4,rect_room_icon_5
	};
	//Создаём иконки
	Room_icon room_icons[6];
	room_icons[0].rectangle = rect_room_icon_0;
	room_icons[0].texture = farm1_icon_texture;
	//Создаём комнаты
	Room rooms[6];

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

	int go_to_train = false; //Флаг, отвечающий за переход к экрану "поезд"

	int choosed_room = -1; //Какая комната выбрана в данный момент

	//int time_of_last_click = GAMESTARTTIME; //Время предыдущего клика
	
	//ГЛАВНЫЙ ЦИКЛ
	while (!quit && !go_to_train) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			quit = true;

		bool was_background_changed = false; //Был ли фон изменён?
		bool was_button_room_pressed = false; //Была ли нажата какая-либо комната?
		bool was_rooms_changed = false; //Была ли хоть одна комната изменена?

		for (int i = 0; i <= 7; i++) {
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
			case 7: { //Нажатие на иконку фермы
				if (LKMPressed(event)) {
					if (choosed_room > -1) {
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
						was_rooms_changed = true;
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

		//Отрисовываем кадр
		Update(window,renderer, texts, background, room_selecting, alert,room_icons,rooms);
		
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

		one_second -= DELTA*0.001;
		if (one_second < 0) { //Каждую секунду
			Update_resourses(); //Пересчёт количества ресурсов
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
		printf_s("time = %d, tut= %.1f\n", GAMETIME, TIMEUNTILTRAIN);
	}

	SDL_DestroyTexture(background_texture);
	SDL_DestroyTexture(background_train_selected_texture);
	SDL_DestroyTexture(room_selecting_texture);
	SDL_DestroyTexture(alert_texture);
	SDL_DestroyTexture(farm1_icon_texture);
	SDL_DestroyTexture(farm1_texture);
	

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

	if (go_to_train) {
		return 1;
	}
	else {
		return 0;
	}
}