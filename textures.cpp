/*
#include <SDL.h>
#include "town.h"

//Загружает текстуру в память

void LoadTexture(SDL_Renderer* renderer, Texture texture,const char name[],const char path[], bool chroma_key) {
	SDL_Surface* surf = SDL_LoadBMP(path);
	if (chroma_key) { SDL_SetColorKey(surf, 1, SDL_MapRGB(surf->format, 0, 255, 0)); }
	texture.name = name;
	texture.texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
}

//Выгружает текстуру из памяти
void UnloadTexture(Texture texture) {
	SDL_DestroyTexture(texture.texture);
}
*/