
#pragma once


struct Room_info {
	char *text_info = new char[256];
};

const int MAX_ROOM_LEVEL = 3; //Максимальный уровень комнаты
const int COUNT_ROOMS = 6; //Максимальное количество комнат

//Инициализирует доход от комнат
void initialize_rooms();