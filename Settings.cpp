#pragma once
#include "Settings.h"
#include "town.h"
#include "menu.h"
#include "battle.h"
#include "notifications.h"


extern int g_music_volume;
extern int g_sound_volume;

extern int user_winsize_w;
extern int user_winsize_h;
extern bool fullscreen;

extern bool QUIT;

int get_settings_of_file()
{
	FILE *f;
	if (fopen_s(&f, "setting.cfg", "r") != NULL)
	{
		printf("HE MOGU\n");
		return -1;
	}
	int gmv = 0;
	fscanf_s(f, "g_music_volume=%d\n", &gmv);
	int gsv = 0;
	fscanf_s(f, "g_sound_volume=%d\n", &gsv);
	int ww = 0;
	fscanf_s(f, "winsize_w=%d\n", &ww);
	int wh = 0;
	fscanf_s(f, "winsize_h=%d\n", &wh);
	int fl = 0;
	fscanf_s(f, "fullscreen=%d", &fl);
	if (true)
	{
		//TODO: �������� ������������ ��������
		g_music_volume = gmv;
		g_sound_volume = gsv;
		user_winsize_w = ww;
		user_winsize_h = wh;
		fullscreen = fl;
	}
	//printf_s("%d, %d, %d, %d", gmv, gsv, ww, wh, fl);
	fclose(f);
	return 1;
}


int set_settings_in_file()
{
	FILE *f;
	if (fopen_s(&f, "setting.cfg", "w") != NULL)
	{
		printf("HE MOGU\n");
		return -1;
	}
	fprintf_s(f, "g_music_volume=%d\n",g_music_volume);
	fprintf_s(f, "g_sound_volume=%d\n", g_sound_volume);
	fprintf_s(f, "winsize_w=%d\n", user_winsize_w);
	fprintf_s(f, "winsize_h=%d\n", user_winsize_h);
	fprintf_s(f, "fullscreen=%d", fullscreen);
	fclose(f);
	return 1;
}


void Update(SDL_Window* window, SDL_Renderer* renderer, Background backgroung, SDL_Rect buttons[], char* texts[],
		int button_flag, int screensize, int volmus, int volsound, bool need_notification) {

	draw_background(renderer, backgroung);
	
	//������ ����������
	for (int i = 0; i < 2; i++)//mus
	{
		int k = 5;
		if (screensize == i)
			k = 0;
		draw_text(window, renderer, texts[i], buttons[i], k);
	}

	//������ ��������
	int k = 5;
	if (fullscreen && screensize == -1)
		k = 0;
	draw_text(window, renderer, (char*)"YES", buttons[2], k);
	k = 5;
	if (!fullscreen)
		k = 0;
	draw_text(window, renderer, (char*)"NO", buttons[11], k);		

	//������ ���������
	for (int i = 0; i < 3; i++)//mus
	{
		int k = 5;
		if (volmus == i)
			k = 0;
		draw_text(window, renderer, texts[2 + i], buttons[3+i], k);
	}
	for (int i = 0; i < 3; i++)//sound
	{
		int k = 5;
		if (volsound == i)
			k = 0;
		draw_text(window, renderer, texts[2 + i], buttons[6 + i], k);
	}

	//������ ���� � �����
	draw_text(window, renderer, texts[5], buttons[9], 0);
	draw_text(window, renderer, texts[6], buttons[10], 0);


	if (button_flag != -1)//���� ����� �� ������ , ��������� ������
		draw_blackout(window, renderer, buttons[button_flag], 0, 50);

	SDL_RenderPresent(renderer);
}


int get_numb_vol(int vol)//���������� ����� ��������� 
{
	if (vol > 100) return 0;
	if (vol >= 50) return 1;
	return 2;
}

int get_numb_sizescreen()//���������� ����� ������� ������
{
	if (fullscreen)
		return -1;
	if (user_winsize_w == 800 && user_winsize_h == 600)
		return 0;
	else if (user_winsize_w == 1440 && user_winsize_h == 900)
		return 1;
	return 2;
}


int launch_settings(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h) {

	//������ ��������
	SDL_Surface* settings_surf = SDL_LoadBMP("resourses/textures/background_settings.bmp");
	SDL_Texture* settings_texture = SDL_CreateTextureFromSurface(renderer, settings_surf);
	SDL_FreeSurface(settings_surf);

	//���)
	Background backgroung;
	backgroung.rectangle = { 0, 0, winsize_w, winsize_h };
	backgroung.texture = settings_texture;

	//��������� , ��� ������
	SDL_Rect but_scrnsz1 = {437, 64, 130, 59};
	SDL_Rect but_scrnsz2 = { 600, 64, 130, 59 };
	SDL_Rect but_fullscrnon = { 287, 158, 64, 61 };
	SDL_Rect but_fullscrnoff = { 366, 158, 64, 61 };
	SDL_Rect but_volmus1 = { 382, 283, 94, 59 };
	SDL_Rect but_volmus2 = { 502, 283, 94, 59 };
	SDL_Rect but_volmus3 = { 615, 283, 94, 59 };
	SDL_Rect but_volsond1 = { 382, 361, 94, 59 };
	SDL_Rect but_volsond2 = { 502, 361, 94, 59 };
	SDL_Rect but_volsond3 = { 615, 361, 94, 59 };
	SDL_Rect but_back = { 382, 471, 184, 54 };
	SDL_Rect but_save = { 188, 471, 184, 54 };
	
	SDL_Rect buttons[12] = { but_scrnsz1, but_scrnsz2, but_fullscrnon , but_volmus1 , but_volmus2 , but_volmus3,
									but_volsond1 ,but_volsond2 ,but_volsond3 ,but_save, but_back, but_fullscrnoff };



	char* texts[7] = { (char*)u8"800x600" , (char*)u8"1440x900", (char*)u8"������", (char*)u8"������", (char*)u8"����",
						(char*)u8"���������", (char*)u8"�����" };

	SDL_Event event;
	bool quit = false;

	int screensize = get_numb_sizescreen();//���� , �������� ���������� 0 - 800*600, 1 - 1440*900, -1 - fullscreen, -2 - ������
	int volmus = get_numb_vol(g_music_volume); //���� ��� ��������� ������ 0 - ������, 1- ������, 2 - ����
	int volsound = get_numb_vol(g_sound_volume); //���� ��� ��������� ����� 0 - ������, 1- ������, 2 - ����

	bool need_notification = false;

	while (!quit && !QUIT) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			QUIT = true;
			quit = true;
		}
		int current_button = -1;
		int button_flag = -1;
		for (int i = 0; i <= 14; i++) {
			button_flag = CheckIfMouseOnButton(event, i, buttons);

			switch (button_flag) {
			case -1: break;
				//������� �� ������
			case 0:
			{
				current_button = 0;
				if (LKMPressed(event) && screensize != 0) {//������� �� ������ 800�600
					screensize = 0;
					fullscreen = false;
					user_winsize_w = 800;
					user_winsize_h = 600;
					need_notification = true;
				}
				break;
			}
			case 1:
			{
				current_button = 1;
				if (LKMPressed(event) && screensize != 1) {//������� �� ������ 1440�900
					screensize = 1;
					fullscreen = false;
					user_winsize_w = 1440;
					user_winsize_h = 900;
					need_notification = true;
				}
				break;
			}
			case 2:
			{
				current_button = 2;
				if (LKMPressed(event) && !fullscreen) {//������� �� ������ fullscreenon
					screensize = -1;
					fullscreen = true;
					need_notification = true;
				}
				break;
			}
			case 3:
			{
				current_button = 3;
				if (LKMPressed(event)) {//������� �� ������ ��� ������
					volmus = 0;
					g_music_volume = 128;
				}
				break;
			}
			case 4:
			{
				current_button = 4;
				if (LKMPressed(event)) {//������� �� ������ ��� c�����
					volmus = 1;
					g_music_volume = 50;
				}
				break;
			}
			case 5:
			{
				current_button = 5;
				if (LKMPressed(event)) {//������� �� ������ ��� ����
					volmus = 2;
					g_music_volume = 0;
				}
				break;
			}
			case 6:
			{
				current_button = 6;
				if (LKMPressed(event)) {//������� �� ������ ��� ������
					volsound = 0;
					g_sound_volume = 128;
				}
				break;
			}
			case 7:
			{
				current_button = 7;
				if (LKMPressed(event)) {//������� �� ������ ��� ������
					volsound = 1;
					g_sound_volume = 50;
				}
				break;
			}
			case 8:
			{
				current_button = 8;
				if (LKMPressed(event)) {//������� �� ������ ��� ����
					volsound = 2;
					g_sound_volume = 0;
				}
				break;
			}
			case 9:
			{
				current_button = 9;
				if (LKMPressed(event)) {//������� �� ������ ���������
					set_settings_in_file();
					//������� ������� � ����������
					quit = 1;
				}
				break;
			}
			case 10:
			{
				current_button = 10;
				if (LKMPressed(event)) {//������� �� ������ �����
					quit = 1;//����� �� �������� ������
				}
				break;
			}
			case 11:
			{
				current_button = 11;
				if (LKMPressed(event) && fullscreen) {//������� �� ������ fullscreenoff
					screensize = 0;
					fullscreen = false;
				}
				break;
			}
			}
		}
		Update(window, renderer, backgroung, buttons, texts, current_button, screensize, volmus, volsound, need_notification);


		need_notification = false;
	}
	

	SDL_DestroyTexture(settings_texture);

	return 0;
}
