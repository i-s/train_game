#pragma once
//Экран "сражение". Если передать custom_difficulty != -1 ,то для ЭТОГО сражения будет использована специальная сложность
//Возврат 0 -> окно закрыто
int battle_game(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h, int custom_difficulty = -1);