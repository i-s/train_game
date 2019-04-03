#pragma once
//Экран "поезд".
//Возврат -1 -> выход из программы с ошибкой (не нажата кнопка город)
//Возврат 0 -> была нажата кнопка город
int train_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h);