#pragma once
//����� "��������". ���� �������� custom_difficulty != -1 ,�� ��� ����� �������� ����� ������������ ����������� ���������
//������� 0 -> ���� �������
int battle_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h, int custom_difficulty = -1);