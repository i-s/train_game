#pragma once
//Рисует текст text в окне window и на рендерере renderer на rect-e rect.
//Если не выбрать text_color, то будут рисоваться жёлтые буквы на сером фоне
//Если установить флаг transperent в true, то текст будет писаться без фона
//Цвета: 1 - чёрный на светлосером
//2 - зелёный на светлосером
//3 - красный на светлосером
void draw_text(SDL_Window* window, SDL_Renderer* renderer, char* text, SDL_Rect rect, int text_color = 0, bool transperent = false);

//Вызывает оповещение с заданным текстом text
//Если задать subtext, будет выведен дополнительный текст с цветом color
void call_notificaton(SDL_Window* window, SDL_Renderer* renderer, char* text, char* subtext = NULL, int color = -1);

//Вызывает голосовое оповещение
//Если не передать номер звука, будет пищать в течении 3-х секунд
//Звуки: 
//default
void call_voice_notification(int sound_number = 0);

//Воспроизводит определённую музыку с определённого момента
//Если позиция не указана, то воспроизводится с начала
//Музыка: 0 - Город
//1 - Битва
//2 - Поезд
//default - Город
void play_music(int music_number, double position = 0);

//Воспроизводит определённый звук с определённого момента
//Если позиция не указана, то воспроизводится с начала
//Звуки: 1 - пистолет
//2 - автомат
//3 - пулемет
//4- чека гранаты
//5 взрыв гранаты
//101 - баззер
//default - 0
void play_sound(int sound_number);

//Рисует счётчики ресурсов с поправкой на размер Rect-а.
void draw_number_text(SDL_Window* window, SDL_Renderer* renderer, int number, SDL_Rect rect, int color = 0, bool transparent = false);