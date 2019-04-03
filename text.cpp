#include <SDL_ttf.h>

//–исует текст text в окне window и на рендерере renderer в точке point.
void draw_text(SDL_Window* window, SDL_Renderer* &renderer, char* text, SDL_Point point, TTF_Font* font) {
	TTF_Init();
	TTF_Font* my_font = TTF_OpenFont("Text.ttf", 100);
	SDL_Color fore_color = { 255,255,100 };
	SDL_Color back_color = { 10, 10, 50 };
	SDL_Surface* textSurface = NULL;
	textSurface = TTF_RenderText_Shaded(my_font, text, fore_color, back_color);
	SDL_Rect rect = { point.x - 15,point.y,30,30 };
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(my_font);
	TTF_Quit();
}