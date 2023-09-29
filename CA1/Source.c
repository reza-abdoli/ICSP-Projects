#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

#pragma warning(disable:4996)

//colors
#define RED 12
#define BLUE 3
#define GREEN 10
#define YELLOW 14
#define GRAY 8
#define PINK 13
#define WHITE 15
#define WAIT_TIME_MILI_SEC 100
//directions
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
// general
#define BOARD_SIZE 40
#define INITIAL_SNAKE_LENGTH 3
#define MINIMUM_SNAKE_LENGTH 2
#define MAX_LEN_SNAKES 30
#define NUMBER_OF_MOUSES 20
//board_characters
#define EMPTY '0'
#define MOUSE 'm'
#define PLAYER1_SNAKE_HEAD '1'
#define PLAYER2_SNAKE_HEAD '2'
#define PLAYER1_SNAKE_BODY 'a'
#define PLAYER2_SNAKE_BODY 'b'
//Bernard, Poison and golden star
#define BERNARD_CLOCK 'c' //on the board character
#define GOLDEN_STAR '*' //on the board character
#define POISON 'x' //on the board character
#define NUMBER_OF_POISONS 5
#define NUMBER_OF_GOLDEN_STARS 3
#define BERNARD_CLOCK_APPEARANCE_CHANCE_PERCENT 20
#define BERNARD_CLOCK_APPEARANCE_CHECK_PERIOD_MILI_SEC 2000
#define BERNARD_CLOCK_FROZEN_TIME_MILI_SEC 4000

CONSOLE_FONT_INFOEX former_cfi;
CONSOLE_CURSOR_INFO former_info;
COORD former_screen_size;

void reset_console() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleDisplayMode(consoleHandle, CONSOLE_WINDOWED_MODE, &former_screen_size);
	SetCurrentConsoleFontEx(consoleHandle, FALSE, &former_cfi);
	SetConsoleCursorInfo(consoleHandle, &former_info);
}

void hidecursor(HANDLE consoleHandle)
{
	GetConsoleCursorInfo(consoleHandle, &former_info);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void set_console_font_and_font_size(HANDLE consoleHandle) {
	former_cfi.cbSize = sizeof(former_cfi);
	GetCurrentConsoleFontEx(consoleHandle, FALSE, &former_cfi);
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 20;
	cfi.dwFontSize.Y = 20;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"Courier");
	SetCurrentConsoleFontEx(consoleHandle, FALSE, &cfi);
}

void set_full_screen_mode(HANDLE consoleHandle) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	former_screen_size.X = csbi.dwSize.X; former_screen_size.Y = csbi.dwSize.Y;
	COORD coord;
	SetConsoleDisplayMode(consoleHandle, CONSOLE_FULLSCREEN_MODE, &coord);
}

void init_screen()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	set_full_screen_mode(consoleHandle);
	hidecursor(consoleHandle);
	set_console_font_and_font_size(consoleHandle);

}

void wait_and_get_direction(int* player1_snake_direction, int* player2_snake_direction) {
	DWORD64 start_time, check_time;
	start_time = GetTickCount64();
	check_time = start_time + WAIT_TIME_MILI_SEC; //GetTickCount returns time in miliseconds
	char key = 0;
	char player1_key_hit = 0;
	char player2_key_hit = 0;
	while (check_time > GetTickCount64()) {
		if (_kbhit()) {
			key = _getch();
			if (key == 0)
				key = _getch();
			if (key == 'w' || key == 'a' || key == 's' || key == 'd')
				player1_key_hit = key;
			if (key == 'i' || key == 'j' || key == 'k' || key == 'l')
				player2_key_hit = key;
		}
	}
	switch (player1_key_hit) {
	case 'w': if (*player1_snake_direction != DOWN) *player1_snake_direction = UP; break;
	case 'a': if (*player1_snake_direction != RIGHT) *player1_snake_direction = LEFT; break;
	case 's': if (*player1_snake_direction != UP) *player1_snake_direction = DOWN; break;
	case 'd': if (*player1_snake_direction != LEFT) *player1_snake_direction = RIGHT; break;
	default: break;
	}
	switch (player2_key_hit) {
	case 'i': if (*player2_snake_direction != DOWN) *player2_snake_direction = UP; break;
	case 'j': if (*player2_snake_direction != RIGHT) *player2_snake_direction = LEFT; break;
	case 'k': if (*player2_snake_direction != UP) *player2_snake_direction = DOWN; break;
	case 'l': if (*player2_snake_direction != LEFT) *player2_snake_direction = RIGHT; break;
	default: break;
	}
}

void draw_point(char point_content) {
	switch (point_content) {
	case PLAYER1_SNAKE_HEAD: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED); printf("@"); break;
	case PLAYER2_SNAKE_HEAD: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);  printf("@"); break;
	case PLAYER1_SNAKE_BODY: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);  printf("o"); break;
	case PLAYER2_SNAKE_BODY: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);  printf("o"); break;
	case MOUSE: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GRAY); printf("m"); break;
	case GOLDEN_STAR: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW); printf("*"); break;
	case POISON: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN); printf("x"); break;
	case BERNARD_CLOCK: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PINK); printf("c"); break;
	default: printf(" ");
	}
}

void draw_horizonatal_walls() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	for (int i = 0; i < BOARD_SIZE + 2; ++i)
		printf("-");
	printf("\n");
}

void draw_board(char board_content[BOARD_SIZE][BOARD_SIZE]) {
	system("cls");
	draw_horizonatal_walls();
	for (int i = 0; i < BOARD_SIZE; i++) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
		printf("|"); // vertical wall 
		for (int j = 0; j < BOARD_SIZE; j++)
			draw_point(board_content[i][j]);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
		printf("|\n"); // vertical wall
	}
	draw_horizonatal_walls();
}

//tabe keshidan mosh
void draw_mouse(char board_content[BOARD_SIZE][BOARD_SIZE]) {
	int range_mouse = 0;
	int index_first_mouse;
	int index_secound_mouse;
	for (int z = 0; z < BOARD_SIZE; z++) {
		if (range_mouse == NUMBER_OF_MOUSES) break;
		for (int b = 0; b < BOARD_SIZE; b++) {
			if (range_mouse == NUMBER_OF_MOUSES) break;
			if (board_content[z][b] == MOUSE) range_mouse += 1;

		}
	}
	// ba halghe hamzaman 20 mosh dar safhe ijad miknim
	if (range_mouse < NUMBER_OF_MOUSES) {
		while (range_mouse < NUMBER_OF_MOUSES) {

			index_first_mouse = rand() % BOARD_SIZE;
			index_secound_mouse = rand() % BOARD_SIZE;
			if (board_content[index_first_mouse][index_secound_mouse] == EMPTY) {
				range_mouse += 1;
				board_content[index_first_mouse][index_secound_mouse] = MOUSE;

			}
		}
	}
}
//tabe keshidan setare
void setare_talaei(char board_content[BOARD_SIZE][BOARD_SIZE]) {
	int setare_talaei = 0;
	int index_first_setare_talaei;
	int index_secound_setare_talaei;
	for (int z = 0; z < BOARD_SIZE; z++) {
		if (setare_talaei == NUMBER_OF_GOLDEN_STARS) break;
		for (int b = 0; b < BOARD_SIZE; b++) {
			if (setare_talaei == NUMBER_OF_GOLDEN_STARS) break;
			if (board_content[z][b] == GOLDEN_STAR) setare_talaei += 1;

		}
	}
	// ba halghe hamzaman 3 setare dar safhe ijad miknim
	if (setare_talaei < NUMBER_OF_GOLDEN_STARS) {
		while (setare_talaei < NUMBER_OF_GOLDEN_STARS) {

			index_first_setare_talaei = rand() % BOARD_SIZE;
			index_secound_setare_talaei = rand() % BOARD_SIZE;
			if (board_content[index_first_setare_talaei][index_secound_setare_talaei] == EMPTY) {
				setare_talaei += 1;
				board_content[index_first_setare_talaei][index_secound_setare_talaei] = GOLDEN_STAR;

			}
		}
	}
}

void turn1(int PLAYER1_SNAKE_ARRAY[MAX_LEN_SNAKES][2], int player1_snake_direction, char board_content[BOARD_SIZE][BOARD_SIZE], int *plus1, int PLAYER2_SNAKE_ARRAY[MAX_LEN_SNAKES][2], int *plus2) {
	int plus_len1 = 0;
	int result = 0;



	//barrasi jahat mar1 va sayere amaliat ha :
	if (player1_snake_direction == DOWN) {
		if (board_content[PLAYER1_SNAKE_ARRAY[0][0] + 1][PLAYER1_SNAKE_ARRAY[0][1]] == GOLDEN_STAR) {

			*plus2 -= 1;
			setare_talaei(board_content);
		}


		if (board_content[PLAYER1_SNAKE_ARRAY[0][0] + 1][PLAYER1_SNAKE_ARRAY[0][1]] == MOUSE) {

			*plus1 += 1;
			draw_mouse(board_content);
		}

		// jabejaei badan
		for (int i = INITIAL_SNAKE_LENGTH + *plus1; i >= 1; --i) {
			PLAYER1_SNAKE_ARRAY[i][0] = PLAYER1_SNAKE_ARRAY[i - 1][0];
			PLAYER1_SNAKE_ARRAY[i][1] = PLAYER1_SNAKE_ARRAY[i - 1][1];
		}
		if (PLAYER1_SNAKE_ARRAY[0][0] == BOARD_SIZE - 1) PLAYER1_SNAKE_ARRAY[0][0] = -1;

		//jabejaei sar ba tavajoh be jahat
		PLAYER1_SNAKE_ARRAY[0][0]++;

	}

	if (player1_snake_direction == UP) {
		if (board_content[PLAYER1_SNAKE_ARRAY[0][0] - 1][PLAYER1_SNAKE_ARRAY[0][1]] == GOLDEN_STAR) {

			*plus2 -= 1;
			setare_talaei(board_content);
		}

		if (board_content[PLAYER1_SNAKE_ARRAY[0][0] - 1][PLAYER1_SNAKE_ARRAY[0][1]] == MOUSE) {

			*plus1 += 1;
			draw_mouse(board_content);
		}

		// jabejaei badan
		for (int i = INITIAL_SNAKE_LENGTH + *plus1; i >= 1; i--) {
			PLAYER1_SNAKE_ARRAY[i][0] = PLAYER1_SNAKE_ARRAY[i - 1][0];
			PLAYER1_SNAKE_ARRAY[i][1] = PLAYER1_SNAKE_ARRAY[i - 1][1];
		}
		if (PLAYER1_SNAKE_ARRAY[0][0] == 0) PLAYER1_SNAKE_ARRAY[0][0] = BOARD_SIZE;

		//jabejaei sar ba tavajoh be jahat
		PLAYER1_SNAKE_ARRAY[0][0]--;


	}

	if (player1_snake_direction == LEFT) {
		if (board_content[PLAYER1_SNAKE_ARRAY[0][0] ][PLAYER1_SNAKE_ARRAY[0][1]-1] == GOLDEN_STAR) {

			*plus2 -= 1;
			setare_talaei(board_content);
		}

		if (board_content[PLAYER1_SNAKE_ARRAY[0][0]][PLAYER1_SNAKE_ARRAY[0][1] - 1] == MOUSE) {
			*plus1 += 1;
			draw_mouse(board_content);
		}
		// jabejaei badan
		for (int i = INITIAL_SNAKE_LENGTH + *plus1; i >= 1; i--) {
			PLAYER1_SNAKE_ARRAY[i][0] = PLAYER1_SNAKE_ARRAY[i - 1][0];
			PLAYER1_SNAKE_ARRAY[i][1] = PLAYER1_SNAKE_ARRAY[i - 1][1];
		}
		if (PLAYER1_SNAKE_ARRAY[0][1] == 0) PLAYER1_SNAKE_ARRAY[0][1] = BOARD_SIZE;

		//jabejaei sar ba tavajoh be jahat
		PLAYER1_SNAKE_ARRAY[0][1]--;


	}

	if (player1_snake_direction == RIGHT) {
		if (board_content[PLAYER1_SNAKE_ARRAY[0][0] ][PLAYER1_SNAKE_ARRAY[0][1]+1] == GOLDEN_STAR) {

			*plus2 -= 1;
			setare_talaei(board_content);
		}

		if (board_content[PLAYER1_SNAKE_ARRAY[0][0]][PLAYER1_SNAKE_ARRAY[0][1] + 1] == MOUSE) {

			*plus1 += 1;
			draw_mouse(board_content);
		}
		// jabejaei badan
		for (int i = INITIAL_SNAKE_LENGTH + *plus1; i >= 1; i--) {
			PLAYER1_SNAKE_ARRAY[i][0] = PLAYER1_SNAKE_ARRAY[i - 1][0];
			PLAYER1_SNAKE_ARRAY[i][1] = PLAYER1_SNAKE_ARRAY[i - 1][1];
		}
		if (PLAYER1_SNAKE_ARRAY[0][1] == BOARD_SIZE - 1) PLAYER1_SNAKE_ARRAY[0][1] = -1;

		//jabejaei sar ba tavajoh be jahat
		PLAYER1_SNAKE_ARRAY[0][1]++;

	}

}

void turn2(int PLAYER2_SNAKE_ARRAY[MAX_LEN_SNAKES][2], int player2_snake_direction, char board_content[BOARD_SIZE][BOARD_SIZE], int *plus2, int PLAYER1_SNAKE_ARRAY[MAX_LEN_SNAKES][2], int *plus1) {


	//barrasi jahat mar2 va sayere amaliat ha :
	if (player2_snake_direction == DOWN) {
		if (board_content[PLAYER2_SNAKE_ARRAY[0][0] + 1][PLAYER2_SNAKE_ARRAY[0][1]] == GOLDEN_STAR) {

			*plus1 -= 1;
			setare_talaei(board_content);
		}

		if (board_content[PLAYER2_SNAKE_ARRAY[0][0] + 1][PLAYER2_SNAKE_ARRAY[0][1]] == MOUSE) {

			*plus2 += 1;
			draw_mouse(board_content);
		}
		// jabejaei badan
		for (int i = INITIAL_SNAKE_LENGTH  + *plus2; i >= 1; --i) {
			PLAYER2_SNAKE_ARRAY[i][0] = PLAYER2_SNAKE_ARRAY[i - 1][0];
			PLAYER2_SNAKE_ARRAY[i][1] = PLAYER2_SNAKE_ARRAY[i - 1][1];
		}
		if (PLAYER2_SNAKE_ARRAY[0][0] == BOARD_SIZE - 1) PLAYER2_SNAKE_ARRAY[0][0] = -1;

		//jabejaei sar ba tavajoh be jahat
		PLAYER2_SNAKE_ARRAY[0][0]++;


	}

	if (player2_snake_direction == UP) {
		if (board_content[PLAYER2_SNAKE_ARRAY[0][0] - 1][PLAYER2_SNAKE_ARRAY[0][1]] == GOLDEN_STAR) {

			*plus1 -= 1;
			setare_talaei(board_content);
		}


		if (board_content[PLAYER2_SNAKE_ARRAY[0][0] - 1][PLAYER2_SNAKE_ARRAY[0][1]] == MOUSE) {

			*plus2 += 1;
			draw_mouse(board_content);
		}
		// jabejaei badan
		for (int i = INITIAL_SNAKE_LENGTH + *plus2; i >= 1; i--) {
			PLAYER2_SNAKE_ARRAY[i][0] = PLAYER2_SNAKE_ARRAY[i - 1][0];
			PLAYER2_SNAKE_ARRAY[i][1] = PLAYER2_SNAKE_ARRAY[i - 1][1];
		}
		if (PLAYER2_SNAKE_ARRAY[0][0] == 0) PLAYER2_SNAKE_ARRAY[0][0] = BOARD_SIZE;

		//jabejaei sar ba tavajoh be jahat
		PLAYER2_SNAKE_ARRAY[0][0]--;


	}

	if (player2_snake_direction == LEFT) {
		if (board_content[PLAYER2_SNAKE_ARRAY[0][0] ][PLAYER2_SNAKE_ARRAY[0][1]-1] == GOLDEN_STAR) {

			*plus1 -= 1;
			setare_talaei(board_content);
		}



		if (board_content[PLAYER2_SNAKE_ARRAY[0][0]][PLAYER2_SNAKE_ARRAY[0][1] - 1] == MOUSE) {
			*plus2 += 1;
			draw_mouse(board_content);
		}
		// jabejaei badan
		for (int i = INITIAL_SNAKE_LENGTH + *plus2; i >= 1; i--) {
			PLAYER2_SNAKE_ARRAY[i][0] = PLAYER2_SNAKE_ARRAY[i - 1][0];
			PLAYER2_SNAKE_ARRAY[i][1] = PLAYER2_SNAKE_ARRAY[i - 1][1];
		}
		if (PLAYER2_SNAKE_ARRAY[0][1] == 0) PLAYER2_SNAKE_ARRAY[0][1] = BOARD_SIZE;

		//jabejaei sar ba tavajoh be jahat
		PLAYER2_SNAKE_ARRAY[0][1]--;


	}

	if (player2_snake_direction == RIGHT) {
		if (board_content[PLAYER2_SNAKE_ARRAY[0][0]][PLAYER2_SNAKE_ARRAY[0][1]+1] == GOLDEN_STAR) {

			*plus1 -= 1;
			setare_talaei(board_content);
		}


		if (board_content[PLAYER2_SNAKE_ARRAY[0][0]][PLAYER2_SNAKE_ARRAY[0][1] + 1] == MOUSE) {

			*plus2 += 1;
			draw_mouse(board_content);
		}
		// jabejaei badan
		for (int i = INITIAL_SNAKE_LENGTH + *plus2; i >= 1; i--) {
			PLAYER2_SNAKE_ARRAY[i][0] = PLAYER2_SNAKE_ARRAY[i - 1][0];
			PLAYER2_SNAKE_ARRAY[i][1] = PLAYER2_SNAKE_ARRAY[i - 1][1];
		}
		if (PLAYER2_SNAKE_ARRAY[0][1] == BOARD_SIZE - 1) PLAYER2_SNAKE_ARRAY[0][1] = -1;

		//jabejaei sar ba tavajoh be jahat
		PLAYER2_SNAKE_ARRAY[0][1]++;


	}


}
// tabe pak kardan safhe
void empty(char board_content[BOARD_SIZE][BOARD_SIZE]) {

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board_content[i][j] == PLAYER1_SNAKE_HEAD || board_content[i][j] == PLAYER2_SNAKE_HEAD || board_content[i][j] == PLAYER1_SNAKE_BODY || board_content[i][j] == PLAYER2_SNAKE_BODY)
				board_content[i][j] = EMPTY;
		}
	}
}

// prototypes
void init_screen();
void reset_console();
void wait_and_get_direction(int* player1_snake_direction, int* player2_snake_direction);
void draw_board(char board_content[BOARD_SIZE][BOARD_SIZE]);

int main() {
	// tariif maghadir mord niaz
	char board_content[BOARD_SIZE][BOARD_SIZE];

	int plus_len1 = 0;
	int plus_len2 = 0;
	int* plus1 = &plus_len1;
	int* plus2 = &plus_len2;

	int player1_snake_direction = DOWN;
	int player2_snake_direction = UP;

	// tarif mokhtasat jadid baray mar dar board content
	int PLAYER1_SNAKE_ARRAY[30][2] = { 0 };
	int PLAYER2_SNAKE_ARRAY[30][2] = { 0 };

	//khali kardan safhe
	for (int z = 0; z < BOARD_SIZE; z++) {
		for (int b = 0; b < BOARD_SIZE; b++) {
			board_content[z][b] = EMPTY;
		}
	}

	//be mokhtasat mar baraye shoro meghdar avalie midim
	PLAYER1_SNAKE_ARRAY[0][0] = 2; PLAYER1_SNAKE_ARRAY[0][1] = 0;
	PLAYER1_SNAKE_ARRAY[1][0] = 1; PLAYER1_SNAKE_ARRAY[1][1] = 0;
	PLAYER1_SNAKE_ARRAY[2][0] = 0; PLAYER1_SNAKE_ARRAY[2][1] = 0;

	PLAYER2_SNAKE_ARRAY[0][0] = 37; PLAYER2_SNAKE_ARRAY[0][1] = 39;
	PLAYER2_SNAKE_ARRAY[1][0] = 38; PLAYER2_SNAKE_ARRAY[1][1] = 39;
	PLAYER2_SNAKE_ARRAY[2][0] = 39; PLAYER2_SNAKE_ARRAY[2][1] = 39;


	//Do not forget to Initialize your variables, specially board_content
	// meghdar avalie haro dar board content mikeshim
	board_content[0][0] = PLAYER1_SNAKE_BODY; board_content[1][0] = PLAYER1_SNAKE_BODY; board_content[2][0] = PLAYER1_SNAKE_HEAD;
	board_content[39][39] = PLAYER2_SNAKE_BODY; board_content[38][39] = PLAYER2_SNAKE_BODY; board_content[37][39] = PLAYER2_SNAKE_HEAD;


	init_screen(); //call this to make window adjustments


	while (TRUE) {
		draw_board(board_content);
		wait_and_get_direction(&player1_snake_direction, &player2_snake_direction);
		// ebteda 20 mosh va 3 setare dar safhe mikeshim
		draw_mouse(board_content);
		setare_talaei(board_content);

		// barrasi shart payan
		if (*plus2<-1) {
			printf("player one win\n");
			break;
		}
		if ( *plus1<-1) {
			printf("player two win\n");
			break;
		}
		if (board_content[PLAYER1_SNAKE_ARRAY[0][0]][PLAYER1_SNAKE_ARRAY[0][1]] == PLAYER2_SNAKE_BODY) {
			reset_console();
			printf("player two win\n");
			break;
		}
		if (board_content[PLAYER1_SNAKE_ARRAY[0][0]][PLAYER1_SNAKE_ARRAY[0][1]] == PLAYER1_SNAKE_BODY)
		{
			reset_console();
			printf("player two win\n");
			break;
		}
		if (board_content[PLAYER2_SNAKE_ARRAY[0][0]][PLAYER2_SNAKE_ARRAY[0][1]] == PLAYER2_SNAKE_BODY)
		{
			reset_console();
			printf("player one win\n");
			break;
		}
		if (board_content[PLAYER1_SNAKE_ARRAY[0][0]][PLAYER1_SNAKE_ARRAY[0][1]] == board_content[PLAYER2_SNAKE_ARRAY[0][0]][PLAYER2_SNAKE_ARRAY[0][1]]) {
			if (*plus1 > *plus2) {
				reset_console();
				printf("player one win\n");
				break;
			}
			if (*plus2 > *plus1) {
				reset_console();
				printf("player two win\n");
				break;
			}
			if (*plus1 == *plus2) {
				reset_console();
				printf("nobody winner\n");
				break;
			}

		}

		// tabe jabejaee , charkhesh , khordan mosh va setare ,  mar 1:
		turn1(PLAYER1_SNAKE_ARRAY, player1_snake_direction, board_content, plus1, PLAYER2_SNAKE_ARRAY, plus2); 

		// tabe jabejaee , charkhesh , khordan mosh va setare ,  mar 2:
		turn2(PLAYER2_SNAKE_ARRAY, player2_snake_direction, board_content, plus2, PLAYER1_SNAKE_ARRAY, plus1);


		if (board_content[PLAYER2_SNAKE_ARRAY[0][0]][PLAYER2_SNAKE_ARRAY[0][1]] == PLAYER1_SNAKE_BODY) {
			printf("player one win\n");
			break;
		}

		empty(board_content);// pak kardan safhe

		// pas az marhel bala , mogheiyat mar ra dar safhe mikeshim
		board_content[PLAYER1_SNAKE_ARRAY[0][0]][PLAYER1_SNAKE_ARRAY[0][1]] = PLAYER1_SNAKE_HEAD;
		for (int x = 1; x < INITIAL_SNAKE_LENGTH + plus_len1; x++) {
			board_content[PLAYER1_SNAKE_ARRAY[x][0]][PLAYER1_SNAKE_ARRAY[x][1]] = PLAYER1_SNAKE_BODY;
		}

		board_content[PLAYER2_SNAKE_ARRAY[0][0]][PLAYER2_SNAKE_ARRAY[0][1]] = PLAYER2_SNAKE_HEAD;
		for (int y = 1; y < INITIAL_SNAKE_LENGTH + plus_len2; y++) {
			board_content[PLAYER2_SNAKE_ARRAY[y][0]][PLAYER2_SNAKE_ARRAY[y][1]] = PLAYER2_SNAKE_BODY;
		}


	}

}