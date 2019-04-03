#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <time.h>
#include "menu.h"
#include "town.h"
#include "text.h"
#include <math.h>
#include <stdlib.h>

/*To-do:
1) Баг с кнопками
2) Оповещение о прибытии поезда
3) Вывод количества ресурсов
*/

//Загружаем глобальные переменные из town.cpp
extern int g_humans, g_resourses, g_food;
extern int difficulty;

//Загружаем глобальные RECT-ы
extern SDL_Rect g_recthumans;
extern SDL_Rect g_rectfood;
extern SDL_Rect g_rectresourses;

//Загружаем глобальный шрифт
//extern TTF_Font* my_font;

//float x;float y;
struct Point {
	float x;
	float y;
};

//SDL_Rect rect;int switched;SDL_Texture* texture;SDL_Texture* texture_switched;
struct Lever {
	SDL_Rect rect;
	int switched;
	SDL_Texture* texture;
	SDL_Texture* texture_switched;
};

extern struct Background;

//SDL_Texture* texture;SDL_Rect rectangle;float angle;SDL_Point coord;SDL_Rect rectangle;float speed;bool shown;int type;bool reached_town;
struct Train {
	SDL_Texture* texture;
	SDL_Rect rectangle;
	float angle;
	Point coord;
	float speed;
	bool shown; //Нужно ли отображать\двигать поезд: 0 - нет, 1 - да
	int type; // Тип поезда: 0 - обычный, 1 - торговый, 2 - с людьми, 3 - рейдерский
	bool reached_town; //Достиг ли поезд города
	float time_before_arrive; //Время до прибытия
};

//SDL_Point point1;SDL_Point point2;
struct Line {
	Point point1;
	Point point2;
};

////Рисует фон
//void draw_background(SDL_Renderer* renderer, Background background) {
//	SDL_RenderCopy(renderer, background.texture, NULL, &background.rectangle);
//}

//Рисует поезд
void draw_train(SDL_Renderer* renderer, Train train) {
	SDL_RenderCopyEx(renderer, train.texture, NULL, &train.rectangle, train.angle, NULL, SDL_FLIP_NONE);
}

//Рисуем рычаг
void draw_lever(SDL_Renderer* renderer, Lever lever) {
	if (lever.switched)
		SDL_RenderCopy(renderer, lever.texture_switched, NULL, &lever.rect);
	else
		SDL_RenderCopy(renderer, lever.texture, NULL, &lever.rect);
}

//Считаем скорость
void CountSpeed(Line path, float speed, float* speed_x, float* speed_y, float* angle) {
	float sin_a, cos_a; // a - угол между скоростью и осью oY

	//Считаем синус и косинус угла
	sin_a = (fabsf(path.point2.x - path.point1.x)) / (sqrt((path.point2.x - path.point1.x)*(path.point2.x - path.point1.x) + (path.point2.y - path.point1.y)*(path.point2.y - path.point1.y)));
	cos_a = sqrt(1 - (sin_a * sin_a));
	*angle = asin(sin_a) * 180 / M_PI;

	//Выясняем, куда движется поезд. Если "znak" = 1, то в сторону увеличения этой координаты, если = -1, то в сторону уменьшения.
	int znak_x, znak_y;
	if (path.point1.x <= path.point2.x)
		znak_x = 1;
	else
		znak_x = -1;
	if (path.point1.y <= path.point2.y)
		znak_y = 1;
	else
		znak_y = -1;

	//Считаем проекции скорости
	*speed_x = speed * sin_a * znak_x;
	*speed_y = speed * cos_a * znak_y;
}

//Возвращает true, если поезд достиг конца пути
bool Reached_End(Line path, Point current_coord, float speed) {
	//Расстояние от текущей позиции до конца пути
	float rast = sqrt(((current_coord.x - path.point2.x) * (current_coord.x - path.point2.x)) + ((current_coord.y - path.point2.y) * (current_coord.y - path.point2.y)));
	if ((rast - speed) <= 0) {
		return true;
	}
	return false;
}

//Возвращает расстояние до конца пути
float Rast_to_End(Line path, Point current_coord, float speed) {
	//Расстояние от текущей позиции до конца пути
	float rast = sqrt(((current_coord.x - path.point2.x) * (current_coord.x - path.point2.x)) + ((current_coord.y - path.point2.y) * (current_coord.y - path.point2.y)));
	return rast - speed;
}

//Двигает поезд на speed_x и speed_y.
void Move_Train(float speed_x, float speed_y, Point* train_position) {
	train_position->x += speed_x;
	train_position->y += speed_y;
}

//Отрисовываем фон, поезд, рычаги.
void Update(SDL_Window* window,SDL_Renderer* renderer, Train train, Background background, Lever lever1, Lever lever2, char* texts[]) {
	draw_background(renderer, background);
	if(train.shown){ draw_train(renderer, train); }	//Не отрисовываем поезд, если он не должен быть виден
	draw_lever(renderer, lever1);
	draw_lever(renderer, lever2);

	draw_text(window,renderer,texts[0],g_recthumans);
	draw_text(window, renderer, texts[1], g_rectfood);
	draw_text(window, renderer, texts[2], g_rectresourses);

	SDL_RenderPresent(renderer);
}

//Переключает рычаг
void SwitchLever(Lever *lever) {
	if ((*lever).switched == 0) {
		(*lever).switched = 1;
	}
	else {
		(*lever).switched = 0;
	}
}

//Задаёт тип поезда в зависимости от сложности
void Define_Train_Type_And_Delay(Train* train, int difficulty, SDL_Texture* textures[]) {
	int chance_type_0 = 5; //Шанс прибытия обычного (тип 0) поезда
	int chance_type_1 = int(fmax(10 - difficulty, 2)); //Шанс прибытия торгового (тип 1) поезда
	int chance_type_2 = int(fmax(10 - difficulty, 1)); //Шанс прибытия пассажирского (тип 2) поезда
	int chance_type_3 = int(fmin(difficulty, 3)); //Шанс прибытия рейдерского (тип 3) поезда

	int luckynumber = rand() % (chance_type_0 + chance_type_1 + chance_type_2 + chance_type_3);

	if (luckynumber < chance_type_0) {
		(*train).type = 0;
	}
	else if (luckynumber < chance_type_0 + chance_type_1) {
		(*train).type = 1;
	}
	else if (luckynumber < chance_type_0 + chance_type_1 + chance_type_2) {
		(*train).type = 2;
	}
	else {
		(*train).type = 3;
	}
	(*train).texture = textures[train->type];

	(*train).time_before_arrive = 5 + rand() % 5;
	//printf_s("train_type = %d\n", (*train).type);
}

//Экран "поезд".
//Возврат -1 -> выход из программы с ошибкой (не нажата кнопка город)
//Возврат 0 -> была нажата кнопка город
int train_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h) {
	srand(int(time(NULL)));
		
	//Создаём ивент и переменную для отслеживания закрытия окна
	SDL_Event event;
	bool quit = false;

	//Загружаем текстуры
	//Фона
	SDL_Surface* background_surf = SDL_LoadBMP("resourses/textures/background.bmp");
	SDL_Texture* background_texture = SDL_CreateTextureFromSurface(renderer, background_surf);
	SDL_FreeSurface(background_surf);
	//Поезда
	SDL_Surface* train_type_0_surf = SDL_LoadBMP("resourses/textures/train_type_0.bmp");
	SDL_SetColorKey(train_type_0_surf, 1, SDL_MapRGB(train_type_0_surf->format, 0, 255, 0));
	SDL_Texture* train_type_0_texture = SDL_CreateTextureFromSurface(renderer, train_type_0_surf);
	SDL_FreeSurface(train_type_0_surf);
	SDL_Surface* train_type_1_surf = SDL_LoadBMP("resourses/textures/train_type_1.bmp");
	SDL_SetColorKey(train_type_1_surf, 1, SDL_MapRGB(train_type_1_surf->format, 0, 255, 0));
	SDL_Texture* train_type_1_texture = SDL_CreateTextureFromSurface(renderer, train_type_1_surf);
	SDL_FreeSurface(train_type_1_surf);
	SDL_Surface* train_type_2_surf = SDL_LoadBMP("resourses/textures/train_type_2.bmp");
	SDL_SetColorKey(train_type_2_surf, 1, SDL_MapRGB(train_type_2_surf->format, 0, 255, 0));
	SDL_Texture* train_type_2_texture = SDL_CreateTextureFromSurface(renderer, train_type_2_surf);
	SDL_FreeSurface(train_type_2_surf);
	SDL_Surface* train_type_3_surf = SDL_LoadBMP("resourses/textures/train_type_3.bmp");
	SDL_SetColorKey(train_type_3_surf, 1, SDL_MapRGB(train_type_3_surf->format, 0, 255, 0));
	SDL_Texture* train_type_3_texture = SDL_CreateTextureFromSurface(renderer, train_type_3_surf);
	SDL_FreeSurface(train_type_3_surf);

	SDL_Texture* train_textures[4] = {train_type_0_texture,train_type_1_texture ,train_type_2_texture ,train_type_3_texture };
	//Рычагов
	SDL_Surface* lever_surf = SDL_LoadBMP("resourses/textures/lever.bmp");
	SDL_SetColorKey(lever_surf, 1, SDL_MapRGB(lever_surf->format, 0, 255, 0));
	SDL_Texture* lever_texture = SDL_CreateTextureFromSurface(renderer, lever_surf);
	SDL_FreeSurface(lever_surf);
	//Нажатых рычагов
	SDL_Surface* lever_switched_surf = SDL_LoadBMP("resourses/textures/lever_switched.bmp");
	SDL_SetColorKey(lever_switched_surf, 1, SDL_MapRGB(lever_switched_surf->format, 0, 255, 0));
	SDL_Texture* lever_switched_texture = SDL_CreateTextureFromSurface(renderer, lever_switched_surf);
	SDL_FreeSurface(lever_switched_surf);

	//Рисуем задний план
	SDL_Rect bground_rectangle = { 0,0,winsize_w,winsize_h };
	Background background = { background_texture, bground_rectangle };
	draw_background(renderer, background);

	//Рисуем рычаги
	Lever lever1 = { { 380,60, 40,50 },0,lever_switched_texture,lever_texture };
	Lever lever2 = { { 305,370,40,50 },0,lever_texture,lever_switched_texture };
	draw_lever(renderer, lever1);
	draw_lever(renderer, lever2);

	//Кнопка города
	SDL_Rect town = {444,196,264,215};

	//Делаем рычаги кнопками
	SDL_Rect buttons[3];
	//Заполняем массив понятным способом
	buttons[0] = lever1.rect;
	buttons[1] = lever2.rect;
	buttons[2] = town;

	//Презентуем рендерер
	SDL_RenderPresent(renderer);

	//Задаём пути
	Line line0 = { {345,-100},{345,-80} }; //Участок пути за экраном

	Line line1 = { {345,-80},{345,25} }; // Первый участок пути (до первого поворота)

	Line line2 = { {345,25},{315,100} }; //Первый поворот
	Line line3 = { {315,100},{-80,225} };

	Line line4 = { {345,25},{345,345} }; //Второй участок пути (до второго поворота)

	Line line5 = { {345,345},{400,440} }; //Второй поворот
	Line line6 = { {400,440}, {560, 440} };
	Line line10 = { {560,440}, {580, 420} };
	Line line7 = { {580,420},{580,300} };

	Line line8 = { {345,345},{345,600} }; //Третий участок пути (до конца экрана)

	Line line9 = { {345,600},{345,700} }; //Участок пути за эрканом

	Line main_path[11] = { line0, line1,line2,line3,line4,line5,line6,line7,line8,line9,line10 };

	//Задаём поезду начальные значения
	//SDL_Point train_position = { 0,0 };


	int start_position = 1; //ПЕРЕМЕННАЯ, ОТВЕЧАЮЩАЯ ЗА МЕСТО ПОЯВЛЕНИЯ ПОЕЗДА


	SDL_Rect train_rectangle = { main_path[start_position].point1.x, main_path[start_position].point1.y,20,80 };
	Train train;
	train.rectangle = train_rectangle;
	train.speed = 1.5;
	train.shown = true;
	train.reached_town = false;
	Define_Train_Type_And_Delay(&train,difficulty,train_textures);

	//Подсчёт времени
	int last_tick_time = 0;
	int delta = 0; //Разница во времени

	//printf_s("speed = "); scanf_s("%f", &train.speed);

	bool have_counted_speed = false; //Флаг, определяющий, была ли уже посчитана скорость
	int flag_line = 0; //Флаг, который равен 1, когда мы дошли до конца пути.
	int current_path = start_position; //Хранит номер текущего пути.

	//Начальное положение поезда:
	train.coord.x = main_path[current_path].point1.x;
	train.coord.y = main_path[current_path].point1.y;

	//Время запуска программы
	int time_of_start = int(time(NULL));
	int time_from_start; //Время со старта программы
	int time_of_last_click=time_of_start; //Время предыдущего клика

	float speed_x, speed_y; //Скорость по x и y для поезда
	float rast; //Расстояние до конечной точки маршрута

	bool town_button_pressed = false; //Нажата ли кнопка "город"?

	//Строки для хранения числовых значений людей, еды и ресурсов.
	char* text_humans = new char[10];
	char* text_food = new char[10];
	char* text_resourses = new char[10];

	//Массив строк для удобной передачи в ф-ию
	char* texts[3] = {text_humans, text_food, text_resourses};

	//Обработка ивентов
	while (!quit) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			quit = true;

		int time_of_one_cycle = 0;

		//Каждый раз в цикле говорим, что мышь не наведена ни на одну из кнопок
		int button_flag = -1;

		if ((time(NULL) - time_of_last_click >= 1)&&(LKMPressed(event))) { //Кликать не чаще 1 раза в секунду
			for (int i = 0; i <= 2; i++) {
				button_flag = CheckIfMouseOnButton(event, i, buttons);
				if (button_flag == 0) {
					SwitchLever(&lever1);
					time_of_last_click = time(NULL); //ПРОБЛЕМА С НАЖАТИЯМИ (ПОВТОРНОЕ ПЕРЕКЛЮЧЕНИЕ)
				}
				else if (button_flag == 1) {
					SwitchLever(&lever2);
					time_of_last_click = time(NULL);
				}
				else if (button_flag == 2) {
					town_button_pressed = true;
				}
			}
		}

		if (train.shown && train.time_before_arrive <= 0) {

			//Проверяем, нужно ли перейти к новому пути и переходим, если нужно
			if (flag_line) {
				have_counted_speed = false;
				if (current_path == 1) { //Если на первом участке пути
					if (lever1.switched) { //Если первый рычаг нажат
						current_path = 2; //Поворачиваем на первый поворот
					}
					else {
						current_path = 4; //Продолжаем движение вниз
					}
				}

				else if (current_path == 2) { //Если мы поворачиваем
					current_path = 3; //Продолжаем поворачивать
				}

				else if (current_path == 3) { //Если дошли до конца поворота
					train.shown = 0; //Скрываем поезд
				}

				else if (current_path == 4) { //Если на втором участке пути
					if (lever2.switched) { //Если второй рычаг нажат
						current_path = 5; //Поворачиваем на второй поворот
					}
					else {
						current_path = 8; //Продолжаем движение вниз
					}
				}

				else if (current_path == 5) { //Если мы поворачиваем
					current_path = 6; //Продолжаем поворачивать
				}
				else if (current_path == 6) { //Если мы поворачиваем
					current_path = 10; //Продолжаем поворачивать
				}
				else if (current_path == 10) { //Если мы поворачиваем
					current_path = 7; //Продолжаем поворачивать
				}
				else if (current_path == 7) { //Если дошли до конца поворота
					train.reached_town = true;
					train.shown = 0; //Скрываем поезд
				}

				else if (current_path == 8) {//Если дошли до конца пути
					current_path = 9;
					train.shown = 0; //Скрываем поезд
				}

				train.coord.x = main_path[current_path].point1.x;
				train.coord.y = main_path[current_path].point1.y;
			}

			if (!have_counted_speed) {
				CountSpeed(main_path[current_path], train.speed, &speed_x, &speed_y, &train.angle);
				if (speed_x > 0) {
					if (speed_y >= 0) {
						train.angle = 360 - train.angle;
					}
				}
				if (speed_y < 0) {
					train.angle = train.angle + 180;
				}
				have_counted_speed = true;
			}

			//if (Reached_End(main_path[current_path], train.coord, train.speed)) {
			rast = Rast_to_End(main_path[current_path], train.coord, train.speed);
			if (rast <= 1) {
				train.coord = main_path[current_path].point2;
				flag_line = 1;
			}
			else {
				Move_Train(speed_x, speed_y, &train.coord);
				train.rectangle.x = train.coord.x;
				train.rectangle.y = train.coord.y;
				flag_line = 0;
			}
		}
		
		if (train.reached_town) {
			switch (train.type) {
			case 0: { //Если прибыл обычный поезд, то...
				g_food += -10 + rand() % 50;
				g_resourses += -10 + rand() % 50;
				break;
			}
			case 1: {//Если прибыл торговый поезд, то...
				g_food += 10 + rand() % 100;
				g_resourses += 10 + rand() % 100;
				break;
			}
			case 2: {//Если прибыл пассажирский поезд, то...
				g_humans += -1 + rand() % 10;
				g_food += -50 + rand() % 60;
				g_resourses += -10 + rand() % 20;
				break;
			}
			case 3: {//Если прибыл рейдерский поезд, то...
				g_humans += -20 + rand() % 20;
				g_food += -50 + rand() % 50;
				g_resourses += -100 + rand() % 100;
				break;
			}
			default: break;
			}
			train.reached_town = false;
		}

		//Подготовка к выводу цифр на экран
		_itoa_s(g_humans, text_humans, 10, 10);
		_itoa_s(g_food, text_food, 10, 10);
		_itoa_s(g_resourses, text_resourses, 10, 10);

		Update(window, renderer, train, background, lever1, lever2, texts);

		time_from_start = time(NULL) - time_of_start;
		
		//Вывод дебаг-информации
		printf_s("time = %d : x = %.0f, y = %.0f, way = %d, time_arrive = %.1f\n", time_from_start, train.coord.x, train.coord.y, current_path, train.time_before_arrive);

		SDL_Delay(10);

		//ТЕСТОВЫЙ ПЕРЕЗАПУСК ПОЕЗДА
		/*
		if (!train.shown) {
			train.shown = true;
			current_path = start_position;
			have_counted_speed = false;
			train.coord.x = main_path[current_path].point1.x;
			train.coord.y = main_path[current_path].point1.y;
			Define_Train_Type_And_Delay(&train,++DIFFICULTY,train_textures);
		}
		*/
		
		//Считаем время
		int tick_time = SDL_GetTicks();
		delta = tick_time - last_tick_time;
		//printf_s("delta = %.3f\n",delta*0.001);
		last_tick_time = tick_time;

		//Если время до прибытия поезда > 0, отнимаем из него прошедшее время за цикл
		if (train.time_before_arrive > 0) {
			train.time_before_arrive -= delta*0.001;
		}
		
		//Выходим из цикла при нажатии на город
		if (town_button_pressed) {
			break;
		}

		//Ограничиваем ФПС.
		/*if (delta < max_tick_time)
			SDL_Delay(max_tick_time - delta);
		*/
	}

	SDL_DestroyTexture(background_texture);
	SDL_DestroyTexture(train_type_0_texture);
	SDL_DestroyTexture(train_type_1_texture);
	SDL_DestroyTexture(train_type_2_texture);
	SDL_DestroyTexture(train_type_3_texture);
	SDL_DestroyTexture(lever_texture);
	SDL_DestroyTexture(lever_switched_texture);

	if (town_button_pressed) {
		return 0;
	}
	else {
		return -1;
	}
}