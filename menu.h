#pragma once
//������� 0 -> ��������� �������
//������� 1 -> ������ ������ Start
//������� -1 -> �������� � �������
int menu(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h);

//���������� 1, ���� ��� ������, � 0, ���� ���.
int LKMPressed(SDL_Event event);

//���������� ����� ������, �� ������� ������ ������, ��� -1, ���� �� ��������.
int CheckIfMouseOnButton(SDL_Event event, int i, SDL_Rect buttons[]);