#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "train.h"
#include "text.h"
#include "town.h"
#include "menu.h"
#include <stdlib.h>

//���������� ���������� ��������:
//���������� ����
int g_humans;
//���������� ���������
int g_resourses;
//���������� ���
int g_food;

//���������� ����������, ���������� �� ��������� ������� ������. ��� ������: ��� ���� ������
int difficulty;

//���������� ���������� SDL_Rect ��� ��������:
SDL_Rect g_recthumans = { 66,2,197,44 };
SDL_Rect g_rectfood = { 66,47,197,51 };
SDL_Rect g_rectresourses = { 66,99,197,49 };

extern struct Background;

//������ ���
void draw_background(SDL_Renderer* renderer, Background background) {
	SDL_RenderCopy(renderer, background.texture, NULL, &background.rectangle);
}

void Update(SDL_Window* window,SDL_Renderer* renderer, char* texts[]) {
	draw_text(window, renderer, texts[0], g_recthumans);
	draw_text(window, renderer, texts[1], g_rectfood);
	draw_text(window, renderer, texts[2], g_rectresourses);

	SDL_RenderPresent(renderer);
}

//����� "�����".
//������� 1 -> ������� � ������ "�����"
//������� 0 -> ���������� ���������
int town_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h) {
	difficulty = 1;
	//��������� ��������
	//����
	SDL_Surface* background_surf = SDL_LoadBMP("resourses/textures/background_town_normal.bmp");
	SDL_Texture* background_texture = SDL_CreateTextureFromSurface(renderer, background_surf);
	SDL_FreeSurface(background_surf);
	SDL_Surface* background_train_selected_surf = SDL_LoadBMP("resourses/textures/background_town_train_selected.bmp");
	SDL_Texture* background_train_selected_texture = SDL_CreateTextureFromSurface(renderer, background_train_selected_surf);
	SDL_FreeSurface(background_train_selected_surf);

	//������ ������ ����
	SDL_Rect bground_rectangle = { 0,0,winsize_w,winsize_h };
	Background background = { background_texture, bground_rectangle };
	draw_background(renderer, background);

	//������ ����� � ���������� ��� ������������ �������� ����
	SDL_Event event;
	bool quit = false;

	//������ ��� �������� �������� �������� �����, ��� � ��������.
	char* text_humans = new char[10];
	char* text_food = new char[10];
	char* text_resourses = new char[10];

	//������ ����� ��� ������� �������� � �-��
	char* texts[3] = { text_humans, text_food, text_resourses };

	//�������� ���� ������
	SDL_Rect rect_button_train = {289,68,52,74};
	SDL_Rect buttons[1] = {rect_button_train};

	int button_flag; //���������, �������� ����� ������� ������

	int go_to_train = false;
	
	while (!quit && !go_to_train) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			quit = true;

		for (int i = 0; i <= 2; i++) {
			button_flag = CheckIfMouseOnButton(event, i, buttons);
			if (button_flag == 0) {
				background.texture = background_train_selected_texture;
				if (LKMPressed(event)) {
					go_to_train = true; //������� � ������ "�����"
				}
			}
		}

		_itoa_s(g_humans, text_humans, 10, 10);
		_itoa_s(g_food, text_food, 10, 10);
		_itoa_s(g_resourses, text_resourses, 10, 10);

		Update(window,renderer, texts);

		SDL_Delay(10);
	}

	printf_s("hum = %d, res = %d, food = %d\n",g_humans,g_resourses,g_food);

	SDL_DestroyTexture(background_texture);
	SDL_DestroyTexture(background_train_selected_texture);
	if (go_to_train) {
		return 1;
	}
	else {
		return 0;
	}
}