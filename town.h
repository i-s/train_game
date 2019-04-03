#pragma once
//Экран "город".
//Возврат 1 -> переход к экрану "поезд"
//Возврат 0 -> завершение программы
int town_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h);

struct Background {
	SDL_Texture* texture;
	SDL_Rect rectangle;
};

//Рисует фон
void draw_background(SDL_Renderer* renderer, Background background);