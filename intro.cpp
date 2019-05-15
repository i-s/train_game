#include <SDL.h>
#include <stdio.h>
#include "battle.h"
//#include <time.h>

extern bool QUIT;
extern int winsize_w, winsize_h;

//Проигрывает интро
void Intro(SDL_Window* window, SDL_Renderer* renderer) {
	//Загружаем текстуры
	//Фоны кадров
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
	//Картинок
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
	
	//Массив текстур кадров
	SDL_Texture* frames[6] = { frame1_texture ,frame2_texture ,frame3_texture ,frame4_texture ,frame5_texture ,frame6_texture };
	//Массив текстур картинок
	SDL_Texture* pictures[6] = { picture1_texture,picture2_texture,picture3_texture,picture4_texture,picture4_texture,picture4_texture };

	//Сколько миллисекунд должен показываться тот или иной кадр.
	int frame_time[6] = {6000,3000,6000,6000,6000,6000};

	int current_frame = 0; //Текущий кадр

	SDL_Event event;

	bool quit = false; //Флаг выхода из цикла (завершения интро)

	int start_intro_time = SDL_GetTicks(); //Время начала показа интро
	int current_time; //Текущее время
	int last_time = start_intro_time; //Последнее хранимое время
	int last_frame_time=0; //Сколько уже показывается последний кадр
	int time_since_intro_started = 0; //Сколько времени прошло с начала интро
	int delta_time; //Разница во времени

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

		bool need_to_change_frame = false; //Флаг, отвечающий за смену кадра

		if (last_frame_time < frame_time[current_frame]){ //Если текущий кадр показывают меньше, чем нужно
			//Показываем кадр
			SDL_RenderCopy(renderer, frames[current_frame], NULL, &full_screen);
			SDL_RenderCopy(renderer, pictures[current_frame], NULL, &picture_rect);
		}
		else {
			need_to_change_frame = true;
			if (current_frame < 6) { //Если остались кадры для отрисовки
				current_frame++; //...меняем кадр
			}
			else {
				quit = true; //Выходим из отрисовки
			}
		}

		if (time_since_intro_started < 1000) { //Плавный переход из чёрного в картинку в течении первых полторы секунды интро
			int i = 255 - int((float(time_since_intro_started) / 1000) * 255);
			draw_blackout(window, renderer, full_screen, 0, i);
			//printf_s("%d %d\n", time_since_intro_started,i);
		}
		SDL_RenderPresent(renderer);

		//Работа с временем		
		SDL_Delay(10);
		current_time = SDL_GetTicks(); //Обновляем текущее время
		delta_time = current_time - last_time; //Высчитываем разницу между последним сохранённым временем и текущим
		last_time = current_time; //Запоминаем текущее время
		if (need_to_change_frame) { //Если нужно сменить кадр
			last_frame_time = 0; //...сбрасываем время этого кадра на 0
		}
		else {
			last_frame_time += delta_time; //Увеличиваем время текущего кадра
		}
		time_since_intro_started += delta_time;//Увеличиваем общее время интро
		//printf_s("%d\n", last_frame_time);
	}

	//Уход в чёрное
	int i = 0;
	while (i < 45) {
		i++;
		draw_blackout(window, renderer, full_screen, 0, i);
		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}

	//Выгружаем текстуры из памяти
	for (int i = 0; i < 6; i++) {
		SDL_DestroyTexture(frames[i]);
	}
	for (int i = 0; i < 4; i++) {
		SDL_DestroyTexture(pictures[i]);
	}
}