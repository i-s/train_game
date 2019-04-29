#pragma once
//Экран "город".
//Возарат 2 -> переход к экрану "сражение"
//Возврат 1 -> переход к экрану "поезд"
//Возврат 0 -> завершение программы
int town_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h);

//Структура заднего фона
//SDL_Texture* texture;SDL_Rect rectangle;
struct Background {
	SDL_Texture* texture;
	SDL_Rect rectangle;
};

//SDL_Texture* texture;SDL_Rect rectangle;int level;int type;int min_humans;int cost_humans;int cost_humans;
//min_humans - мин кол-во людей для постройки
//cost_foods - сколько еды стоит
//cost_resourses - сколько ресурсов стоит
struct Room
{
	SDL_Texture* texture;
	SDL_Rect rectangle;
	int level;
	int type;
	int min_humans;
	int cost_foods;
	int cost_resourses;
};


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


//char path[100];char name[50];SDL_Texture* texture;bool chroma_key;
struct Texture {
	char name[50];
	SDL_Texture* texture;
	bool chroma_key;
};

//Рисует фон
void draw_background(SDL_Renderer* renderer, Background background);

//Изменяет количество ресурсов по формулам
void Update_resourses();

//Изменяет сложность игры по формулам
void Update_difficulty();

//Определяет время до следующего рейда
void define_new_battle_time();