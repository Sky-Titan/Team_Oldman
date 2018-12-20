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

typedef struct villian vil; // "F" structure name

clock_t s, f; // 시간잼수

int t = 0, draw_flag = 0;

#define vil_num LINES - 13 // Villian 개수

void move_villians(vil *vils);

void draw_villians(vil *vils);

void *p1_function(void *data);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 쓰레드 초기화

//game window LeftUp(12,1)~RightLow(LINE-2,COLS-2)

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
		
		// pthread_mutex_lock(&mutex);
		mvprintw(vils[i].x, vils[i].y, "F");
		// pthread_mutex_unlock(&mutex);
	}
}

void move_villians(vil *vils)
{
	int i;

	for (i = 0; i < vil_num; i++)
	{
		// pthread_mutex_lock(&mutex);
		mvprintw(vils[i].x, vils[i].y, " ");
		// pthread_mutex_unlock(&mutex);
		refresh;
		vils[i].y += vils[i].orientation;
		// pthread_mutex_lock(&mutex);
		mvprintw(vils[i].x, vils[i].y, "F");
		// pthread_mutex_unlock(&mutex);
		refresh;
		if (vils[i].y == 1 || vils[i].y == COLS - 2)
			vils[i].orientation *= (-1);
	}
}

void *p1_function(void *data)
{
	vil *vils;
	vils = ((vil *)data); // F 구조체배열 받아오기

	while (1)
	{
		if(t<6){
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
			usleep(80000);
		}
		else if(t<18)
		{
			while(!draw_flag)
			{
				pthread_mutex_lock(&mutex);
				draw_villians(vils);
                                pthread_mutex_unlock(&mutex);
                                draw_flag = 1;
                        }
                        pthread_mutex_lock(&mutex);
                        move_villians(vils);
                        pthread_mutex_unlock(&mutex);
                        usleep(40000);
		}
		else if(t<36)
		{
			while(!draw_flag)
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
		else if(t<60)
		{
		 	while(!draw_flag)
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
	}
}

int main()
{
	int i;

	int level[4] = {1, 2, 3, 4};
	int levels = 0;

	int flag = 0; // 시작지점 왼쪽 오른쪽 결정

	int x0 = 12, nlines = LINES - 12, y0 = 0, ncols = COLS - 1; // new window의 꼭짓점들

	int draw_flag = 0; //villain들이 draw된 횟수 (0이면 한번 그리고 1이면 그만 그려)

	pthread_t p_thread[1];

	int thr_id[1];

	int thr_flag[1] = {
		0,
	}; // thread 실행횟수

	int status;

	initscr();   // reset curses
	clear();	 // window clear
	curs_set(0); // curser invisible
	noecho();	// echo invisible

	srand(time(NULL)); // 랜덤함수

	// WINDOW *win = newwin(LINES, COLS, x0, y0);

	// 12, LINES
	//--------------------------------------------------------------------
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
	//--------------------------------------------------------------------------
	vil *vils = (vil *)malloc(sizeof(vil) * vil_num);

	//time calculate and step appear
	s = clock();

	thr_id[0] = pthread_create(&p_thread[0], NULL, p1_function, (void *)&vils);

	if(thr_id[0] < 0)
	{
		perror("thread create error : ");
		exit(0);
	}

	while(1)
	{
		pthread_mutex_lock(&mutex);
		move(7, 5);
		if(t<5)	
			printw("| Stage %d | Time %3d |", 1, t);
		else if (t < 6)
		 	printw("| Stage %d | Time %3d | Boss Detected |", 1, t);
		else if (t < 16)
		 	printw("| Stage %d | Time %3d |                    ", 2, t);
		else if (t < 18)
		 	printw("| Stage %d | Time %3d | Boss Detected |", 2, t);
		else if (t < 33)
		 	printw("| Stage %d | Time %3d |                    ", 3, t);
		else if (t < 36)
		 	printw("| Stage %d | Time %3d | Boss Detected |", 3, t);
		else if (t < 56)
		 	printw("| Stage %d | Time %3d |                    ", 4, t);
		else if (t < 60)
		 	printw("| Stage %d | Time %3d | Boss Detected |", 4, t);
		else break;
		refresh();
		pthread_mutex_unlock(&mutex);
		f = clock();
		t = (int)(f - s) / 1000000;
	}
	//result print
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
	printw("When you press any key, you get out.");

	refresh();
	getch();
	endwin(); // Restore normal terminal behavior
	echo();

	free(vils);
}
