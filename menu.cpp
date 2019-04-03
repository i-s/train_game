#include <SDL.h>

/*To-do:
V ������� ���������� button-�� �� ���������� ��� ������� (������ � ��������� ����)
1) ���������� �� �������� � DrawButtons
2) ����������� ��������� ������ �� ������� ����
3) �������� ���� ��� ������
4) ���� ��������?
*/

//���������� ����� ������, �� ������� ������ ������, ��� -1, ���� �� ��������.
int CheckIfMouseOnButton(SDL_Event event, int i, SDL_Rect buttons[]) {
	if (event.button.x >= buttons[i].x && event.button.x <= buttons[i].x + buttons[i].w &&
		event.button.y >= buttons[i].y && event.button.y <= buttons[i].y + buttons[i].h)
		return i;
	return -1;
}

//���������� 1, ���� ��� ������, � 0, ���� ���.
int LKMPressed(SDL_Event event) {
	if (event.button.button == SDL_BUTTON_LEFT)
		return 1;
	return 0;
}

void DrawButtons(SDL_Renderer* renderer,SDL_Rect buttons[], SDL_Texture* start_texture, SDL_Texture* setting_texture, SDL_Texture* exit_texture) {
	SDL_RenderCopy(renderer, start_texture, NULL, &buttons[0]);
	SDL_RenderCopy(renderer, setting_texture, NULL, &buttons[1]);
	SDL_RenderCopy(renderer, exit_texture, NULL, &buttons[2]);
}

//������� 0 -> ��������� �������
//������� 1 -> ������ ������ Start
//������� -1 -> �������� � �������
int menu(SDL_Window* window, SDL_Renderer* renderer, int winsize_w, int winsize_h) {
	//SDL_Init(SDL_INIT_EVERYTHING);

	//������ ����� � ���������� ��� ������������ ������� ����
	SDL_Event event;
	bool quit = false;

	//����� ����� ������������ ����
	SDL_Rect full_screen = { 0,0,winsize_w,winsize_h };

	//����� ����� ������������ ������ (��������)
	SDL_Rect button_start = { 260,240,270,80 };
	SDL_Rect button_setting = { 260,340,270,80 };
	SDL_Rect button_exit = { 260, 440,270,80 };

	//������ ��� �������� ������
	SDL_Rect buttons[3] = {};
	//�������� ���������� �������:
	buttons[0] = button_start;
	buttons[1] = button_setting;
	buttons[2] = button_exit;

	//��������� ��������
	SDL_Surface* menu_surf = SDL_LoadBMP("textures/menu.bmp");
	SDL_Texture* menu_texture = SDL_CreateTextureFromSurface(renderer, menu_surf);
	SDL_FreeSurface(menu_surf);
	SDL_Surface* start_surf = SDL_LoadBMP("textures/start.bmp");
	SDL_Texture* start_texture = SDL_CreateTextureFromSurface(renderer, start_surf);
	SDL_FreeSurface(start_surf);
	SDL_Surface* setting_surf = SDL_LoadBMP("textures/setting.bmp");
	SDL_Texture* setting_texture = SDL_CreateTextureFromSurface(renderer, setting_surf);
	SDL_FreeSurface(setting_surf);
	SDL_Surface* exit_surf = SDL_LoadBMP("textures/exit.bmp");
	SDL_Texture* exit_texture = SDL_CreateTextureFromSurface(renderer, exit_surf);
	SDL_FreeSurface(exit_surf);
	SDL_Surface* start_surf_click = SDL_LoadBMP("textures/start_selected.bmp");
	SDL_Texture* start_texture_click = SDL_CreateTextureFromSurface(renderer, start_surf_click);
	SDL_FreeSurface(start_surf_click);
	SDL_Surface* setting_surf_click = SDL_LoadBMP("textures/setting_selected.bmp");
	SDL_Texture* setting_texture_click = SDL_CreateTextureFromSurface(renderer, setting_surf_click);
	SDL_FreeSurface(setting_surf_click);
	SDL_Surface* exit_surf_click = SDL_LoadBMP("textures/exit_selected.bmp");
	SDL_Texture* exit_texture_click = SDL_CreateTextureFromSurface(renderer, exit_surf_click);
	SDL_FreeSurface(exit_surf_click);

	//������������ ������ � ���
	SDL_RenderCopy(renderer, menu_texture, NULL, &full_screen);
	DrawButtons(renderer, buttons, start_texture, setting_texture, exit_texture);
	SDL_RenderPresent(renderer);

	//���������� ��� �������� ������ ������
	int button_flag;
	//����������, ������� ����� 1, ����� ���������� �������� ������ � ������� �� �������
	int buttons_changed=0;
	//����������, ��� �������� ��������� ����: 1 - ������ ������ Start
	int game_started = 0;

	//�������� ����
	while (!quit && !game_started) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
			quit = 1;
			return 0; //������� 0 -> ��������� �������
		}

		//������ ��� � ����� �������, ��� ���� �� �������� �� �� ���� �� ������
		button_flag = -1;

		//����, �����������, �������� �� ���� �� ������
		for (int i = 0; i <= 2; i++) {
			button_flag = CheckIfMouseOnButton(event, i, buttons);
			if (button_flag >= 0) {
				switch (i)
				{
					//������ �� ������ Start?
				case 0:SDL_RenderCopy(renderer, start_texture_click, NULL, &button_start);
					 //������ ������ Start?
					if (LKMPressed(event))
						game_started = 1;
					break;
					//������ �� ������ Settings?
				case 1: SDL_RenderCopy(renderer, setting_texture_click, NULL, &button_setting);
					break;
					//������ ������ Exit?
				case 2: SDL_RenderCopy(renderer, exit_texture_click, NULL, &button_exit);
					//������ ������ Exit?
					if (LKMPressed(event))
						quit = 1;
					break;
				default:
					return -1; //������� -1 -> �������� � �������
				}
				SDL_RenderPresent(renderer);
				buttons_changed = 1;
				break;
			}
		}

		//���� �� �������� �� �� ����� �� ������ - ���������� ������ � ��������� ���������
		if (button_flag == -1 && buttons_changed == 1) {
			DrawButtons(renderer, buttons, start_texture, setting_texture, exit_texture);
			SDL_RenderPresent(renderer);
			buttons_changed = 0;
		}

		SDL_Delay(10);
	}

	//������� ������
	SDL_DestroyTexture(menu_texture);
	SDL_DestroyTexture(start_texture);
	SDL_DestroyTexture(setting_texture);
	SDL_DestroyTexture(exit_texture);
	SDL_DestroyTexture(start_texture_click);
	SDL_DestroyTexture(setting_texture_click);
	SDL_DestroyTexture(exit_texture_click);
	if (game_started) return 1; //������� 1 - ������ ������ Start
	return 0; //������� 0 -> ��������� �������
}