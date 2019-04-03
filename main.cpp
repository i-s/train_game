#include <SDL.h>
#include "menu.h"
#include "train.h"
#include "town.h"
#undef main

int winsize_w = 800, winsize_h = 600;

//��������� ���������� ���������� �� town.cpp
extern int g_humans,g_resourses,g_food;

/*To-do:
V ���������� ������ � ����
1) �������� ������
2) ���� � ����
3) ����� ������ (������� �� ������ � ���� ������)
*/

int main() {
	//������ ����
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* WINDOW = SDL_CreateWindow("Main window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		winsize_w, winsize_h, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(WINDOW, -1, 0);

	//�������� ��������� �������� ��������:
	g_humans = 100;
	g_resourses = 50;
	g_food = 300;

	//��������� ����
	int menu_flag = menu(WINDOW, renderer, winsize_w, winsize_h);
	int train_flag;
	int town_flag;

	//��������� ����
	if (menu_flag == 1) {
		//��������� �����
		 train_flag = train_game(WINDOW, renderer, winsize_w, winsize_h); 
	}

	if (train_flag == 0) {
		town_flag = town_game(WINDOW, renderer, winsize_w, winsize_h);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(WINDOW);
	return 0;
}