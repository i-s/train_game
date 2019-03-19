#pragma once
//Возврат 0 -> программа закрыта
//Возврат 1 -> нажата кнопка Start
//Возврат -1 -> проблема с кнопкой
int menu(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h);