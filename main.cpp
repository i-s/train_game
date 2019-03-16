#include <SDL.h>
#include <stdio.h>
#undef main

int winsize_w = 800, winsize_h = 600;

void draw_background(SDL_Renderer* renderer,SDL_Texture* bground_texture, SDL_Rect rectangle) {
	//SDL_Surface* background_surf = SDL_LoadBMP("background.bmp");
	//SDL_Texture* background_texture = SDL_CreateTextureFromSurface(renderer, background_surf);
	//SDL_FreeSurface(background_surf);
	SDL_RenderCopy(renderer, bground_texture, NULL, &rectangle);
	//SDL_DestroyTexture(bground_texture);
}

void draw_train(SDL_Renderer* renderer,SDL_Texture* train_texture ,SDL_Rect rectangle) {
	//SDL_Surface* train_surf = SDL_LoadBMP("train.bmp");
	//SDL_SetColorKey(train_surf,1,SDL_MapRGB(train_surf->format,0,255,0));
	//SDL_Texture* train_texture = SDL_CreateTextureFromSurface(renderer, train_surf);
	//SDL_FreeSurface(train_surf);
	SDL_RenderCopy(renderer, train_texture, NULL, &rectangle);
	//SDL_DestroyTexture(train_texture);
}

void train_move(float train_speed,int *train_coord, int* clock) {
	if (train_speed > 1) {
		*train_coord += int(train_speed);
		*clock = 0;
	}
	else if ((train_speed >= 0) && (*clock / (1/train_speed) >= 1)) {
		*train_coord += 1;
		*clock = 0;
	}
}

int main() {
	//Создаём окно
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Main window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		winsize_w, winsize_h, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	//Создаём 2 surface-а: один - для заднего плана, второй - для переднего.

	//Загружаем текстуры
	//Фона
	SDL_Surface* background_surf = SDL_LoadBMP("background.bmp");
	SDL_Texture* background_texture = SDL_CreateTextureFromSurface(renderer, background_surf);
	SDL_FreeSurface(background_surf);
	//Поезда
	SDL_Surface* train_surf = SDL_LoadBMP("train.bmp");
	SDL_SetColorKey(train_surf, 1, SDL_MapRGB(train_surf->format, 0, 255, 0));
	SDL_Texture* train_texture = SDL_CreateTextureFromSurface(renderer, train_surf);
	SDL_FreeSurface(train_surf);

	//Рисуем задний план
	SDL_Rect bground_rectangle = {0,0,winsize_w,winsize_h};
	//draw_background(renderer, bground_rectangle);
	draw_background(renderer, background_texture, bground_rectangle);

	//Рисуем поезд
	SDL_Point train_start_position = {345,-160};
	//SDL_Point train_current_position = train_start_position;
	SDL_Rect train_rectangle = {train_start_position.x,train_start_position.y,20,80};
	draw_train(renderer,train_texture,train_rectangle);

	//Презентуем рендерер
	SDL_RenderPresent(renderer);

	//Подсчёт времени
	const int FPS = 60;
	const int max_tick_time = 1000 / FPS;
	int last_tick_time = 0;
	int delta = 0;
	int clock = 0;
	
	//Двигаем поезд
	float train_speed = 2;
	while (train_start_position.y < winsize_h) {
		int tick_time = SDL_GetTicks();
		delta = tick_time - last_tick_time;
		printf_s("sec = %.3f y=%d\n",float(delta)/1000, train_start_position.y);
		last_tick_time = tick_time;
		//train_start_position.y+= train_speed;
		train_move(train_speed, &train_start_position.y, &clock);
		train_rectangle = { train_start_position.x,train_start_position.y,20,80 };
		draw_background(renderer, background_texture, bground_rectangle);
		draw_train(renderer, train_texture, train_rectangle);
		SDL_RenderPresent(renderer);
		if(delta<max_tick_time)
			SDL_Delay(max_tick_time-delta);
		clock++;
	}

	//Высвобождаем память
	SDL_DestroyTexture(background_texture);
	SDL_DestroyTexture(train_texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}