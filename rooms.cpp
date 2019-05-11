#define MAX_ROOM_LEVEL 3
#define COUNT_ROOMS 6

//доход от комнаты [тип][уровень][0.hum,1.food,2.res]
float income_from_rooms[COUNT_ROOMS][MAX_ROOM_LEVEL][3];

//цена комнаты [тип][уровень][0.minhum,1.food,2.res]
int cost_rooms[COUNT_ROOMS][MAX_ROOM_LEVEL][3];

//изменение комнатой ограничений [тип][уровень][0.maxhum,1.maxfood,2.maxres]
int change_cap_rooms[COUNT_ROOMS][MAX_ROOM_LEVEL][3];

//Инициализирует массивы для создания комнат
void initialize_rooms() {
	//инициализация дохода
	{
		//ферма
		{
			income_from_rooms[0][0][0] = 0;
			income_from_rooms[0][0][1] = 1;
			income_from_rooms[0][0][2] = 0;

			income_from_rooms[0][1][0] = 0;
			income_from_rooms[0][1][1] = 2.5;
			income_from_rooms[0][1][2] = 0;

			income_from_rooms[0][2][0] = 0;
			income_from_rooms[0][2][1] = 7.5;
			income_from_rooms[0][2][2] = 0;
		}
		//фабрика
		{
			income_from_rooms[1][0][0] = 0;
			income_from_rooms[1][0][1] = 0;
			income_from_rooms[1][0][2] = 1;

			income_from_rooms[1][1][0] = 0;
			income_from_rooms[1][1][1] = 0;
			income_from_rooms[1][1][2] = 2.45;

			income_from_rooms[1][2][0] = 0;
			income_from_rooms[1][2][1] = 0;
			income_from_rooms[1][2][2] = 4;
		}
		//Жилая комната
		{
			income_from_rooms[2][0][0] = 0.001;
			income_from_rooms[2][0][1] = 0;
			income_from_rooms[2][0][2] = 0;

			income_from_rooms[2][1][0] = 0.005;
			income_from_rooms[2][1][1] = 0;
			income_from_rooms[2][1][2] = 0;

			income_from_rooms[2][2][0] = 0.01;
			income_from_rooms[2][2][1] = 0;
			income_from_rooms[2][2][2] = 0;
		}
		//Оружейная
		{
			income_from_rooms[3][0][0] = 0;
			income_from_rooms[3][0][1] = 0;
			income_from_rooms[3][0][2] = -0.05;

			income_from_rooms[3][1][0] = 0;
			income_from_rooms[3][1][1] = 0;
			income_from_rooms[3][1][2] = -0.15;

			income_from_rooms[3][2][0] = 0;
			income_from_rooms[3][2][1] = 0;
			income_from_rooms[3][2][2] = -0.35;
		}
		//Cклад
		{
			income_from_rooms[4][0][0] = 0;
			income_from_rooms[4][0][1] = 0;
			income_from_rooms[4][0][2] = -0.05;

			income_from_rooms[4][1][0] = 0;
			income_from_rooms[4][1][1] = 0;
			income_from_rooms[4][1][2] = -0.15;

			income_from_rooms[4][2][0] = 0;
			income_from_rooms[4][2][1] = 0;
			income_from_rooms[4][2][2] = -0.35;
		}
		//Радио
		{
			income_from_rooms[5][0][0] = 0;
			income_from_rooms[5][0][1] = 0;
			income_from_rooms[5][0][2] = -0.05;

			income_from_rooms[5][1][0] = 0;
			income_from_rooms[5][1][1] = 0;
			income_from_rooms[5][1][2] = -0.15;

			income_from_rooms[5][2][0] = 0;
			income_from_rooms[5][2][1] = 0;
			income_from_rooms[5][2][2] = -0.35;
		}
	}
	//инициализация цены
	{
		//ферма
		{
			cost_rooms[0][0][0] = 5;
			cost_rooms[0][0][1] = 10;
			cost_rooms[0][0][2] = 1;

			cost_rooms[0][1][0] = 50;
			cost_rooms[0][1][1] = 50;
			cost_rooms[0][1][2] = 3;

			cost_rooms[0][2][0] = 75;
			cost_rooms[0][2][1] = 100;
			cost_rooms[0][2][2] = 6;
		}
		//фабрика
		{
			cost_rooms[1][0][0] = 20;
			cost_rooms[1][0][1] = 50;
			cost_rooms[1][0][2] = 10;

			cost_rooms[1][1][0] = 40;
			cost_rooms[1][1][1] = 75;
			cost_rooms[1][1][2] = 20;

			cost_rooms[1][2][0] = 50;
			cost_rooms[1][2][1] = 100;
			cost_rooms[1][2][2] = 30;
		}
		//Жилая комната
		{
			cost_rooms[2][0][0] = 50;
			cost_rooms[2][0][1] = 100;
			cost_rooms[2][0][2] = 35;

			cost_rooms[2][1][0] = 100;
			cost_rooms[2][1][1] = 200;
			cost_rooms[2][1][2] = 50;

			cost_rooms[2][2][0] = 150;
			cost_rooms[2][2][1] = 250;
			cost_rooms[2][2][2] = 80;
		}
		//Оружейная
		{
			cost_rooms[3][0][0] = 25;
			cost_rooms[3][0][1] = 50;
			cost_rooms[3][0][2] = 50;

			cost_rooms[3][1][0] = 75;
			cost_rooms[3][1][1] = 50;
			cost_rooms[3][1][2] = 200;

			cost_rooms[3][2][0] = 125;
			cost_rooms[3][2][1] = 100;
			cost_rooms[3][2][2] = 300;
		}
		//склад
		{
			cost_rooms[4][0][0] = 25;
			cost_rooms[4][0][1] = 50;
			cost_rooms[4][0][2] = 25;

			cost_rooms[4][1][0] = 75;
			cost_rooms[4][1][1] = 50;
			cost_rooms[4][1][2] = 50;

			cost_rooms[4][2][0] = 125;
			cost_rooms[4][2][1] = 100;
			cost_rooms[4][2][2] = 75;
		}
		//радио
		{
			cost_rooms[5][0][0] = 25;
			cost_rooms[5][0][1] = 50;
			cost_rooms[5][0][2] = 150;

			cost_rooms[5][1][0] = 75;
			cost_rooms[5][1][1] = 50;
			cost_rooms[5][1][2] = 200;

			cost_rooms[5][2][0] = 125;
			cost_rooms[5][2][1] = 100;
			cost_rooms[5][2][2] = 300;
		}
	}
	//инициализация изменений ограничений
	{
		//ферма
		{
			change_cap_rooms[0][0][0] = 0;
			change_cap_rooms[0][0][1] = 0;
			change_cap_rooms[0][0][2] = 0;

			change_cap_rooms[0][1][0] = 0;
			change_cap_rooms[0][1][1] = 0;
			change_cap_rooms[0][1][2] = 0;

			change_cap_rooms[0][2][0] = 0;
			change_cap_rooms[0][2][1] = 0;
			change_cap_rooms[0][2][2] = 0;
		}
		//фабрика
		{
			change_cap_rooms[1][0][0] = 0;
			change_cap_rooms[1][0][1] = 0;
			change_cap_rooms[1][0][2] = 0;

			change_cap_rooms[1][1][0] = 0;
			change_cap_rooms[1][1][1] = 0;
			change_cap_rooms[1][1][2] = 0;

			change_cap_rooms[1][2][0] = 0;
			change_cap_rooms[1][2][1] = 0;
			change_cap_rooms[1][2][2] = 0;
		}
		//жилая комната
		{
			change_cap_rooms[2][0][0] = 20;
			change_cap_rooms[2][0][1] = 0;
			change_cap_rooms[2][0][2] = 0;

			change_cap_rooms[2][1][0] = 50;
			change_cap_rooms[2][1][1] = 0;
			change_cap_rooms[2][1][2] = 0;

			change_cap_rooms[2][2][0] = 100;
			change_cap_rooms[2][2][1] = 0;
			change_cap_rooms[2][2][2] = 0;
		}
		//оружейная
		{
			change_cap_rooms[3][0][0] = 00;
			change_cap_rooms[3][0][1] = 0;
			change_cap_rooms[3][0][2] = 0;

			change_cap_rooms[3][1][0] = 00;
			change_cap_rooms[3][1][1] = 0;
			change_cap_rooms[3][1][2] = 0;

			change_cap_rooms[3][2][0] = 0;
			change_cap_rooms[3][2][1] = 0;
			change_cap_rooms[3][2][2] = 0;
		}
		//склад
		{
			change_cap_rooms[4][0][0] = 0;
			change_cap_rooms[4][0][1] = 50;
			change_cap_rooms[4][0][2] = 50;

			change_cap_rooms[4][1][0] = 0;
			change_cap_rooms[4][1][1] = 150;
			change_cap_rooms[4][1][2] = 110;

			change_cap_rooms[4][2][0] = 0;
			change_cap_rooms[4][2][1] = 250;
			change_cap_rooms[4][2][2] = 175;
		}
		//радио
		{
			change_cap_rooms[5][0][0] = 0;
			change_cap_rooms[5][0][1] = 0;
			change_cap_rooms[5][0][2] = 0;

			change_cap_rooms[5][1][0] = 0;
			change_cap_rooms[5][1][1] = 0;
			change_cap_rooms[5][1][2] = 0;

			change_cap_rooms[5][2][0] = 0;
			change_cap_rooms[5][2][1] = 0;
			change_cap_rooms[5][2][2] = 0;
		}

	}
}