#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>

struct villian
{ // f구조체
	int x, y;
	int orientation;
};

typedef struct villian vil;

clock_t s, f;

#define vil_num LINES - 13

// void ef_spawnpos(ef *f); // f 스폰위치, 컬러
// void ef_speed(ef *f, int level); // f 스피드
void gotoxy(int x, int y); // 좌표이동
void textcolor(int color); // 컬러

//game window LeftUp(12,1)~RightLow(LINE-2,COLS-2)


void draw_villians(vil *vils)
{
	int i, flag=0;

	for (i = 0; i < vil_num; i++)
	{
		vils[i].x = i + 12;

		flag = rand() % 2;

		vils[i].y = rand() % (COLS-3) + 1;

		if (flag == 0)
			vils[i].orientation = 1; // 오른쪽으로 이동
		else if(flag == 1)
			vils[i].orientation = -1; // 왼쪽으로 이동

		mvprintw(vils[i].x, vils[i].y, "F");
	}
}

void move_villians(vil *vils)
{
	int i;

	for(i=0; i<vil_num; i++){
		mvprintw(vils[i].x, vils[i].y, " ");
		refresh;
		vils[i].y += vils[i].orientation;
		mvprintw(vils[i].x, vils[i].y, "F");
		refresh;
		if(vils[i].y==1 || vils[i].y==COLS-2)
			vils[i].orientation *= (-1);
		
	}
}

int main()
{
	int i, t = 0;
	// clock_t s, f;

	int level[4] = {1, 2, 3, 4};
	int levels = 0;

	int flag = 0; // 시작지점 왼쪽 오른쪽 결정

	int x0 = 12, nlines = LINES - 12, y0 = 0, ncols = COLS - 1; // new window의 꼭짓점들

	int draw_flag = 0 ; //villain들이 draw된 횟수 (0이면 한번 그리고 1이면 그만 그려)

	pthread_t p_thread[1];

	// int vil_num = LINES-12; // F 개수

	// vil vils[vil_num]; // 악당 구조체 배열 50개

	initscr();   // reset curses
	clear();	 // window clear
	curs_set(0); // curser invisible
	noecho();	// echo invisible

	srand(time(NULL)); // 랜덤함수

	// WINDOW *win = newwin(LINES, COLS, x0, y0);

	// 12, LINES

	// window frame
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
	addstr("Grade Theft Auto");
	move(4, 5);
	addstr("This game is to get A grade while avoiding F grades");
	refresh();

	vil *vils = (vil *)malloc(sizeof(vil) * vil_num);

	//time calculate and step appear
	s = clock();

	// villians appear
	// while(1){

	// }
	// draw_villians(vils);
	while (1)
	{
		move(7, 5);
		if (t < 50)
		{
			printw("| Stage %d | Time %3d |", level[0], t);
			while(!draw_flag)
			{
				draw_villians(vils);
				draw_flag=1;
			}
			move_villians(vils);
			sleep(1);
		}
		else if (t < 60)
			printw("| Stage %d | Time %3d | Boss Detected |", level[1], t);
		else if (t < 160)
			printw("| Stage %d | Time %3d |                    ", 2, t);
		else if (t < 180)
			printw("| Stage %d | Time %3d | Boss Detected |", level[2], t);
		else if (t < 330)
			printw("| Stage %d | Time %3d |                    ", 3, t);
		else if (t < 360)
			printw("| Stage %d | Time %3d | Boss Detected |", level[3], t);
		else if (t < 560)
			printw("| Stage %d | Time %3d |                    ", 4, t);
		else if (t < 600)
			printw("| Stage %d | Time %3d | Boss Detected |", level[4], t);
		else
			break;
		refresh();
		f = clock();
		t = (int)(f - s) / 1000000;
	}
	//result print
	move(9, 5);
	if (t < 60)
		printw("You got F grade, time : %d", t);
	else if (t < 180)
		printw("You got D grade, time : %d", t);
	else if (t < 360)
		printw("You got C grade, time : %d", t);
	else if (t < 600)
		printw("You got B grade, time : %d", t);
	else
		printw("You got A grade, Congratulations");

	move(10, 5);
	printw("When you press any key, you get out.");

	refresh();
	getch();
	endwin(); // Restore normal terminal behavior
	echo();

	free(vils);
}
