#pragma once
//Экран "город".
//Возврат 1 -> переход к экрану "поезд"
//Возврат 0 -> завершение программы
int town_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h);

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
//