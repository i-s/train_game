#include <SDL.h>
#include <stdio.h>

//Рисует фон
void draw_background(SDL_Renderer* renderer, SDL_Texture* bground_texture, SDL_Rect rectangle) {
	SDL_RenderCopy(renderer, bground_texture, NULL, &rectangle);
}

//Рисует поезд
void draw_train(SDL_Renderer* renderer, SDL_Texture* train_texture, SDL_Rect rectangle) {
	SDL_RenderCopy(renderer, train_texture, NULL, &rectangle);
}

//Двигает поезд
void train_move(float train_speed, int *train_coord, int* clock) {
	if (int(train_speed) >= 1) { //Если скорость больше 1,..
		*train_coord += int(train_speed); //...просто передвигаем поезд на столько-то.
		*clock = 0;//Сбрасываем клок для избежания его роста. (Будущее использование?)
	}
	else if ((train_speed >= 0) && (*clock / (1 / train_speed) >= 1)) { //Если скорость меньше 1, ждём, пока клок будет делиться на 1/скорость и...
		*train_coord += 1;//...двигаем поезд на 1.
		*clock = 0;//Незабываем сбрасывать клок.
	}
}

void game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h) {

	//Создаём ивент и переменную для отслеживания закрытия окна
	SDL_Event event;
	bool quit = false;

	//Загружаем текстуры
	//Фона
	SDL_Surface* background_surf = SDL_LoadBMP("textures/background.bmp");
	SDL_Texture* background_texture = SDL_CreateTextureFromSurface(renderer, background_surf);
	SDL_FreeSurface(background_surf);
	//Поезда
	SDL_Surface* train_surf = SDL_LoadBMP("textures/train.bmp");
	SDL_SetColorKey(train_surf, 1, SDL_MapRGB(train_surf->format, 0, 255, 0));
	SDL_Texture* train_texture = SDL_CreateTextureFromSurface(renderer, train_surf);
	SDL_FreeSurface(train_surf);

	//Рисуем задний план
	SDL_Rect bground_rectangle = { 0,0,winsize_w,winsize_h };
	draw_background(renderer, background_texture, bground_rectangle);

	//Рисуем поезд
	SDL_Point train_start_position = { 345,-160 };
	SDL_Rect train_rectangle = { train_start_position.x,train_start_position.y,20,80 };
	draw_train(renderer, train_texture, train_rectangle);

	//Презентуем рендерер
	SDL_RenderPresent(renderer);

	//Подсчёт времени
	const int FPS = 60;
	const int max_tick_time = 1000 / FPS;
	int last_tick_time = 0;
	int delta = 0; //Разница во времени
	int clock = 0; //Клок для работы со скоростью

	//Двигаем поезд
	float train_speed;
	printf_s("speed = "); scanf_s("%f", &train_speed);

	//Обработка ивентов
	while (!quit) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			quit = true;

		//Считаем время
		int tick_time = SDL_GetTicks();
		delta = tick_time - last_tick_time;
		printf_s("sec = %.3f y=%d\n", float(delta) / 1000, train_start_position.y); //Выводим дебаг-информацию
		last_tick_time = tick_time;

		//Всё ниже - UPDATE
		if (train_start_position.y < winsize_h) {
			//Двигаем поезд
			train_move(train_speed, &train_start_position.y, &clock);
			train_rectangle = { train_start_position.x,train_start_position.y,20,80 };
			//Перерисовываем задний фон и поезд
			draw_background(renderer, background_texture, bground_rectangle);
			draw_train(renderer, train_texture, train_rectangle);
			//Представляем рендерер
			SDL_RenderPresent(renderer);
			//Увеличиваем clock для работы со скоростью
			clock++;
		}
		else {
			quit = true;
		}

		//Ограничиваем ФПС.
		if (delta < max_tick_time)
			SDL_Delay(max_tick_time - delta);
	}

	SDL_DestroyTexture(background_texture);
	SDL_DestroyTexture(train_texture);
}