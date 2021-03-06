#pragma once


//float x;float y;
struct Point {
	float x;
	float y;
};

//SDL_Texture* texture;SDL_Rect rectangle;bool shown;
struct Town_block {
	SDL_Texture* texture;
	SDL_Rect rectangle;
	bool shown;
};

//SDL_Rect rect;int switched;SDL_Texture* texture;SDL_Texture* texture_switched;
struct Lever {
	SDL_Rect rect;
	int switched;
	SDL_Texture* texture;
	SDL_Texture* texture_switched;
};

extern struct Background;

struct Text_box {
	SDL_Texture* texture;
	SDL_Rect rectangle;
};

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

//Экран "поезд".
//Возврат -1 -> выход из программы с ошибкой (не нажата кнопка город)
//Возврат 0 -> была нажата кнопка город
int train_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h);


int get_new_train_time();

