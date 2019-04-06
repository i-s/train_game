#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "train.h"
#include "text.h"
#include "town.h"
#include "menu.h"
#include <stdlib.h>
#include <SDL_mixer.h>
#include <time.h>

//Объявление глобальных ресурсов:
//глобальные люди
int g_humans;
//глобальные материалы
int g_resourses;
//глобальная еда
int g_food;


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

//Рисует фон
void draw_background(SDL_Renderer* renderer, Background background) {
	SDL_RenderCopy(renderer, background.texture, NULL, &background.rectangle);
}

//Рисует обводку окна
void draw_room_selecting(SDL_Renderer* renderer, Room_Selecting room_selecting) {
	SDL_RenderCopy(renderer, room_selecting.texture, NULL, &room_selecting.rectangle);
}

void Update(SDL_Window* window,SDL_Renderer* renderer, char* texts[], Background background, Room_Selecting room_selecting) {
	draw_background(renderer, background);

	if (room_selecting.shown) {
		draw_room_selecting(renderer, room_selecting);
	}

	draw_text(window, renderer, texts[0], g_recthumans);
	draw_text(window, renderer, texts[1], g_rectfood);
	draw_text(window, renderer, texts[2], g_rectresourses);

	SDL_RenderPresent(renderer);
}

//Экран "город".
//Возврат 1 -> переход к экрану "поезд"
//Возврат 0 -> завершение программы
int town_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h) {
	difficulty = 1; //Сложность игры
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

	//Рисуем задний план
	SDL_Rect bground_rectangle = { 0,0,winsize_w,winsize_h };
	Background background = { background_texture, bground_rectangle };
	draw_background(renderer, background);

	//Выделение комнаты
	SDL_Rect room_selecting_rect = {0,0,231,150};
	Room_Selecting room_selecting;
	room_selecting.rectangle = room_selecting_rect;
	room_selecting.texture = room_selecting_texture;
	room_selecting.shown = false;

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
	SDL_Rect rect_button_train = {289,68,52,74};
	SDL_Rect rect_room_0 = {14,187,229,148};
	SDL_Rect rect_room_1 = { 287,187,229,148 };
	SDL_Rect rect_room_2 = { 555,187,229,148 };
	SDL_Rect rect_room_3 = { 14,342,229,148 };
	SDL_Rect rect_room_4 = { 287,342,229,148 };
	SDL_Rect rect_room_5 = { 555,342,229,148 };
	SDL_Rect buttons[7] = {rect_button_train,rect_room_0,rect_room_1,
		rect_room_2, rect_room_3,rect_room_4,rect_room_5 };

	int button_flag; //Перменная, хранящая номер нажатой кнопки

	//Подсчёт времени
	extern int LASTTICKTIME;
	extern int DELTA; //Разница во времени
	extern int GAMESTARTTIME;
	extern int GAMETIME; //Время со старта программы
	int time_of_last_click = GAMESTARTTIME; //Время предыдущего клика
	extern float TIMEUNTILTRAIN;//время до поезда

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
	
	//ГЛАВНЫЙ ЦИКЛ
	while (!quit && !go_to_train) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			quit = true;

		bool was_background_changed = false; //Был ли фон изменён?
		bool was_button_room_pressed = false; //Была ли нажата какая-либо комната?

		for (int i = 0; i <= 6; i++) {
			button_flag = CheckIfMouseOnButton(event, i, buttons);

			switch (button_flag) {
			case -1: break;
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
			default: printf_s("There is a problem with a buttons!\n"); break;
			}
		}
		
		//Если комната выделена, нарисовать рамку.
		if (choosed_room != -1 && was_button_room_pressed == true) {
			room_selecting.rectangle = buttons[choosed_room];
			room_selecting.shown = true;
		}

		//Подготавливаем текст для вывода
		_itoa_s(g_humans, text_humans, 10, 10);
		_itoa_s(g_food, text_food, 10, 10);
		_itoa_s(g_resourses, text_resourses, 10, 10);

		//Отрисовываем кадр
		Update(window,renderer, texts, background, room_selecting);



		
		//Возвращаем нормальную текстуру фона, если никакая кнопка не нажата и фон изменялся
		if (was_background_changed == true) {
			background.texture = background_texture;
			was_background_changed = false;
		}

		SDL_Delay(10);
		GAMETIME = time(NULL) - GAMESTARTTIME;
		int tick_time = SDL_GetTicks();
		DELTA = tick_time - LASTTICKTIME;

		//printf_s("DELTA = %.3f\n",DELTA*0.001);
		LASTTICKTIME = tick_time;

		//Если время до прибытия поезда > 0, отнимаем из него прошедшее время за цикл
		if (TIMEUNTILTRAIN > 0) {
			TIMEUNTILTRAIN -= DELTA * 0.001;
		}
		else {
			TIMEUNTILTRAIN = get_new_train_time();
		}
		printf_s("time = %d, tut= %.1f\n", GAMETIME, TIMEUNTILTRAIN);
	}

	//printf_s("hum = %d, res = %d, food = %d\n",g_humans,g_resourses,g_food);

	SDL_DestroyTexture(background_texture);
	SDL_DestroyTexture(background_train_selected_texture);
	SDL_DestroyTexture(room_selecting_texture);

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