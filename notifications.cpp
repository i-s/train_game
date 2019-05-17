#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include "town.h"

//Рисует текст text в окне window и на рендерере renderer на rect-e rect.
//Если не выбрать text_color, то будут рисоваться жёлтые буквы на сером фоне
//Если установить флаг transperent в true, то текст будет писаться без фона
//Цвета: 1 - чёрный на светлосером
//2 - зелёный на светлосером
//3 - красный на светлосером
void draw_text(SDL_Window* window, SDL_Renderer* renderer, char* text, SDL_Rect rect, int text_color = 0, bool transperent = false) {
	
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
	case 2: { //Вывод зелёного текста на светлосером фоне
		fore_color = { 10,210,10 };
		back_color = { 245, 245, 245 };
		break;
	}
	case 3: { //Вывод красного текста на светлосером фоне
		fore_color = { 210,10,10 };
		back_color = { 245, 245, 245 };
		break;
	}
	case 4: { //Вывод черного текста на белом фоне
		fore_color = { 0,0,0 };
		back_color = { 255, 255, 255 };
		break;
	}
	default: { //Если цвет не определён специально (text_color = 0) выводим жёлые буквы на сером фоне
		fore_color = { 255,216,0 };
		back_color = { 128, 128, 128 };
		break;
		}
	}
	SDL_Surface* textSurface = NULL;
	if (!transperent)
		textSurface = TTF_RenderUTF8_Shaded(my_font, text, fore_color, back_color);
	else
		textSurface = TTF_RenderUTF8_Blended(my_font, text, fore_color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(my_font);
	TTF_Quit();
}

//Рисует счётчики ресурсов с поправкой на размер Rect-а.
void draw_number_text(SDL_Window* window, SDL_Renderer* renderer, int number, SDL_Rect rect, int color = 0, bool transparent = false) {
	SDL_Rect new_rect = rect;
	if (number / 10 < 1) { //Если число одноразрядное
		new_rect.x += new_rect.w * 2 / 3;
		new_rect.w /= 3;
	}
	else if (number / 100 < 1) { //Если число двухразрядное
		new_rect.x += new_rect.w / 2;
		new_rect.w /= 2;
	}
	else if (number / 1000 < 1) { //Если число трёхразрядное
		new_rect.x += new_rect.w * 0.5 / 1.5;
		new_rect.w = int(new_rect.w / 1.5);
	}
	else { //Если число других размеров
		//...ничего не делаем
	}
	char* text = new char[10];
	_itoa_s(number, text, 10, 10);
	draw_text(window,renderer,text,new_rect, color, transparent);
}

extern int winsize_w, winsize_h;
extern int g_music_volume;
extern int g_sound_volume;

//Вызывает оповещение с заданным текстом text
//Если задать subtext, будет выведен дополнительный текст с цветом color
void call_notificaton(SDL_Window* window, SDL_Renderer* renderer, char* text, char* subtext = NULL, int color = -1) {
	//Загружаем текстуру подложки оповещения
	SDL_Surface* notification_box_surf = SDL_LoadBMP("resourses/textures/notification_box.bmp");
	SDL_SetColorKey(notification_box_surf, 1, SDL_MapRGB(notification_box_surf->format, 0, 255, 0));
	SDL_Texture* notification_box_texture = SDL_CreateTextureFromSurface(renderer, notification_box_surf);
	SDL_FreeSurface(notification_box_surf);

	SDL_Rect notification_box = {200,250,400,100}; //Окошко для текста
	SDL_RenderCopy(renderer, notification_box_texture, NULL, &notification_box);
	//После отрисовки границы немного уменьшаем размеры box-а, чтобы текст на заходил на рамки
	notification_box = { 205,255,390,90 };

	SDL_Rect sub_notification_box = { 225,350,350,90 }; //Окошко для дополнительного текста
	draw_text(window, renderer, text, notification_box,1,true);
	if (subtext != NULL && color > -1) {
		SDL_RenderCopy(renderer, notification_box_texture, NULL, &sub_notification_box);
		sub_notification_box = { 230,355,340,80 };
		draw_text(window, renderer, subtext, sub_notification_box,color,true);
	}
	SDL_RenderPresent(renderer);
	SDL_Delay(3000); //Задержка на 3 секунды
	//Выгружаем текстуры
	SDL_DestroyTexture(notification_box_texture);
}

//Вызывает голосовое оповещение
//Если не передать номер звука, будет пищать в течении 3-х секунд
//Звуки: 
//default
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
//Музыка: 0 - Город
//1 - Битва
//2 - Поезд
//default - Город
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

//Воспроизводит определённый звук с определённого момента
//Если позиция не указана, то воспроизводится с начала
//Звуки: 1 - пистолет
//2 - автомат
//3 - пулемет
//4- чека гранаты
//5 взрыв гранаты
//101 - баззер
//default - 0
void play_sound(int sound_number) {
	Mix_Init(MIX_INIT_MP3);
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		// Обработка ошибки
	}
	Mix_Chunk *sound;
	switch (sound_number)
	{
	case 1: sound = Mix_LoadWAV("resourses/sound/sound_pistol.mp3"); break; //звук для пистолета
	case 2: sound = Mix_LoadWAV("resourses/sound/sound_automat.mp3"); break; //звук для автомата
	case 3: sound = Mix_LoadWAV("resourses/sound/sound_machinegun.mp3"); break; //звук для пулемета
	case 4: sound = Mix_LoadWAV("resourses/sound/sound_grenade1.mp3"); break; //звук для гранаты 1-ый
	case 5: sound = Mix_LoadWAV("resourses/sound/sound_grenade2.mp3"); break; //звук для гранаты 2-ой
	case 101: sound = Mix_LoadWAV("resourses/sound/buzzer.mp3"); break; //Баззер, оповещение
	default: sound = Mix_LoadWAV("resourses/sound/sound_test.mp3"); break;
	}
	if (sound == NULL) {
		printf("Mix_LoadWAV: %s\n", Mix_GetError());
		// Обработка ошибки
	}
	Mix_RewindMusic();
	if (Mix_PlayChannel(-1, sound, 0) == -1 ) {
		printf("Mix_Playsound: %s\n", Mix_GetError());
		// Обработка ошибки
	}
	Mix_VolumeChunk(sound, g_sound_volume);
}
