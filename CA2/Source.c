#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "admin.h"
//#include "user.h"

#define USERS_DATA_LOCATION "users.txt"
#define HALL_DATA "saloons.txt"
#define FOOD_DATA "food.txt"

#pragma warning(disable:4996)

typedef struct users_data {
	char *username;
	int student_id;
	char *password;
	int money;
	struct users_data*next;

}users_data;

typedef struct food {
	char* name;
	int food_id;
	int price;
	int capacity;
	int hall_id;
	int day;
	struct food*next;
}food;

typedef struct hall {
	char*name;
	int hall_id;
	int capacity;
	struct hall*next;
}hall;

#define COUPON_DATA "coupon.txt"

typedef struct copoun {
	int copoun_id;
	int percentage;
	int capacity;
	struct copoun*next;
}copoun;

#define FOOD_DATA_FOR_REPORT "food_report.txt"
#define HALL_DATA_FOR_REPORT "hall_report.txt"

typedef struct information {
	int day;
	int money;
	int food_id;
	int studentd_id_login;
	struct information*next;
}information;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void add_hall_func(hall*head_hall_for_user_menu) {
	int checker = 0;
	FILE*temp_file = fopen(HALL_DATA, "a+");
	hall* entered_hall_data = (hall*)calloc(1, sizeof(hall));
	int i; char ch;

	for (i = 0; (ch = getchar()) != ' '; i++) {
		entered_hall_data->name = (char*)realloc(entered_hall_data->name, (i + 1) * sizeof(char));
		entered_hall_data->name[i] = ch;
	}
	entered_hall_data->name[i] = '\0';

	scanf("%d", &entered_hall_data->hall_id); getchar();
	scanf("%d", &entered_hall_data->capacity); getchar();

	hall *temp_data = (hall*)calloc(1, sizeof(hall));
	temp_data = head_hall_for_user_menu;
	temp_data = temp_data->next;
	while (temp_data != NULL) {

		if (entered_hall_data->hall_id == temp_data->hall_id) {
			checker = 1;
		}
		temp_data = temp_data->next;
	}
	if (checker == 1) printf("hall id is not unique!\n");
	if (checker == 0) { 
		fprintf(temp_file, "%s %d %d\n", entered_hall_data->name, entered_hall_data->hall_id, entered_hall_data->capacity);
		printf("done(aded)\n");

		hall *temp7 = (hall*)calloc(1, sizeof(hall));
		hall *ptr;
		temp7 = head_hall_for_user_menu;
		while (1) {
			if (temp7->next == NULL) {
				temp7->next = entered_hall_data;
				ptr = temp7->next;
				ptr->next = NULL;
				break;
			}
			else { temp7 = temp7->next; }
		}

	}
	fclose(temp_file);
}

void add_food_func(food*head_for_menu_of_user, hall*head_hall_for_user_menu) {
	
	//////////////////vorodi gereftan
	food* entered_food_data = (food*)calloc(1, sizeof(food));
	int i; char ch;
	for (i = 0; (ch = getchar()) != ' '; i++) {
		entered_food_data->name = (char*)realloc(entered_food_data->name, (i + 1) * sizeof(char));
		entered_food_data->name[i] = ch;
	}
	entered_food_data->name[i] = '\0';
	scanf("%d", &entered_food_data->food_id); getchar();
	scanf("%d", &entered_food_data->price); getchar();
	scanf("%d", &entered_food_data->capacity); getchar();
	scanf("%d", &entered_food_data->hall_id); getchar();
	scanf("%d", &entered_food_data->day); getchar();
	////////////////

	int checker = 0;
	int yy = 0;

	hall*temp_hall1 = head_hall_for_user_menu;
	temp_hall1 = temp_hall1->next;
	//login admin admin
	//add_food kebab 1 2 3 3 5
	while (temp_hall1 != NULL) {
		//printf("%d\n", temp_hall1->hall_id);
		if (entered_food_data->hall_id == temp_hall1->hall_id) {
			checker = 1;
		}
		temp_hall1 = temp_hall1->next;
	}
	int no_print_food_eror = 0;
	if (checker == 0) { printf("wrong hall id\n"); no_print_food_eror = 1; }
	//'''''''''''''''''''''''''''''''''''''''
	int checker_two = 0;
	food *temp_data = (food*)calloc(1, sizeof(food));
	temp_data = head_for_menu_of_user;
	temp_data = temp_data->next;
	while (temp_data != NULL) {

		if (entered_food_data->food_id == temp_data->food_id) {
			checker_two = 1;
		}
		temp_data = temp_data->next;
	}
	if (no_print_food_eror == 0) { if (checker_two == 1) printf("food id is not unique\n"); }
	if (checker_two == 0 && checker == 1) {
		FILE*temp_file_food = fopen(FOOD_DATA, "a+");
		printf("food aded succesfuly\n");
		fprintf(temp_file_food, "%s %d %d %d %d %d\n", entered_food_data->name, entered_food_data->food_id, entered_food_data->price, entered_food_data->capacity, entered_food_data->hall_id, entered_food_data->day);
		fclose(temp_file_food);

		food *temp7 = (food*)calloc(1, sizeof(food));
		food *ptr;
		temp7 = head_for_menu_of_user;
		while (1) {
			if (temp7->next == NULL) {
				temp7->next = entered_food_data;
				ptr = temp7->next;
				ptr->next = NULL;
				break;
			}
			else { temp7 = temp7->next; }
		}






	}
}

void add_coupon_func(copoun* head_for_coupon) {
	FILE*temp_file_for_coupon = fopen(COUPON_DATA, "a+");

	copoun* entered_coupon_data = (copoun*)calloc(1, sizeof(copoun));
	scanf("%d %d %d", &entered_coupon_data->copoun_id, &entered_coupon_data->percentage, &entered_coupon_data->capacity);

	copoun* temp_data = (copoun*)calloc(1, sizeof(copoun));
	temp_data = head_for_coupon;
	temp_data = temp_data->next;
	int checker = 0;
	while (temp_data != NULL) {

		if (entered_coupon_data->copoun_id == temp_data->copoun_id) { checker = 1; }
		temp_data = temp_data->next;
	}

	if (checker == 1) printf("copoun already exist\n");
	if (checker == 0) {
		fprintf(temp_file_for_coupon, "%d %d %d\n", entered_coupon_data->copoun_id, entered_coupon_data->percentage, entered_coupon_data->capacity); printf("done\n");

		copoun *temp7 = (copoun*)calloc(1, sizeof(copoun));
		copoun *ptr;
		temp7 = head_for_coupon;
		while (1) {
			if (temp7->next == NULL) {
				temp7->next = entered_coupon_data;
				ptr = temp7->next;
				ptr->next = NULL;
				break;
			}
			else { temp7 = temp7->next; }
		}

	}
	fclose(temp_file_for_coupon);
}

int food_capacity_by_food_id(int food_id, food* head_food) {
	food*temp = head_food;
	temp = temp->next;
	while (temp != NULL) {
		if (temp->food_id == food_id) {
			return temp->capacity;
		}
		temp = temp->next;
	}
	return 0;
}

int number_of_food_reserved_and_caculate_profit(int food_id, int*profit_of_this_food, information*head_of_iformation) {
	int number_reserved = 0;
	information*temp = head_of_iformation;
	temp = temp->next;
	while (temp != NULL) {
		if (temp->food_id == food_id) {
			number_reserved++;
			*profit_of_this_food += temp->money;
		}

		temp = temp->next;
	}
	return number_reserved;
}

int is_user_printed(int*arr_of_printed_users, int food_id) {
	for (int i = 0; i < sizeof(arr_of_printed_users) / sizeof(int); i++) {
		if (arr_of_printed_users[i] == food_id) {
			return 1;
		}
	}
	return 0;
}

void food_report_func(users_data*head, food*head_for_menu_of_user, hall*head_hall_for_user_menu, copoun* head_for_coupon, information* head_of_iformation) {
	FILE*temp_file = fopen(FOOD_DATA_FOR_REPORT, "w");
	food*temp1 = head_for_menu_of_user;
	temp1 = temp1->next;
	char print_file[26] = "there is no food available";
	
	information* temp_money = head_of_iformation;
	temp_money = temp_money->next;

	information* temp_pp = head_of_iformation;
	temp_pp = temp_pp->next;

	information*temp_for_prft = head_of_iformation;
	temp_for_prft = temp_for_prft->next;

	int p_rofit;


	int b;
	if (temp1 == NULL) { 
		printf("done\n");	
		fwrite(print_file, sizeof(char), 26, temp_file);
	}
	int profit_of_this_food = 0;
	int checker = 0;
	if (temp1 != NULL) {
	

		int* arr_of_printed_users = (int*)calloc(1, sizeof(int));
		for (int i = 0; temp_money != NULL; i++) {
			profit_of_this_food = 0;
			b = is_user_printed(arr_of_printed_users, temp_money->food_id);
			
			if (b == 0) {
				fprintf(temp_file,"%d %d %d", temp_money->food_id, food_capacity_by_food_id(temp_money->food_id, head_for_menu_of_user), number_of_food_reserved_and_caculate_profit(temp_money->food_id, &profit_of_this_food, head_of_iformation));
				p_rofit = profit_of_this_food;
				fprintf(temp_file, " %d\n", profit_of_this_food);
				arr_of_printed_users = (int*)realloc(arr_of_printed_users, (i + 1) * sizeof(int));
				arr_of_printed_users[i] = temp_money->food_id;

			}
			temp_money = temp_money->next;
		}
		//fclose(temp_file);
		printf("done!\n");
	
		int profit_no_reserve = 0;
		int no_reserve = 0;
		int u = 0;
		while (temp1) {
			while (temp_pp) {
				if (temp1->food_id == temp_pp->food_id) u = 1;
				temp_pp = temp_pp->next;
			}
			if (u == 0) fprintf(temp_file, "%d %d %d %d\n",temp1->food_id,temp1->capacity,profit_no_reserve,no_reserve);
				u = 0;
			temp_pp = head_of_iformation;
			temp_pp = temp_pp->next;
			temp1 = temp1->next;
		}
		
	}

	fclose(temp_file);
	
}

int find_hall_id_food(int hall_id,int day, information* head_of_iformation, food*head_for_menu_of_user,int*profit_hall) {
	information*check = head_of_iformation;
	check = check->next;

	int a = 0;
	//int profit_this_hall = 0;
	food*check_two = head_for_menu_of_user;
	check_two = check_two->next;

	while (check_two) {
		if (check_two->hall_id == hall_id) {
			while (check) {
				if (check->day == day && check->food_id== check_two->food_id) {
					*profit_hall += check->money; a++;
				}

				check = check->next;
			}
		}

		check_two = check_two->next;
	}
	return a;
}

void hall_report_func(users_data*head, food*head_for_menu_of_user, hall*head_hall_for_user_menu, copoun* head_for_coupon, information* head_of_iformation) {
	FILE*temp_file = fopen(HALL_DATA_FOR_REPORT, "w");
	hall*temp1 = head_hall_for_user_menu;
	temp1 = temp1->next;
	char print_file[16] = "there is no hall";

	int profit_this_hall = 0;
	int prft = 0;

	hall* temp_pp = head_hall_for_user_menu;
	temp_pp = temp_pp->next;

	int c;

	if (temp1 == NULL) {
		printf("done\n");
		fwrite(print_file, sizeof(char), 16, temp_file);
	}

	if (temp1 != NULL) {
		printf("done\n");
		while (temp_pp) {
			printf("%d %s %d\n", temp_pp->hall_id, temp_pp->name, temp_pp->capacity);
			fprintf(temp_file,"%d %s %d\n",temp_pp->hall_id, temp_pp->name, temp_pp->capacity);
			for (int i = 1; i < 8; i++) {
				c = find_hall_id_food(temp_pp->hall_id, i, head_of_iformation, head_for_menu_of_user, &profit_this_hall);
				prft = profit_this_hall;
				printf("%d %d %d\n", i, c, prft);
				fprintf(temp_file, "%d %d %d\n", i, c, prft);
				//c = find_hall_id_food(temp_pp->hall_id, i, head_of_iformation, head_for_menu_of_user,&profit_this_hall);
				profit_this_hall = 0;
			}
			profit_this_hall = 0;
			temp_pp = temp_pp->next;
		}
	}
	fclose(temp_file);
}

void admin_stuff(users_data*head, food*head_for_menu_of_user, hall*head_hall_for_user_menu, copoun* head_for_coupon, information* head_of_iformation) {



	char* kbhit; char ch; int i;
	while (1) {
		kbhit = (char*)calloc(1, sizeof(char));
		for (i = 0; ((ch = getchar()) != ' ') && (ch != '\n'); i++) {

			kbhit = (char*)realloc(kbhit, (i + 1) * sizeof(char));
			kbhit[i] = ch;
		}
		kbhit[i] = '\0';

		if (strcmp(kbhit, "logout") == 0) { printf("done\n"); return; }
		else if (strcmp(kbhit, "add_hall") == 0) add_hall_func(head_hall_for_user_menu);
		else if (strcmp(kbhit, "add_food") == 0) add_food_func(head_for_menu_of_user, head_hall_for_user_menu);
		else if (strcmp(kbhit, "add_coupon") == 0) add_coupon_func(head_for_coupon);
		else if (strcmp(kbhit, "exit") == 0) { exit(0); }
		else if (strcmp(kbhit, "food_report") == 0) food_report_func(head, head_for_menu_of_user, head_hall_for_user_menu, head_for_coupon, head_of_iformation);
		else if (strcmp(kbhit, "hall_report") == 0) hall_report_func(head, head_for_menu_of_user, head_hall_for_user_menu, head_for_coupon, head_of_iformation);
		else { printf("Invalid command!\n"); fseek(stdin, 0, SEEK_CUR); }
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void show_reserved_func(users_data* temp_data, food* head_for_menu_of_user, users_data*head, hall* head_hall_for_user_menu, copoun* head_for_coupon, information* head_of_iformation) {

	information* ptr = head_of_iformation;
	ptr = ptr->next;
	
	food* ptr_two = head_for_menu_of_user;
	ptr_two = ptr_two->next;

	hall* temp = head_hall_for_user_menu;
	temp = temp->next;

	
		for (int i = 1; i < 8; i++) {
		printf("%d:\n", i);

		while (ptr) {
			if (ptr->studentd_id_login == temp_data->student_id) {
				if (ptr->day == i) {
					while (ptr_two) {
						if (ptr_two->food_id == ptr->food_id) {
							while (temp) {
								if (ptr_two->hall_id == temp->hall_id) printf("%s %d %d %s\n", ptr_two->name, ptr->money, ptr->food_id, temp->name);
								temp = temp->next;
							}
							temp = head_hall_for_user_menu;
							temp = temp->next;
						}
						ptr_two = ptr_two->next;
					}
					ptr_two = head_for_menu_of_user;
					ptr_two = ptr_two->next;
				}
			}
			ptr = ptr->next;
		}
		temp = head_hall_for_user_menu;
		temp = temp->next;
		ptr = head_of_iformation;
		ptr = ptr->next;
		ptr_two = head_for_menu_of_user;
		ptr_two = ptr_two->next;
	}
}

void reserve_discount_food_func(users_data* temp_data, food* head_for_menu_of_user, users_data*head, hall* head_hall_for_user_menu, copoun* head_for_coupon, information* head_of_iformation) {
	users_data*ptr = head;
	ptr = ptr->next;

	hall*ptr_two = head_hall_for_user_menu;
	ptr_two = ptr_two->next;

	food*temp_food = head_for_menu_of_user;
	temp_food = temp_food->next;

	copoun*temp_coupon = head_for_coupon;
	temp_coupon = temp_coupon->next;

	information *temp3 = (information*)calloc(1, sizeof(information));
	temp3->studentd_id_login = temp_data->student_id;


	int n; int cpn_cde;
	scanf("%d %d", &n, &cpn_cde);
	//printf("%d %d\n", n, cpn_cde);
	int checker = 0;
	int checker_2 = 0;
	int checker_3 = 0;
	int checker_4 = 0;
	int checker_5 = 0;
	int y=0; int food_prce=0; int per_money=0; int cpty=0;
	while (temp_food) {
		if (temp_food->food_id == n) {
			y = temp_food->hall_id;
			food_prce = temp_food->price;
			cpty = temp_food->capacity;
			checker = 1;
			if (temp_food->capacity < 1) checker_2 = 1;
			break;

		}
		temp_food = temp_food->next;
	}


	int hall_cpty=0;
	while (ptr_two) {

		if (ptr_two->hall_id == y)
		{
			hall_cpty = ptr_two->capacity;
			if (ptr_two->capacity < 1)
			{
				checker_4 = 1;

				break;
			}
		}

		ptr_two = ptr_two->next;
	}

	while (ptr) {
		if (ptr->student_id == temp_data->student_id) {
			per_money = ptr->money;
			if (ptr->money < food_prce) { checker_3 = 1; break; }
		}
		ptr = ptr->next;
	}

	int percentage_coupon;
	int cpty_cpn;
	while (temp_coupon) {
		if (temp_coupon->copoun_id == cpn_cde) {
			percentage_coupon = temp_coupon->percentage;
			cpty_cpn = temp_coupon->capacity;
			checker_5 = 1;
			break;
		}
		temp_coupon = temp_coupon->next;
	}


	ptr = head;
	ptr = ptr->next;

	ptr_two = head_hall_for_user_menu;
	ptr_two = ptr_two->next;

	temp_food = head_for_menu_of_user;
	temp_food = temp_food->next;

	temp_coupon = head_for_coupon;
	temp_coupon = temp_coupon->next;

	if (checker == 0 || checker_2 == 1 || checker_3 == 1 || checker_4 == 1|| checker_5 == 0) printf("not possible\n");
	else {
		printf("done\n");
		cpty = cpty - 1;
		per_money = per_money - food_prce;
		hall_cpty = hall_cpty - 1;
		//hesab systemm


		while (temp_food) {
			if (temp_food->food_id == n) {
				temp3->day = temp_food->day;
				temp3->food_id = temp_food->food_id;
				temp_food->capacity = cpty;
				break;
			}
			temp_food = temp_food->next;
		}

		while (ptr_two) {
			if (ptr_two->hall_id == y) {
				ptr_two->capacity = hall_cpty;
				break;
			}
			ptr_two = ptr_two->next;
		}
		while (ptr) {
			if (ptr->student_id == temp_data->student_id) {
				ptr->money = ptr->money -( (food_prce*(100 - percentage_coupon) ) / 100);
				temp3->money = ((food_prce*(100 - percentage_coupon)) / 100);
				break;
			}
			ptr = ptr->next;
		}

		while (temp_coupon) {
			if (temp_coupon->copoun_id == cpn_cde) {
				temp_coupon->capacity -= 1;
				break;
			}
			temp_coupon = temp_coupon->next;
		}

		information *temp7 = (information*)calloc(1, sizeof(information));
		information *ptr_three;
		temp7 = head_of_iformation;
		while (1) {
			if (temp7->next == NULL) {
				temp7->next = temp3;
				ptr_three = temp7->next;
				ptr_three->next = NULL;
				break;
			}
			else { temp7 = temp7->next; }
		}

	}

}

void reserve_food_func(users_data* temp_data, food* head_for_menu_of_user, users_data*head, hall* head_hall_for_user_menu, information* head_of_iformation) {

	users_data*ptr = head;
	ptr = ptr->next;

	hall*ptr_two = head_hall_for_user_menu;
	ptr_two = ptr_two->next;

	information *temp3 = (information*)calloc(1, sizeof(information));
	int n;
	scanf("%d", &n);

	food*temp_food = head_for_menu_of_user;
	temp_food = temp_food->next;
	int checker = 0;
	int checker_2 = 0;
	int checker_3 = 0;
	int checker_4 = 0;
	int y=0; int food_prce=0; int per_money=0; int cpty=0;
	while (temp_food) {
		if (temp_food->food_id == n) {
		//	temp3->day = temp_food->day;
			//temp3->food_id = temp_food->food_id;
			y = temp_food->hall_id;
			food_prce = temp_food->price;
			cpty = temp_food->capacity;
			checker = 1;
			if (temp_food->capacity < 1) checker_2 = 1;
			break;

		}
		temp_food = temp_food->next;
	}


	int hall_cpty=0;
	while (ptr_two) {

		if (ptr_two->hall_id == y)
		{
			hall_cpty = ptr_two->capacity;
			if (ptr_two->capacity < 1)
			{
				checker_4 = 1;

				break;
			}
		}

		ptr_two = ptr_two->next;
	}
	temp3->studentd_id_login = temp_data->student_id;
	
	while (ptr) {
		if (ptr->student_id == temp_data->student_id) {
			per_money = ptr->money;
			if (ptr->money < food_prce) { checker_3 = 1; break; }
		}
		ptr = ptr->next;
	}

	ptr = head;
	ptr = ptr->next;

	ptr_two = head_hall_for_user_menu;
	ptr_two = ptr_two->next;

	temp_food = head_for_menu_of_user;
	temp_food = temp_food->next;

	if (checker == 0 || checker_2 == 1 || checker_3 == 1 || checker_4 == 1) printf("not possible\n");
	else {
		printf("done\n");
		cpty = cpty - 1;
		per_money = per_money - food_prce;
		hall_cpty = hall_cpty - 1;
		//hesab systemm


		while (temp_food) {
			if (temp_food->food_id == n) {
				temp_food->capacity = cpty;
				temp3->day = temp_food->day;
				temp3->food_id = temp_food->food_id;
				break;
			}
			temp_food = temp_food->next;
		}

		while (ptr_two) {
			if (ptr_two->hall_id == y) {
				ptr_two->capacity = hall_cpty;
				break;
			}
			ptr_two = ptr_two->next;
		}
		while (ptr) {
			if (ptr->student_id == temp_data->student_id) {
				ptr->money = ptr->money - food_prce;
				temp3->money = food_prce;
				break;
			}
			ptr = ptr->next;
		}

		//information*ptr_three = head_of_iformation;
		//ptr_three = ptr_three->next;

		information *temp7 = (information*)calloc(1, sizeof(information));
		information *ptr_three;
		temp7 = head_of_iformation;
		while (1) {
			if (temp7->next == NULL) {
				temp7->next = temp3;
				ptr_three = temp7->next;
				ptr_three->next = NULL;
				break;
			}
			//printf("%d %d %d %d\n", temp7->money, temp7->day, temp7->food_id, temp7->studentd_id_login);
			else { temp7 = temp7->next; }
			//printf("%d %d %d %d\n", temp7->money, temp7->day, temp7->food_id, temp7->studentd_id_login);
		}
/*
		temp7 = head_of_iformation;
		temp7 = temp7->next;
		while (temp7) {
			printf("%d %d %d %d\n", temp7->money, temp7->day, temp7->food_id, temp7->studentd_id_login);

			temp7 = temp7->next;
		}
		*/
		
		/*

		information*headdd = (information*)malloc(sizeof(information));
		headdd->next = NULL;

		information* ptr_seven = (information*)calloc(1, sizeof(information));
		ptr_seven = headdd;
		ptr_seven = ptr_seven->next;
		information *ptr_s;

		information *temp9 = (information*)calloc(1, sizeof(information));
		temp9 = head_of_iformation;
		temp9 = temp9->next;

		int v = 0;

		while (temp9) {
			//if (ptr_seven->next == NULL) {
				ptr_seven = temp9;
			//	ptr_s = ptr_seven;
			//	ptr_s->next = NULL;
			//}
			printf("%d %d %d %d\n", ptr_seven->money, ptr_seven->day, ptr_seven->food_id, ptr_seven->studentd_id_login);
			 temp9 = temp9->next; 
			 ptr_seven = ptr_seven->next;
		//	 ptr_seven->next = NULL;
		}

		*/
	}
}

void menu_func(food*head_for_menu_of_user, hall* head_hall_for_user_menu) {
	hall* temp = head_hall_for_user_menu;
	food* ptr = head_for_menu_of_user;
	ptr = ptr->next;
	temp = temp->next;


	for (int i = 1; i < 8; i++) {
		printf("%d:\n", i);

		while (ptr) {
			if (ptr->day == i) {
				while (temp) {
					if (temp->hall_id == ptr->hall_id) break;
					temp = temp->next;
				}
				printf("%s %d %d %s %d\n", ptr->name, ptr->price, ptr->food_id, temp->name, ptr->capacity);
				temp = head_hall_for_user_menu; temp = temp->next;
			}
			ptr = ptr->next;
		}
		temp = head_hall_for_user_menu; temp = temp->next;
		ptr = head_for_menu_of_user;
		ptr = ptr->next;
	}

}

void user_stuff(users_data*head, users_data* temp_data, food* head_for_menu_of_user, hall* head_hall_for_user_menu, copoun* head_for_coupon, information* head_of_iformation) {
	users_data*ptr = head;
	ptr = ptr->next;
	users_data*ptr_show_money = head;
	ptr_show_money = ptr_show_money->next;

	char* kbhit; char ch; int i;
	int money; int first_money = 0;

	while (1) {
		kbhit = (char*)calloc(1, sizeof(char));
		for (i = 0; ((ch = getchar()) != ' ') && (ch != '\n'); i++) {

			kbhit = (char*)realloc(kbhit, (i + 1) * sizeof(char));
			kbhit[i] = ch;
		}
		kbhit[i] = '\0';

		if (strcmp(kbhit, "logout") == 0) { printf("done\n"); return; }
		else if (strcmp(kbhit, "get_money") == 0) {
			scanf("%d", &money);
			while (ptr) {
				if (temp_data->student_id == ptr->student_id) {
					ptr->money += money; printf("done\n"); break;
				}
				ptr = ptr->next;
			}
		}
		else if (strcmp(kbhit, "show_money") == 0) {
			while (ptr_show_money) {
				if (temp_data->student_id == ptr_show_money->student_id) {
					printf("%d\n", ptr_show_money->money); break;
				}
				ptr_show_money = ptr_show_money->next;
			}
		}
		else if (strcmp(kbhit, "menu") == 0) {
			menu_func(head_for_menu_of_user, head_hall_for_user_menu);
		}

		else if (strcmp(kbhit, "reserve") == 0) {
			reserve_food_func(temp_data, head_for_menu_of_user, head, head_hall_for_user_menu, head_of_iformation);
		}
		else if (strcmp(kbhit, "reserve_discount") == 0) {
			reserve_discount_food_func(temp_data, head_for_menu_of_user, head, head_hall_for_user_menu, head_for_coupon, head_of_iformation);
		}

		else if (strcmp(kbhit, "show_reserved") == 0) {
			show_reserved_func(temp_data, head_for_menu_of_user, head, head_hall_for_user_menu, head_for_coupon, head_of_iformation);
		}
		else if (strcmp(kbhit, "exit") == 0) {
			exit(0);
		}
		else if (strcmp(kbhit, "add_hall") == 0 || strcmp(kbhit, "add_food") == 0 || strcmp(kbhit, "add_coupon") == 0 || strcmp(kbhit, "food_report") == 0 || strcmp(kbhit, "hall_report") == 0) {
			printf("permission denied");
			fseek(stdin, 0, SEEK_CUR);
		}
		else {
			printf("Invalid command!\n");
			fseek(stdin, 0, SEEK_CUR);
		}
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
login_func(users_data*head, food*head_for_menu_of_user, hall*head_hall_for_user_menu, copoun* head_for_coupon, information* head_of_iformation) {


	users_data *temp_data = (users_data*)malloc(sizeof(users_data));
	char ch; int i = 0, j = 0;
	temp_data->username = (char*)malloc(sizeof(char));
	temp_data->password = (char*)malloc(sizeof(char));
	while (1) {
		ch = getchar();
		if (ch == ' ') break;
		temp_data->username = (char*)realloc(temp_data->username, (i + 1) * sizeof(char));
		temp_data->username[i] = ch;
		i++;
	}
	temp_data->username[i] = '\0';

	while (1) {
		ch = getchar();
		if (ch == '\n') break;
		temp_data->password = (char*)realloc(temp_data->password, (j + 1) * sizeof(char)); temp_data->password[j] = ch; j++;
	}
	temp_data->password[j] = '\0';
	//..............,............................................................
	int checker = 0;
	users_data *temp_check = head;
	temp_check = temp_check->next;
	while (temp_check) {
		if (strcmp(temp_data->username, temp_check->username) == 0 && strcmp(temp_data->password, temp_check->password) == 0) {
			temp_data->student_id = temp_check->student_id; temp_data->money = temp_check->money;  checker = 1;
		}
		if (strcmp(temp_data->username, "admin") == 0 && strcmp(temp_data->password, "admin") == 0) checker = -1;
		temp_check = temp_check->next;
	}

	if (checker == 1) { printf("welcome %s\n", temp_data->username); user_stuff(head, temp_data, head_for_menu_of_user, head_hall_for_user_menu, head_for_coupon, head_of_iformation); }
	if (checker == 0) printf("wrong user or pass\n");
	if (checker == -1) { printf("welcome admin\n");  admin_stuff(head, head_for_menu_of_user, head_hall_for_user_menu, head_for_coupon, head_of_iformation); }
}

void sign_in_func(users_data*head) {
	users_data *temp1 = head;
	temp1 = temp1->next;
	users_data *temp_check = head;
	temp_check = temp_check->next;
	users_data *temp_data = (users_data*)malloc(sizeof(users_data));
	char ch; int i = 0, j = 0;

	temp_data->username = (char*)malloc(sizeof(char));
	temp_data->password = (char*)malloc(sizeof(char));

	while (1) {
		ch = getchar();
		if (ch == ' ') break;
		temp_data->username = (char*)realloc(temp_data->username, (i + 1) * sizeof(char));
		temp_data->username[i] = ch;
		i++;
	}
	temp_data->username[i] = 0;

	scanf("%d", &temp_data->student_id);

	getchar();

	while (1) {
		ch = getchar();
		if (ch == '\n') break;
		temp_data->password = (char*)realloc(temp_data->password, (j + 1) * sizeof(char)); temp_data->password[j] = ch; j++;
	}
	temp_data->password[j] = '\0';
	temp_data->money = 0;

	int checker = 0;
	while (temp_check) {
		if (strcmp(temp_data->username, temp_check->username) == 0 || temp_data->student_id == temp_check->student_id) checker = 1;
		temp_check = temp_check->next;
	}

	if (checker == 0) {
		FILE*user_file = fopen(USERS_DATA_LOCATION, "a+");
		fprintf(user_file, "%s %d %s\n", temp_data->username, temp_data->student_id, temp_data->password);
		fclose(user_file);
		printf("done\n");
		//next step
		users_data *temp7 = (users_data*)calloc(1, sizeof(users_data));
		users_data *ptr;
		temp7 = head;
		while (1) {
			if (temp7->next == NULL) {
				temp7->next = temp_data;
				ptr = temp7->next;
				ptr->next = NULL;
				break;
			}
			else { temp7 = temp7->next; }
		}
	}
	if (checker == 1) printf("already exist\n");
}


int main() {
	int y = 0;
	users_data*head = (users_data*)malloc(sizeof(users_data));
	head->next = NULL;
	users_data *temp = head;
	users_data *temp1 = head;
	FILE*temp_file = fopen(USERS_DATA_LOCATION, "r");
	if (temp_file != NULL) {
		while (1) {
			char ch; int i;
			temp->next = (users_data*)calloc(1, sizeof(users_data)); temp = temp->next;

			temp->username = (char*)calloc(1, sizeof(char)); temp->password = (char*)calloc(1, sizeof(char)); temp->money = 0;

			for (i = 0; ((ch = fgetc(temp_file)) != ' ') && ch != '\n'; i++) {
				if (ch == EOF) { break; }
				temp->username = (char*)realloc(temp->username, (i + 1) * sizeof(char)); temp->username[i] = ch;
			}
			temp->username[i] = '\0';

			fscanf(temp_file, "%d", &temp->student_id);

			fgetc(temp_file);

			for (i = 0; (ch = fgetc(temp_file)) != '\n'&&ch != ' '&&ch != EOF; i++) {
				temp->password = (char*)realloc(temp->password, (i + 1) * sizeof(char)); temp->password[i] = ch;
			}
			temp->password[i] = '\0';

			if (ch = fgetc(temp_file) == EOF) { break; }
			else { fseek(temp_file, -1, SEEK_CUR); }

			temp->next = NULL;
		}
		fclose(temp_file);
	}

	else {
		FILE*temp_file = fopen(USERS_DATA_LOCATION, "w");
		fclose(temp_file);
	}











	food* head_for_menu_of_user = (food*)calloc(1, sizeof(food));
	head_for_menu_of_user->next = NULL;

	food *temp_cont = head_for_menu_of_user;
	FILE*temp_file_food = fopen(FOOD_DATA, "r");
	if (temp_file_food != NULL) {
		while (1) {
			char c; int i;

			temp_cont->next = (food*)calloc(1, sizeof(food));
			temp_cont = temp_cont->next;
			temp_cont->name = (char*)calloc(1, sizeof(char));

			for (i = 0; ((c = fgetc(temp_file_food)) != ' ') && c != '\n'; i++) {
				if (c == EOF) { break; }
				temp_cont->name = (char*)realloc(temp_cont->name, (i + 1) * sizeof(char));
				temp_cont->name[i] = c;
			}
			temp_cont->name[i] = '\0';

			fscanf(temp_file_food,"%d %d %d %d %d", &temp_cont->food_id, &temp_cont->price, &temp_cont->capacity, &temp_cont->hall_id, &temp_cont->day);
			//printf("%d\n", temp_cont->food_id);
			fgetc(temp_file_food);
			//printf("%d\n", temp_cont->food_id);
			if (c = fgetc(temp_file_food) == EOF) { break; }
			else { fseek(temp_file_food, -1, SEEK_CUR); }
			temp_cont->next = NULL;
		}
		fclose(temp_file_food);
	}
	else {
		FILE*temp_file_food = fopen(FOOD_DATA, "w");
		fclose(temp_file_food);
	}


	hall* head_hall_for_user_menu = (hall*)calloc(1, sizeof(hall));
	head_hall_for_user_menu->next = NULL;
	hall *temp_hall = head_hall_for_user_menu;
	FILE*temp_file_for_hall_menu = fopen(HALL_DATA, "r");
	if (temp_file_for_hall_menu != NULL) {
		while (1) {
			char ch; int i;

			temp_hall->next = (hall*)calloc(1, sizeof(hall));
			temp_hall = temp_hall->next;
			temp_hall->name = (char*)calloc(1, sizeof(char));

			for (i = 0; ((ch = fgetc(temp_file_for_hall_menu)) != ' ') && ch != '\n'; i++) {
				if (ch == EOF) { break; }
				temp_hall->name = (char*)realloc(temp_hall->name, (i + 1) * sizeof(char)); temp_hall->name[i] = ch;
			}
			temp_hall->name[i] = '\0';

			fscanf(temp_file_for_hall_menu, "%d %d", &temp_hall->hall_id, &temp_hall->capacity);
			fgetc(temp_file_for_hall_menu);
			//	printf("%d\n", temp_hall->hall_id);
			if (ch = fgetc(temp_file_for_hall_menu) == EOF) { break; }
			else { fseek(temp_file_for_hall_menu, -1, SEEK_CUR); }
			temp_hall->next = NULL;
		}
		fclose(temp_file_for_hall_menu);
	}


		copoun* head_for_coupon = (copoun*)calloc(1, sizeof(copoun));
		head_for_coupon->next = NULL;
		copoun *temp_of_coupon = head_for_coupon;
		FILE*temp_file_for_coupon = fopen(COUPON_DATA, "r");
		if (temp_file_for_coupon != NULL) {
		while (1) {
			char ch;
			temp_of_coupon->next = (copoun*)calloc(1, sizeof(copoun));
			temp_of_coupon = temp_of_coupon->next;

			fscanf(temp_file_for_coupon, "%d %d %d", &temp_of_coupon->copoun_id, &temp_of_coupon->percentage, &temp_of_coupon->capacity);
			fgetc(temp_file_for_coupon);
			if (ch = fgetc(temp_file_for_coupon) == EOF) { break; }
			else { fseek(temp_file_for_coupon, -1, SEEK_CUR); }
			temp_of_coupon->next = NULL;
		}
		fclose(temp_file_for_coupon);
	}
	else {
		FILE*temp_file_for_coupon = fopen(COUPON_DATA, "w");
		fclose(temp_file_for_coupon);
	}


	information* head_of_iformation = (information*)calloc(1, sizeof(information));
	head_of_iformation->next = NULL;



	int logged_in = 0; int is_admin = 0; int check_correct = 0; int p = 0; int flag = 0, m = 0;

	char* kbhit; char ch; int i;


	while (1) {

		kbhit = (char*)calloc(1, sizeof(char));
		for (i = 0; ((ch = getchar()) != ' ') && (ch != '\n'); i++) {

			kbhit = (char*)realloc(kbhit, (i + 1) * sizeof(char));
			kbhit[i] = ch;
		}
		kbhit[i] = '\0';


		if (strcmp(kbhit, "login") == 0)
		{
			login_func(head, head_for_menu_of_user, head_hall_for_user_menu, head_for_coupon, head_of_iformation);
		}

		else if (strcmp(kbhit, "signup") == 0)
		{
			sign_in_func(head);
		}
		else if (strcmp(kbhit, "exit") == 0) {
			exit(0);
		}
		else if (strcmp(kbhit, "logout") == 0 || strcmp(kbhit, "get_money") == 0 || strcmp(kbhit, "show_money") == 0 || strcmp(kbhit, "menu") == 0 || strcmp(kbhit, "reserve") == 0 || strcmp(kbhit, "reserve_discount") == 0 || strcmp(kbhit, "show_reserved") == 0 || strcmp(kbhit, "show_money") == 0 || strcmp(kbhit, "add_hall") == 0 || strcmp(kbhit, "add_food") == 0 || strcmp(kbhit, "add_coupon") == 0 || strcmp(kbhit, "food_report") == 0 || strcmp(kbhit, "hall_report") == 0) {
			printf("access denied\n");
			fseek(stdin, 0, SEEK_CUR);
		}
		else {
			printf("Invalid command!\n");
			fseek(stdin, 0, SEEK_CUR);
		}
	}
}