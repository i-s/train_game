#pragma once
//����� "�����".
//������� 1 -> ������� � ������ "�����"
//������� 0 -> ���������� ���������
int town_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h);

struct Background {
	SDL_Texture* texture;
	SDL_Rect rectangle;
};

//������ ���
void draw_background(SDL_Renderer* renderer, Background background);