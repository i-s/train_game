#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include "menu.h"

/*To-do:
V) �������� ������ �� ����� � �����
V) ��������� train_move (����� �� ���������� �����)
1) �������� � ��������� ��������
2) �������� � ������������ � ���� (��������� ���� � ���� ��������)
3) ����������� ���������� ��� ����������� � �������\������
4) �������������� � ��������
*/

//float x;float y;
struct Point {
	float x;
	float y;
};

//SDL_Rect rect;int switched;SDL_Texture* texture;SDL_Texture* texture_switched;
struct Lever {
	SDL_Rect rect;
	int switched;
	SDL_Texture* texture;
	SDL_Texture* texture_switched;
};

struct Background {
	SDL_Texture* texture;
	SDL_Rect rectangle;
};

//SDL_Texture* texture;SDL_Rect rectangle;float angle;SDL_Point coord;SDL_Rect rectangle;float speed;bool shown
struct Train {
	SDL_Texture* texture;
	SDL_Rect rectangle;
	float angle;
	Point coord;
	float speed;
	bool shown;
};

//SDL_Point point1;SDL_Point point2;
struct Line {
	Point point1;
	Point point2;
};

//������ ���
void draw_background(SDL_Renderer* renderer, Background background) {
	SDL_RenderCopy(renderer, background.texture, NULL, &background.rectangle);
}

//������ �����
void draw_train(SDL_Renderer* renderer, Train train) {
	SDL_RenderCopyEx(renderer, train.texture, NULL, &train.rectangle, train.angle, NULL, SDL_FLIP_NONE);
}

void draw_lever(SDL_Renderer* renderer, Lever lever) {
	if (lever.switched)
		SDL_RenderCopy(renderer, lever.texture_switched, NULL, &lever.rect);
	else
		SDL_RenderCopy(renderer, lever.texture, NULL, &lever.rect);
}

//������� ��������
void CountSpeed(Line path, float speed, float* speed_x, float* speed_y, float* angle) {
	float sin_a, cos_a; // a - ���� ����� ��������� � ���� oY

	//������� ����� � ������� ����
	sin_a = (fabsf(path.point2.x - path.point1.x)) / (sqrt((path.point2.x - path.point1.x)*(path.point2.x - path.point1.x) + (path.point2.y - path.point1.y)*(path.point2.y - path.point1.y)));
	cos_a = sqrt(1 - (sin_a * sin_a));
	*angle = asin(sin_a) * 180 / M_PI;

	//��������, ���� �������� �����. ���� "znak" = 1, �� � ������� ���������� ���� ����������, ���� = -1, �� � ������� ����������.
	int znak_x, znak_y;
	if (path.point1.x <= path.point2.x)
		znak_x = 1;
	else
		znak_x = -1;
	if (path.point1.y <= path.point2.y)
		znak_y = 1;
	else
		znak_y = -1;

	//������� �������� ��������
	*speed_x = speed * sin_a * znak_x;
	*speed_y = speed * cos_a * znak_y;
}

//���������� true, ���� ����� ������ ����� ����
bool Reached_End(Line path, Point current_coord, float speed) {
	//���������� �� ������� ������� �� ����� ����
	float rast = sqrt(((current_coord.x - path.point2.x) * (current_coord.x - path.point2.x)) + ((current_coord.y - path.point2.y) * (current_coord.y - path.point2.y)));
	if ((rast - speed) <= 0) {
		return true;
	}
	return false;
}

//���������� ���������� �� ����� ����
float Rast_to_End(Line path, Point current_coord, float speed) {
	//���������� �� ������� ������� �� ����� ����
	float rast = sqrt(((current_coord.x - path.point2.x) * (current_coord.x - path.point2.x)) + ((current_coord.y - path.point2.y) * (current_coord.y - path.point2.y)));
	return rast - speed;
}

//������� ����� �� speed_x � speed_y.
void Move_Train(float speed_x, float speed_y, Point* train_position) {
	train_position->x += speed_x;
	train_position->y += speed_y;
}

//������������ ���, �����, ������.
void Update(SDL_Renderer* renderer, Train train, Background background, Lever lever1, Lever lever2) {
	draw_background(renderer, background);
	if(train.shown){ draw_train(renderer, train); }	//�� ������������ �����, ���� �� �� ������ ���� �����
	draw_lever(renderer, lever1);
	draw_lever(renderer, lever2);
	SDL_RenderPresent(renderer);
}

void SwitchLever(Lever *lever) {
	if ((*lever).switched == 0) {
		(*lever).switched = 1;
	}
	else {
		(*lever).switched = 0;
	}
}

void game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h) {

	//������ ����� � ���������� ��� ������������ �������� ����
	SDL_Event event;
	bool quit = false;

	//��������� ��������
	//����
	SDL_Surface* background_surf = SDL_LoadBMP("textures/background.bmp");
	SDL_Texture* background_texture = SDL_CreateTextureFromSurface(renderer, background_surf);
	SDL_FreeSurface(background_surf);
	//������
	SDL_Surface* train_surf = SDL_LoadBMP("textures/train.bmp");
	SDL_SetColorKey(train_surf, 1, SDL_MapRGB(train_surf->format, 0, 255, 0));
	SDL_Texture* train_texture = SDL_CreateTextureFromSurface(renderer, train_surf);
	SDL_FreeSurface(train_surf);
	//�������
	SDL_Surface* lever_surf = SDL_LoadBMP("textures/lever.bmp");
	SDL_SetColorKey(lever_surf, 1, SDL_MapRGB(lever_surf->format, 0, 255, 0));
	SDL_Texture* lever_texture = SDL_CreateTextureFromSurface(renderer, lever_surf);
	SDL_FreeSurface(lever_surf);
	//������� �������
	SDL_Surface* lever_switched_surf = SDL_LoadBMP("textures/lever_switched.bmp");
	SDL_SetColorKey(lever_switched_surf, 1, SDL_MapRGB(lever_switched_surf->format, 0, 255, 0));
	SDL_Texture* lever_switched_texture = SDL_CreateTextureFromSurface(renderer, lever_switched_surf);
	SDL_FreeSurface(lever_switched_surf);

	//������ ������ ����
	SDL_Rect bground_rectangle = { 0,0,winsize_w,winsize_h };
	Background background = { background_texture, bground_rectangle };
	draw_background(renderer, background);

	//������ ������
	Lever lever1 = { { 380,60, 40,50 },0,lever_switched_texture,lever_texture };
	Lever lever2 = { { 305,370,40,50 },0,lever_texture,lever_switched_texture };
	draw_lever(renderer, lever1);
	draw_lever(renderer, lever2);

	//������ ������ ��������
	SDL_Rect buttons[2];
	//��������� ������ �������� ��������
	buttons[0] = lever1.rect;
	buttons[1] = lever2.rect;

	//���������� ��������
	SDL_RenderPresent(renderer);

	//����� ����
	Line line0 = { {345,-100},{345,-80} }; //������� ���� �� �������

	Line line1 = { {345,-80},{345,25} }; // ������ ������� ���� (�� ������� ��������)

	Line line2 = { {345,25},{315,100} }; //������ �������
	Line line3 = { {315,100},{-80,225} };

	Line line4 = { {345,25},{345,345} }; //������ ������� ���� (�� ������� ��������)

	Line line5 = { {345,345},{400,440} }; //������ �������
	Line line6 = { {400,440}, {580, 440} };
	Line line7 = { {580,440},{580,300} };

	Line line8 = { {345,345},{345,600} }; //������ ������� ���� (�� ����� ������)

	Line line9 = { {345,600},{345,700} }; //������� ���� �� �������

	Line main_path[10] = { line0, line1,line2,line3,line4,line5,line6,line7,line8,line9 };

	//����� ������ ��������� ��������
	//SDL_Point train_position = { 0,0 };


	int IWantThisToBeTheFirst = 1; //����������, ���������� �� ����� ��������� ������


	SDL_Rect train_rectangle = { main_path[IWantThisToBeTheFirst].point1.x, main_path[IWantThisToBeTheFirst].point1.y,20,80 };
	Train train = { train_texture, train_rectangle , 0 , line1.point1, 0, true };

	//������� �������
	/*
	const int FPS = 60;
	const int max_tick_time = 1000 / FPS;
	int last_tick_time = 0;
	int delta = 0; //������� �� �������
	int clock = 0; //���� ��� ������ �� ���������
	*/

	//������� �����
	printf_s("speed = "); scanf_s("%f", &train.speed);

	bool have_counted_speed = false; //����, ������������, ���� �� ��� ��������� ��������
	int flag_line = 0; //����, ������� ����� 1, ����� �� ����� �� ����� ����.
	int current_path = IWantThisToBeTheFirst; //������ ����� �������� ����.

	//��������� ��������� ������:
	train.coord.x = main_path[current_path].point1.x;
	train.coord.y = main_path[current_path].point1.y;

	//����� ������� ���������
	int time_of_start = time(NULL);
	int time_from_start; //����� �� ������ ���������
	int time_of_last_click=time_of_start; //����� ����������� �����

	float speed_x, speed_y;

	float rast;

	//��������� �������
	while (!quit) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			quit = true;

		//������ ��� � ����� �������, ��� ���� �� �������� �� �� ���� �� ������
		int button_flag = -1;

		if (time(NULL) - time_of_last_click >= 1) { //������� �� ���� 1 ���� � �������
			for (int i = 0; i <= 1; i++) {
				button_flag = CheckIfMouseOnButton(event, i, buttons);
				if (LKMPressed(event)) {
					if (button_flag == 0) {
						SwitchLever(&lever1);
						time_of_last_click = time(NULL);
					}
					else if (button_flag == 1) {
						SwitchLever(&lever2);
						time_of_last_click = time(NULL);
					}
				}
			}
		}

		if (train.shown) {

			//���������, ����� �� ������� � ������ ���� � ���������, ���� �����
			if (flag_line) {
				have_counted_speed = false;
				if (current_path == 1) { //���� �� ������ ������� ����
					if (lever1.switched) { //���� ������ ����� �����
						current_path = 2; //������������ �� ������ �������
					}
					else {
						current_path = 4; //���������� �������� ����
					}
				}

				else if (current_path == 2) { //���� �� ������������
					current_path = 3; //���������� ������������
				}

				else if (current_path == 3) { //���� ����� �� ����� ��������
					train.shown = 0; //�������� �����
				}

				else if (current_path == 4) { //���� �� ������ ������� ����
					if (lever2.switched) { //���� ������ ����� �����
						current_path = 5; //������������ �� ������ �������
					}
					else {
						current_path = 8; //���������� �������� ����
					}
				}

				else if (current_path == 5) { //���� �� ������������
					current_path = 6; //���������� ������������
				}
				else if (current_path == 6) { //���� �� ������������
					current_path = 7; //���������� ������������
				}
				else if (current_path == 7) { //���� ����� �� ����� ��������
					train.shown = 0; //�������� �����
				}

				else if (current_path == 8) {//���� ����� �� ����� ����
					current_path = 9;
					train.shown = 0; //�������� �����
				}

				train.coord.x = main_path[current_path].point1.x;
				train.coord.y = main_path[current_path].point1.y;
			}

			if (!have_counted_speed) {
				CountSpeed(main_path[current_path], train.speed, &speed_x, &speed_y, &train.angle);
				if (speed_x > 0) {
					train.angle = 360 - train.angle;
				}
				if (speed_y < 0) {
					train.angle = train.angle + 180;
				}
				have_counted_speed = true;
			}

			//if (Reached_End(main_path[current_path], train.coord, train.speed)) {
			rast = Rast_to_End(main_path[current_path], train.coord, train.speed);
			if (rast <=1) {
				train.coord = main_path[current_path].point2;
				flag_line = 1;
			}
			else {
				Move_Train(speed_x, speed_y, &train.coord);
				train.rectangle.x = train.coord.x;
				train.rectangle.y = train.coord.y;
				flag_line = 0;
			}
		}

		Update(renderer, train, background, lever1, lever2);

		time_from_start = time(NULL) - time_of_start;
		
		//����� �����-����������
		printf_s("time = %d : x = %.0f, y = %.0f, current_way = %d, rast = %.0f\n",time_from_start,train.coord.x, train.coord.y, current_path, rast);

		SDL_Delay(10);

		//�������� ���������� ������
		if (!train.shown) {
			train.shown = true;
			current_path = IWantThisToBeTheFirst;
			have_counted_speed = false;
			train.coord.x = main_path[current_path].point1.x;
			train.coord.y = main_path[current_path].point1.y;
		}
		
		///������� �����
		//int tick_time = SDL_GetTicks();
		//delta = tick_time - last_tick_time;
		//printf_s("sec = %.3f y=%d\n", float(delta) / 1000, train_start_position.y); //������� �����-����������
		//last_tick_time = tick_time;

		//������������ ���.
		/*if (delta < max_tick_time)
			SDL_Delay(max_tick_time - delta);
		*/
	}

	SDL_DestroyTexture(background_texture);
	SDL_DestroyTexture(train_texture);
}