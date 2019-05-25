#pragma once
#include "train.h"


//����� "��������". ���� �������� custom_difficulty != -1 ,�� ��� ����� �������� ����� ������������ ����������� ���������
//������� 0 -> ���� �������
int battle_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h, int custom_difficulty = -1);



//���������� "����" SDL_Texture* texture;SDL_Rect rectangle;
//Point coordinates;
//float real_size_w, real_size_h;
//int type; //��� �����: 0 - ������� ����� 
//bool active = false; ������� �� ���� (����� �� ��� ������������, ����������?)
//int spawn_point;
//int hp; ��������
//������ �������� � �������: ���, �������, ����
//int loss_food;
//int loss_resourses;
//int loss_humans;
struct Enemy {
	SDL_Texture* texture;
	SDL_Rect rectangle;
	Point coordinates;
	float real_size_w; //��������, ������� ������ �����
	float real_size_h; //��������, ������� ������ �����
	int type; //��� �����: 0 - ������� �����
	bool active = false; //������� �� ���� (����� �� ��� ������������, ����������?)
	int spawn_point;
	int hp; //����� �����
	float speed; //�������� �����
	//������ �������� � �������: ���, �������, ����
	int loss_food;
	int loss_resourses;
	int loss_humans;
};

struct SpawnPoint {
	SDL_Point point;
	bool open = true; //�������� �� ����� ��� ������
};

struct Gun {
	int damage;
	float cooldown;
	SDL_Rect cooldown_rect;
	SDL_Rect button_rect;
	float time_end_reload = 100;// ������ ������ ��������
	float cost;
	bool active = true;
	int sound_number;
	//����
};

//������ ������ ������ �������� ����������
void draw_blackout(SDL_Window* window, SDL_Renderer* renderer, SDL_Rect rect,
	float percentage_of_recharge, int blackout_level);

//��������� ���������� ����� � ��������������, ����� 1, ���� ���������, � 0 , ���� ���
int check_mouse_on_rect(SDL_Rect rectangle, SDL_Event event);