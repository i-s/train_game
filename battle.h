#pragma once


//����� "��������". ���� �������� custom_difficulty != -1 ,�� ��� ����� �������� ����� ������������ ����������� ���������
//������� 0 -> ���� �������
int battle_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h, int custom_difficulty = -1);



//���������� "����" SDL_Texture* texture;SDL_Rect rectangle;
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
	int type; //��� �����: 0 - ������� �����
	bool active = false; //������� �� ���� (����� �� ��� ������������, ����������?)
	int spawn_point;
	int hp; //����� �����
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