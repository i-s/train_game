#pragma once
//Рисует текст text в окне window и на рендерере renderer на rect-e rect.
//Если не выбрать text_color, то будут рисоваться жёлтые буквы на сером фоне
void draw_text(SDL_Window* window, SDL_Renderer* renderer, char* text, SDL_Rect rect, int text_color = 0);

//Вызывает оповещение с заданным текстом text
void call_notificaton(SDL_Window* window, SDL_Renderer* renderer, char* text);

//Вызывает звуковое оповещение
//Если не передать номер звука, будет пищать в течении 3-х секунд
void call_voice_notification(int sound_number = 0);

//Воспроизводит определённую музыку с определённого момента
//Если позиция не указана, то воспроизводится с начала
void play_music(int music_number, double position = 0);