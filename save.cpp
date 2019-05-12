#include <stdio.h>
#include "rooms.h"
#define FILE_NAME "save1.sav"

extern float g_humans, g_resourses, g_food;
extern int g_rooms[2][3][2];
extern int lever1_pulled, lever2_pulled;
extern float g_time_before_raid, TIMEUNTILTRAIN;
extern float g_humans_cap, g_resourses_cap, g_food_cap;
extern float g_income_food,g_income_res,g_income_hum;

//��������� ���� � ����
void save_game() {
	FILE *write_file;
	fopen_s(&write_file, FILE_NAME, "w");
	//���������� ���������� ��������
	fprintf_s(write_file, "%d %d %d\n", int(g_humans), int(g_resourses), int(g_food));

	//���������� income ���� ��������
	fprintf_s(write_file, "%.3f %.3f %.3f\n", g_income_food, g_income_res, g_income_hum);

	//���������� ������ ���������� ��������
	fprintf_s(write_file, "%d %d %d\n", int(g_humans_cap), int(g_resourses_cap), int(g_food_cap));

	/*���������� ������ � �������� � ������ �������:
	/1-�� �������/ [� ���] [� �������]
	/2-�� �������/ [� ���] [� �������]
	...
	*/
	for (int i = 0; i < COUNT_ROOMS; i++) {
		fprintf_s(write_file, "%d %d\n", g_rooms[i/3][i%3][0], g_rooms[i / 3][i % 3][1]);
	}
	
	//���������� ��������� �������
	fprintf_s(write_file, "%d %d\n", lever1_pulled, lever2_pulled);
	
	//���������� ����� �� ���������� ����� � ������
	fprintf_s(write_file, "%.1f %.1f\n", g_time_before_raid, TIMEUNTILTRAIN);

	int control_sum = (int(g_humans) + int(g_resourses) + int(g_food) + int(g_humans_cap) + int(g_resourses_cap) + int(g_food_cap) + lever1_pulled + lever2_pulled)%10;

	fprintf_s(write_file, "%d\n", control_sum);

	fclose(write_file);
}

//��������� ���� �� �����
//���������� -1, ���� ���� �� ��� ������
//���������� 0, ���� ���� ����� ������������ ��� ��������� (�� �������� �������� ����������� ������)
//���������� 1, ���� ���� ��� ������� ��������
int load_game() {
	//��������� �������� ��������� ��������
	float temp_humans, temp_resourses, temp_food, temp_humans_cap, temp_resourses_cap, temp_food_cap,
		temp_time_before_raid, temp_TIMEUNTILTRAIN,temp_income_food, temp_income_res, temp_income_hum;
	int temp_lever1_pulled, temp_lever2_pulled;
	int temp_rooms[2][3][2];

	FILE *read_file;
	fopen_s(&read_file, FILE_NAME, "r");
	if (read_file != NULL) { //���� ���� ��������
		//��������� ���������� ��������
		fscanf_s(read_file, "%f %f %f", &temp_humans, &temp_resourses, &temp_food);
		//���������� income ���� ��������
		fscanf_s(read_file, "%f %f %f\n", &temp_income_food, &temp_income_res, &temp_income_hum);
		//��������� ������ ���������� ��������
		fscanf_s(read_file, "%f %f %f", &temp_humans_cap, &temp_resourses_cap, &temp_food_cap);
		//��������� �������
		for (int i = 0; i < COUNT_ROOMS; i++) {
			fscanf_s(read_file, "%d %d", &temp_rooms[i / 3][i % 3][0], &temp_rooms[i / 3][i % 3][1]);
		}
		//��������� ��������� �������
		fscanf_s(read_file, "%d %d", &temp_lever1_pulled, &temp_lever2_pulled);
		//��������� ����� �� ���������� ����� � ������
		fscanf_s(read_file, "%f %f", &temp_time_before_raid, &temp_TIMEUNTILTRAIN);
		//��������� ����������� �����
		int read_control_sum;
		fscanf_s(read_file, "%d", &read_control_sum);
		//����������� ����������� �����, ��������� ��������� ������
		int control_sum = (int(temp_humans) + int(temp_resourses) + int(temp_food) + int(temp_humans_cap) + int(temp_resourses_cap) + int(temp_food_cap) + temp_lever1_pulled + temp_lever2_pulled) % 10;

		if (control_sum == read_control_sum) { //���� ����������� ����������� ����� ��������� � �����������
			//����������� ���������� ���������� �� ��������� �����
			g_humans = temp_humans; g_resourses = temp_resourses; g_food = temp_food;
			g_income_hum = temp_income_hum; g_income_res = temp_income_res; g_income_food = temp_income_food;
			g_humans_cap = temp_humans_cap; g_resourses_cap = temp_resourses_cap; g_food_cap = temp_food_cap;
			//g_rooms = temp_rooms;
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 3; j++) {
					for (int z = 0; z < 2; z++) {
						g_rooms[i][j][z] = temp_rooms[i][j][z];
					}
				}
			}
			lever1_pulled = temp_lever1_pulled; lever2_pulled = temp_lever2_pulled;
			g_time_before_raid = temp_time_before_raid; TIMEUNTILTRAIN = temp_TIMEUNTILTRAIN;
			fclose(read_file);
			return 1;
		}
		else { //...�����
			fclose(read_file);
			return 0; //���������� ������
		}
	}
	else {//���� ���� �� ��������
		return -1; //���������� ������
	}
}