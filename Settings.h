#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

int get_settings_of_file();
int set_settings_in_file();

int launch_settings(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h);