#pragma once
//Возврат 0 -> программа закрыта
//Возврат 1 -> нажата кнопка Start
//Возврат -1 -> проблема с кнопкой
int menu(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h);

//Возвращает 1, если ЛКМ нажата, и 0, если нет.
int LKMPressed(SDL_Event event);

//Возвращает номер кнопки, на которую наведён курсор, или -1, если не наведена.
int CheckIfMouseOnButton(SDL_Event event, int i, SDL_Rect buttons[]);