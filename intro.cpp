#include <SDL.h>
#include <stdio.h>
#include "battle.h"
//#include <time.h>

extern bool QUIT;
extern int winsize_w, winsize_h;

//����������� �����
void Intro(SDL_Window* window, SDL_Renderer* renderer) {
	//��������� ��������
	//���� ������
	SDL_Surface* frame1_surf = SDL_LoadBMP("resourses/textures/intro/frame1.bmp");
	SDL_SetColorKey(frame1_surf, 1, SDL_MapRGB(frame1_surf->format, 0, 255, 0));
	SDL_Texture* frame1_texture = SDL_CreateTextureFromSurface(renderer, frame1_surf);
	SDL_FreeSurface(frame1_surf);
	SDL_Surface* frame2_surf = SDL_LoadBMP("resourses/textures/intro/frame2.bmp");
	SDL_SetColorKey(frame2_surf, 1, SDL_MapRGB(frame2_surf->format, 0, 255, 0));
	SDL_Texture* frame2_texture = SDL_CreateTextureFromSurface(renderer, frame2_surf);
	SDL_FreeSurface(frame2_surf);
	SDL_Surface* frame3_surf = SDL_LoadBMP("resourses/textures/intro/frame3.bmp");
	SDL_SetColorKey(frame3_surf, 1, SDL_MapRGB(frame3_surf->format, 0, 255, 0));
	SDL_Texture* frame3_texture = SDL_CreateTextureFromSurface(renderer, frame3_surf);
	SDL_FreeSurface(frame3_surf);
	SDL_Surface* frame4_surf = SDL_LoadBMP("resourses/textures/intro/frame4.bmp");
	SDL_SetColorKey(frame4_surf, 1, SDL_MapRGB(frame4_surf->format, 0, 255, 0));
	SDL_Texture* frame4_texture = SDL_CreateTextureFromSurface(renderer, frame4_surf);
	SDL_FreeSurface(frame4_surf);
	SDL_Surface* frame5_surf = SDL_LoadBMP("resourses/textures/intro/frame5.bmp");
	SDL_SetColorKey(frame5_surf, 1, SDL_MapRGB(frame5_surf->format, 0, 255, 0));
	SDL_Texture* frame5_texture = SDL_CreateTextureFromSurface(renderer, frame5_surf);
	SDL_FreeSurface(frame5_surf);
	SDL_Surface* frame6_surf = SDL_LoadBMP("resourses/textures/intro/frame6.bmp");
	SDL_SetColorKey(frame6_surf, 1, SDL_MapRGB(frame6_surf->format, 0, 255, 0));
	SDL_Texture* frame6_texture = SDL_CreateTextureFromSurface(renderer, frame6_surf);
	SDL_FreeSurface(frame6_surf);
	//��������
	SDL_Surface* picture1_surf = SDL_LoadBMP("resourses/textures/intro/picture1.bmp");
	SDL_SetColorKey(picture1_surf, 1, SDL_MapRGB(picture1_surf->format, 0, 255, 0));
	SDL_Texture* picture1_texture = SDL_CreateTextureFromSurface(renderer, picture1_surf);
	SDL_FreeSurface(picture1_surf);
	SDL_Surface* picture2_surf = SDL_LoadBMP("resourses/textures/intro/picture2.bmp");
	SDL_SetColorKey(picture2_surf, 1, SDL_MapRGB(picture2_surf->format, 0, 255, 0));
	SDL_Texture* picture2_texture = SDL_CreateTextureFromSurface(renderer, picture2_surf);
	SDL_FreeSurface(picture2_surf);
	SDL_Surface* picture3_surf = SDL_LoadBMP("resourses/textures/intro/picture3.bmp");
	SDL_SetColorKey(picture3_surf, 1, SDL_MapRGB(picture3_surf->format, 0, 255, 0));
	SDL_Texture* picture3_texture = SDL_CreateTextureFromSurface(renderer, picture3_surf);
	SDL_FreeSurface(picture3_surf);
	SDL_Surface* picture4_surf = SDL_LoadBMP("resourses/textures/intro/picture4.bmp");
	SDL_SetColorKey(picture4_surf, 1, SDL_MapRGB(picture4_surf->format, 0, 255, 0));
	SDL_Texture* picture4_texture = SDL_CreateTextureFromSurface(renderer, picture4_surf);
	SDL_FreeSurface(picture4_surf);
	
	//������ ������� ������
	SDL_Texture* frames[6] = { frame1_texture ,frame2_texture ,frame3_texture ,frame4_texture ,frame5_texture ,frame6_texture };
	//������ ������� ��������
	SDL_Texture* pictures[6] = { picture1_texture,picture2_texture,picture3_texture,picture4_texture,picture4_texture,picture4_texture };

	//������� ����������� ������ ������������ ��� ��� ���� ����.
	int frame_time[6] = {6000,3000,6000,6000,6000,6000};

	int current_frame = 0; //������� ����

	SDL_Event event;

	bool quit = false; //���� ������ �� ����� (���������� �����)

	int start_intro_time = SDL_GetTicks(); //����� ������ ������ �����
	int current_time; //������� �����
	int last_time = start_intro_time; //��������� �������� �����
	int last_frame_time=0; //������� ��� ������������ ��������� ����
	int time_since_intro_started = 0; //������� ������� ������ � ������ �����
	int delta_time; //������� �� �������

	SDL_Rect full_screen = { 0,0,winsize_w,winsize_h };
	SDL_Rect picture_rect = {200,100,400,199};

	while (!quit) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			QUIT = true;
			quit = true;
		}
		if (event.key.keysym.sym == SDLK_SPACE) {
			quit = true;
		}

		bool need_to_change_frame = false; //����, ���������� �� ����� �����

		if (last_frame_time < frame_time[current_frame]){ //���� ������� ���� ���������� ������, ��� �����
			//���������� ����
			SDL_RenderCopy(renderer, frames[current_frame], NULL, &full_screen);
			SDL_RenderCopy(renderer, pictures[current_frame], NULL, &picture_rect);
		}
		else {
			need_to_change_frame = true;
			if (current_frame < 6) { //���� �������� ����� ��� ���������
				current_frame++; //...������ ����
			}
			else {
				quit = true; //������� �� ���������
			}
		}

		if (time_since_intro_started < 1000) { //������� ������� �� ������� � �������� � ������� ������ ������� ������� �����
			int i = 255 - int((float(time_since_intro_started) / 1000) * 255);
			draw_blackout(window, renderer, full_screen, 0, i);
			//printf_s("%d %d\n", time_since_intro_started,i);
		}
		SDL_RenderPresent(renderer);

		//������ � ��������		
		SDL_Delay(10);
		current_time = SDL_GetTicks(); //��������� ������� �����
		delta_time = current_time - last_time; //����������� ������� ����� ��������� ���������� �������� � �������
		last_time = current_time; //���������� ������� �����
		if (need_to_change_frame) { //���� ����� ������� ����
			last_frame_time = 0; //...���������� ����� ����� ����� �� 0
		}
		else {
			last_frame_time += delta_time; //����������� ����� �������� �����
		}
		time_since_intro_started += delta_time;//����������� ����� ����� �����
		//printf_s("%d\n", last_frame_time);
	}

	//���� � ������
	int i = 0;
	while (i < 45) {
		i++;
		printf_s("%d\n",i);
		draw_blackout(window, renderer, full_screen, 0, i);
		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}

	//��������� �������� �� ������
	for (int i = 0; i < 6; i++) {
		SDL_DestroyTexture(frames[i]);
	}
	for (int i = 0; i < 4; i++) {
		SDL_DestroyTexture(pictures[i]);
	}
}