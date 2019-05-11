#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include "town.h"

//Рисует текст text в окне window и на рендерере renderer на rect-e rect.
//Если не выбрать text_color, то будут рисоваться жёлтые буквы на сером фоне
void draw_text(SDL_Window* window, SDL_Renderer* renderer, char* text, SDL_Rect rect, int text_color = 0) {
	TTF_Init();
	TTF_Font* my_font = TTF_OpenFont("resourses/Text.ttf", 100);
	SDL_Color fore_color;
	SDL_Color back_color;

	switch (text_color)
	{
	case 1: { //Вывод предупредающего, чёрного текста на светлосером фоне (для предупреждений)
		fore_color = { 0,0,0 };
		back_color = { 245, 245, 245 };
		break;
	}
	default: { //Если цвет не определён специально (text_color = 0) выводим жёлые буквы на сером фоне
		fore_color = { 255,216,0 };
		back_color = { 128, 128, 128 };
		break;
		}
	}
	SDL_Surface* textSurface = NULL;
	textSurface = TTF_RenderText_Shaded(my_font, text, fore_color, back_color);
	//SDL_Rect rect = { point.x - 15,point.y,30,30 };
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(my_font);
	TTF_Quit();
}

extern int winsize_w, winsize_h;
extern int g_music_volume;

//Вызывает оповещение с заданным текстом text
void call_notificaton(SDL_Window* window, SDL_Renderer* renderer, char* text) {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); //Разрешаем смешивание цветов
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
	SDL_Rect notification_background = {0,0,winsize_w,winsize_h}; //Фон, размерами с окно
	SDL_RenderFillRect(renderer, &notification_background); //Отрисовываем затемнение окна
	SDL_Rect notification_box = {200,250,400,100}; //Окошко для текста
	draw_text(window, renderer, text, notification_box,1);
	SDL_RenderPresent(renderer);
	SDL_Delay(3000); //Задержка на 3 секунды
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE); //Хватит смешивать цвета!
}

//Вызывает звуковое оповещение
//Если не передать номер звука, будет пищать в течении 3-х секунд
void call_voice_notification(int sound_number = 0) {
	Mix_Init(MIX_INIT_MP3);
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		// Обработка ошибки
	}
	Mix_Music *music;
	switch (sound_number)
	{
	default:
		music = Mix_LoadMUS("resourses/music/buzzer.mp3"); break;
	}
	if (music == NULL) {
		printf("Mix_LoadMUS: %s\n", Mix_GetError());
		// Обработка ошибки
	}
	if (Mix_PlayMusic(music, 1) == -1) {
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
		// Обработка ошибки
		Mix_VolumeMusic(g_music_volume);
	}
}

//Воспроизводит определённую музыку с определённого момента
//Если позиция не указана, то воспроизводится с начала
void play_music(int music_number, double position = 0) {
	Mix_Init(MIX_INIT_MP3);
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		// Обработка ошибки
	}
	Mix_Music *music;
	switch (music_number)
	{
	case 0: music = Mix_LoadMUS("resourses/music/music_test.mp3"); break; //Музыка для города
	case 1: music = Mix_LoadMUS("resourses/music/music_attack.mp3"); break; //Музыка для битвы
	case 2: music = Mix_LoadMUS("resourses/music/music_train.mp3"); break; //Музыка для поезда
	default: music = Mix_LoadMUS("resourses/music/music_test.mp3"); break;
	}
	if (music == NULL) {
		printf("Mix_LoadMUS: %s\n", Mix_GetError());
		// Обработка ошибки
	}
	Mix_RewindMusic();
	if (Mix_FadeInMusic(music,-1,500) == -1) {
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
		// Обработка ошибки
	}
	Mix_SetMusicPosition(position);
	Mix_VolumeMusic(g_music_volume);
}