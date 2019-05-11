#include <SDL.h>

//Возвращает номер кнопки, на которую наведён курсор, или -1, если не наведена.
int CheckIfMouseOnButton(SDL_Event event, int i, SDL_Rect buttons[]) {
	if (event.button.x >= buttons[i].x && event.button.x <= buttons[i].x + buttons[i].w &&
		event.button.y >= buttons[i].y && event.button.y <= buttons[i].y + buttons[i].h)
		return i;
	return -1;
}

//Возвращает 1, если ЛКМ нажата, и 0, если нет.
int LKMPressed(SDL_Event event) {
	if (event.button.button == SDL_BUTTON_LEFT && event.button.type == SDL_MOUSEBUTTONDOWN)
		return 1;
	return 0;
}

void DrawButtons(SDL_Renderer* renderer,SDL_Rect buttons[], SDL_Texture* start_texture, SDL_Texture* setting_texture, SDL_Texture* exit_texture) {
	SDL_RenderCopy(renderer, start_texture, NULL, &buttons[0]);
	SDL_RenderCopy(renderer, setting_texture, NULL, &buttons[1]);
	SDL_RenderCopy(renderer, exit_texture, NULL, &buttons[2]);
}

//Экран "меню".
//Возврат 0 -> программа закрыта
//Возврат 1 -> нажата кнопка Start
//Возврат -1 -> проблема с кнопкой
int menu(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h) {
	//SDL_Init(SDL_INIT_EVERYTHING);

	//Создаём ивент и переменную для отслеживания событий окна
	SDL_Event event;
	bool quit = false;

	//Задаём место расположения фона
	SDL_Rect full_screen = { 0,0,winsize_w,winsize_h };

	//Задаём места расположения кнопок (НАГЛЯДНО)
	SDL_Rect button_start = { 260,240,270,80 };
	SDL_Rect button_setting = { 260,340,270,80 };
	SDL_Rect button_exit = { 260, 440,270,80 };

	//Массив для хранения кнопок
	SDL_Rect buttons[3] = {};
	//Понятное заполнение массива:
	buttons[0] = button_start;
	buttons[1] = button_setting;
	buttons[2] = button_exit;

	//Загружаем текстуры
	SDL_Surface* menu_surf = SDL_LoadBMP("resourses/textures/menu.bmp");
	SDL_Texture* menu_texture = SDL_CreateTextureFromSurface(renderer, menu_surf);
	SDL_FreeSurface(menu_surf);
	SDL_Surface* start_surf = SDL_LoadBMP("resourses/textures/start.bmp");
	SDL_Texture* start_texture = SDL_CreateTextureFromSurface(renderer, start_surf);
	SDL_FreeSurface(start_surf);
	SDL_Surface* setting_surf = SDL_LoadBMP("resourses/textures/setting.bmp");
	SDL_Texture* setting_texture = SDL_CreateTextureFromSurface(renderer, setting_surf);
	SDL_FreeSurface(setting_surf);
	SDL_Surface* exit_surf = SDL_LoadBMP("resourses/textures/exit.bmp");
	SDL_Texture* exit_texture = SDL_CreateTextureFromSurface(renderer, exit_surf);
	SDL_FreeSurface(exit_surf);
	SDL_Surface* start_surf_click = SDL_LoadBMP("resourses/textures/start_selected.bmp");
	SDL_Texture* start_texture_click = SDL_CreateTextureFromSurface(renderer, start_surf_click);
	SDL_FreeSurface(start_surf_click);
	SDL_Surface* setting_surf_click = SDL_LoadBMP("resourses/textures/setting_selected.bmp");
	SDL_Texture* setting_texture_click = SDL_CreateTextureFromSurface(renderer, setting_surf_click);
	SDL_FreeSurface(setting_surf_click);
	SDL_Surface* exit_surf_click = SDL_LoadBMP("resourses/textures/exit_selected.bmp");
	SDL_Texture* exit_texture_click = SDL_CreateTextureFromSurface(renderer, exit_surf_click);
	SDL_FreeSurface(exit_surf_click);

	//Отрисовываем кнопки и фон
	SDL_RenderCopy(renderer, menu_texture, NULL, &full_screen);
	DrawButtons(renderer, buttons, start_texture, setting_texture, exit_texture);
	SDL_RenderPresent(renderer);

	//Переменная для хранения номера кнопки
	int button_flag;
	//Переменная, которая равна 1, когда необходимо заменить кнопки с нажатых на обычные
	int buttons_changed=0;
	//Переменная, для хранения состояния игры: 1 - нажата кнопка Start
	int game_started = 0;

	//Основной цикл
	while (!quit && !game_started) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			quit = 1;
			return 0; //Возврат 0 -> программа закрыта
		}

		//Каждый раз в цикле говорим, что мышь не наведена ни на одну из кнопок
		button_flag = -1;

		//Цикл, проверяющий, наведена ли мышь на кнопку
		for (int i = 0; i <= 2; i++) {
			button_flag = CheckIfMouseOnButton(event, i, buttons);
			if (button_flag >= 0) {
				switch (i)
				{
					//Курсор на кнопке Start?
				case 0:SDL_RenderCopy(renderer, start_texture_click, NULL, &button_start);
					 //Нажата кнопка Start?
					if (LKMPressed(event))
						game_started = 1;
					break;
					//Курсор на кнопке Settings?
				case 1: SDL_RenderCopy(renderer, setting_texture_click, NULL, &button_setting);
					break;
					//Нажата кнопка Exit?
				case 2: SDL_RenderCopy(renderer, exit_texture_click, NULL, &button_exit);
					//Нажата кнопка Exit?
					if (LKMPressed(event))
						quit = 1;
					break;
				default:
					return -1; //Возврат -1 -> проблема с кнопкой
				}
				SDL_RenderPresent(renderer);
				buttons_changed = 1;
				break;
			}
		}

		//Если не наведено ни на какую из кнопок - отрисовать кнопки в начальное положение
		if (button_flag == -1 && buttons_changed == 1) {
			DrawButtons(renderer, buttons, start_texture, setting_texture, exit_texture);
			SDL_RenderPresent(renderer);
			buttons_changed = 0;
		}

		SDL_Delay(10);
	}

	//Очищаем память
	SDL_DestroyTexture(menu_texture);
	SDL_DestroyTexture(start_texture);
	SDL_DestroyTexture(setting_texture);
	SDL_DestroyTexture(exit_texture);
	SDL_DestroyTexture(start_texture_click);
	SDL_DestroyTexture(setting_texture_click);
	SDL_DestroyTexture(exit_texture_click);
	if (game_started) return 1; //Возврат 1 - нажата кнопка Start
	return 0; //Возврат 0 -> программа закрыта
}