#pragma once
//Рисует текст text в окне window и на рендерере renderer в точке point.
void draw_text(SDL_Window* window, SDL_Renderer* renderer, char* text, SDL_Rect rect);