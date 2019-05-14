#pragma once


//Экран "сражение". Если передать custom_difficulty != -1 ,то для ЭТОГО сражения будет использована специальная сложность
//Возврат 0 -> окно закрыто
int battle_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h, int custom_difficulty = -1);



//Ситруктура "Враг" SDL_Texture* texture;SDL_Rect rectangle;
//int type; //Тип врага: 0 - обычный зомби 
//bool active = false; Активен ли враг (нужно ли его отрисовывать, приближать?)
//int spawn_point;
//int hp; здоровье
//Потери ресурсов в порядке: еда, ресурсы, люди
//int loss_food;
//int loss_resourses;
//int loss_humans;
struct Enemy {
	SDL_Texture* texture;
	SDL_Rect rectangle;
	int type; //Тип врага: 0 - обычный зомби
	bool active = false; //Активен ли враг (нужно ли его отрисовывать, приближать?)
	int spawn_point;
	int hp; //Жизни врага
	//Потери ресурсов в порядке: еда, ресурсы, люди
	int loss_food;
	int loss_resourses;
	int loss_humans;
};

struct SpawnPoint {
	SDL_Point point;
	bool open = true; //Свободно ли место для спавна
};

struct Gun {
	int damage;
	float cooldown;
	SDL_Rect cooldown_rect;
	SDL_Rect button_rect;
	float time_end_reload = 100;// просто лишнее значение
	float cost;
	bool active = true;
	int sound_number;
	//звук
};