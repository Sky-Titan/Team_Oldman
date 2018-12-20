#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>

#define START_X 0
#define START_Y 0

static struct termios initial_settings, new_settings;
static int peek_character = -1;

struct villian
{ // F구조체
	int x, y;
	int orientation;
};

typedef struct villian vil; // "F" structure name

clock_t s, f; // 시간

int t = 0, draw_flag = 0; // 시간, 한번 그려졌는지의 flag

#define vil_num LINES - 13 // Villian 개수

void move_villians(vil *vils); // Villian 움직이기

void draw_villians(vil *vils); // Villian 처음에 그리기

void *p1_function(void *data); // Villian 한번 그리고 계속 움직이는 Thread에 전달하는 함수

void *p2_function(); // User 움직이게 해주는 함수

void init_keyboard(); // Keyboard 설정

void close_keyboard(); // Keyboard 설정 원상복귀

int _kbhit(); // 키보드 입력을 1개씩 받게 설정

int _getch(); // 키보드 입력 받기

void move_man(); // User 이동

void game_end(); // Game 종료

void crash(); // 충돌 판정

int crash_flag = 0; //충돌했는지 나타내는 Flag

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 쓰레드 초기화

void draw_villians(vil *vils)
{
	int i, flag = 0;

	for (i = 0; i < vil_num; i++)
	{
		vils[i].x = i + 12;

		flag = rand() % 2;

		vils[i].y = rand() % (COLS - 3) + 1;

		if (flag == 0)
			vils[i].orientation = 1; // 오른쪽으로 이동
		else if (flag == 1)
			vils[i].orientation = -1; // 왼쪽으로 이동

		mvprintw(vils[i].x, vils[i].y, "F");
	}
}

void move_villians(vil *vils)
{
	int i;

	for (i = 0; i < vil_num; i++)
	{
		mvprintw(vils[i].x, vils[i].y, " ");
		refresh;
		vils[i].y += vils[i].orientation;
		mvprintw(vils[i].x, vils[i].y, "F");
		refresh;
		if (vils[i].y == 1 || vils[i].y == COLS - 2)
			vils[i].orientation *= (-1);
	}
}

void *p1_function(void *data)
{
	static int retval = 999;
	vil *vils;
	vils = ((vil *)data); // F 구조체배열 받아오기

	while (1)
	{
		if (crash_flag == 1)
			pthread_exit((void *)&retval);
		if (t < 6)
		{
			while (!draw_flag)
			{
				pthread_mutex_lock(&mutex);
				draw_villians(vils);
				pthread_mutex_unlock(&mutex);
				draw_flag = 1;
			}
			pthread_mutex_lock(&mutex);
			move_villians(vils);
			pthread_mutex_unlock(&mutex);
			usleep(20000);
		}
		else if (t < 18)
		{
			while (!draw_flag)
			{
				pthread_mutex_lock(&mutex);
				draw_villians(vils);
				pthread_mutex_unlock(&mutex);
				draw_flag = 1;
			}
			pthread_mutex_lock(&mutex);
			move_villians(vils);
			pthread_mutex_unlock(&mutex);
			usleep(10000);
		}
		else if (t < 36)
		{
			while (!draw_flag)
			{
				pthread_mutex_lock(&mutex);
				draw_villians(vils);
				pthread_mutex_unlock(&mutex);
				draw_flag = 1;
			}
			pthread_mutex_lock(&mutex);
			move_villians(vils);
			pthread_mutex_unlock(&mutex);
			usleep(5000);
		}
		else if (t < 60)
		{
			while (!draw_flag)
			{
				pthread_mutex_lock(&mutex);
				draw_villians(vils);
				pthread_mutex_unlock(&mutex);
				draw_flag = 1;
			}
			pthread_mutex_lock(&mutex);
			move_villians(vils);
			pthread_mutex_unlock(&mutex);
			usleep(2500);
		}
	}
}

void *p2_function(void *data)
{
	vil *vils;
	vils = ((vil *)data); // F 구조체배열 받아오기
	move_man(vils);
}

void init_keyboard()
{
	tcgetattr(0, &initial_settings);
	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_cc[VMIN] = 1;
	new_settings.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &new_settings);
}

void close_keyboard()
{
	tcsetattr(0, TCSANOW, &initial_settings);
}

int _getch()
{
	char ch;

	if (peek_character != -1)
	{
		ch = peek_character;
		peek_character = -1;
		return ch;
	}

	read(0, &ch, 1);
	return ch;
}

int _kbhit()
{
	unsigned char ch;
	int nread;

	if (peek_character != -1)
		return 1;

	new_settings.c_cc[VMIN] = 0;
	tcsetattr(0, TCSANOW, &new_settings);
	nread = read(0, &ch, 1);
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &new_settings);

	if (nread == 1)
	{
		peek_character = ch;
		return 1;
	}

	return 0;
}

int x = START_X; //student's x position
int y = START_Y; //student's y position

void move_man(vil *vils) //call this function when create and move student
{
	int x_prev = x, y_prev = y;
	int ch, i = 0;
	init_keyboard();
	static int retval = 999;

	x = 12;
	y = 1;
	pthread_mutex_lock(&mutex);
	move(x, y);
	addstr("O");
	pthread_mutex_unlock(&mutex);
	while (1)
	{
		if (crash_flag == 1)
			pthread_exit((void *)&retval);
		pthread_mutex_lock(&mutex);
		move(x, y);
		addstr("O");
		if (_kbhit())
		{

			ch = _getch();

			switch (ch)
			{
			case 'w': //up
				if (x != 12)
				{
					y_prev = y;
					x_prev = x;
					x--;
				}
				break;
			case 's': //down
				if (x != LINES - 2)
				{
					y_prev = y;
					x_prev = x;
					x++;
				}
				break;
			case 'a': //left
				if (y != 1)
				{
					x_prev = x;
					y_prev = y;
					y--;
				}
				break;
			case 'd': //right
				if (y != COLS - 2)
				{
					x_prev = x;
					y_prev = y;
					y++;
				}
				break;
			}
		}
		refresh();
		move(x_prev, y_prev);
		addstr(" ");
		pthread_mutex_unlock(&mutex);
		crash(vils);
	}
	close_keyboard();
}

void crash(vil *vils)
{
	int i = 0;
	for (i = 0; i < vil_num; i++)
		if (x == vils[i].x && y == vils[i].y)
			crash_flag = 1;
}

int main()
{
	int i;

	int flag = 0; // 시작지점 왼쪽 오른쪽 결정

	int x0 = 12, nlines = LINES - 12, y0 = 0, ncols = COLS - 1; // new window의 꼭짓점들

	int draw_flag = 0; //villain들이 draw된 횟수 (0이면 한번 그리고 1이면 그만 그려)

	pthread_t p_thread[2];

	int thr_id[2];

	int status;

	initscr();   // reset curses
	clear();	 // window clear
	curs_set(0); // curser invisible
	noecho();	// echo invisible

	srand(time(NULL)); // 랜덤함수

	for (i = 1; i < COLS - 1; i++)
	{
		move(0, i);
		addch('=');
		move(6, i);
		addch('=');
		move(8, i);
		addch('=');
		move(11, i);
		addch('=');
		move(LINES - 1, i);
		addch('=');
	}
	for (i = 0; i < LINES; i++)
	{
		move(i, 0);
		addch('|');
		move(i, COLS - 1);
		addch('|');
	}
	move(2, 5);
	addstr("Grade Theft A");
	move(4, 5);
	addstr("This game is to get A grade while avoiding F grades");
	refresh();
	//--------------------------------------------------------------------------
	vil *vils = (vil *)malloc(sizeof(vil) * vil_num);

	//time calculate and step appear
	s = clock();

	thr_id[0] = pthread_create(&p_thread[0], NULL, p1_function, (void *)&vils);

	if (thr_id[0] < 0)
	{
		perror("thread create error : ");
		exit(0);
	}

	thr_id[1] = pthread_create(&p_thread[1], NULL, p2_function, (void *)&vils);

	if (thr_id[1] < 0)
	{
		perror("thread create error : ");
		exit(0);
	}

	while (1)
	{
		if (crash_flag == 1)
			break;
		pthread_mutex_lock(&mutex);
		move(7, 5);
		if (t < 6)
			printw("| Stage %d | Time %3d |", 1, t);
		else if (t < 18)
			printw("| Stage %d | Time %3d | Speed X 2 |", 2, t);
		else if (t < 36)
			printw("| Stage %d | Time %3d | Speed X 2 |", 3, t);
		else if (t < 60)
			printw("| Stage %d | Time %3d | Speed X 2 |", 4, t);
		else
		{
			pthread_mutex_unlock(&mutex);
			crash_flag = 1;
			break;
		}
		refresh();
		pthread_mutex_unlock(&mutex);

		f = clock();
		t = (int)(f - s) / 1000000;
	}
	
	game_end();

	free(vils);

}

void game_end()
{
	initscr(); // reset curses
	clear();   // window clear
	pthread_mutex_lock(&mutex);
	move(9, 5);
	if (t < 6)
		printw("You got F grade, time : %d", t);
	else if (t < 18)
		printw("You got D grade, time : %d", t);
	else if (t < 36)
		printw("You got C grade, time : %d", t);
	else if (t < 60)
		printw("You got B grade, time : %d", t);
	else
		printw("You got A grade, Congratulations");

	move(10, 5);
	printw("When you press 'q'key, you get out.");
	pthread_mutex_unlock(&mutex);
	refresh();
	while (1)
	{
		if (getch() == 'q')
			break;
	}
	endwin(); // Restore normal terminal behavior
	echo();
	close_keyboard();

	exit(0);
}
