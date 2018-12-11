#include<termios.h>
#include<curses.h>
#include<stdio.h>
#include<signal.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

#define START_X 0
#define START_Y 0
static struct termios initial_settings, new_settings;
static int peek_character = -1;

void init_keyboard(){

	tcgetattr(0,&initial_settings);
	new_settings = initial_settings;
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~ECHO;
	new_settings.c_cc[VMIN] = 1;
	new_settings.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &new_settings);
}


void close_keyboard(){

	tcsetattr(0, TCSANOW, &initial_settings);
	
}

void finish_loop()
{
        endwin();
        close_keyboard();
        exit(0);
}


int _kbhit(){

	unsigned char ch;
	int nread;

	if (peek_character != -1) 
	return 1;

	new_settings.c_cc[VMIN]=0;
	tcsetattr(0, TCSANOW, &new_settings);
	nread = read(0,&ch,1);
	new_settings.c_cc[VMIN]=1;
	tcsetattr(0, TCSANOW, &new_settings);
	
	if(nread == 1){
		peek_character = ch;
		return 1;
	}

	return 0;

}



int _getch()
{

	char ch;

	if(peek_character != -1) 
	{

		ch = peek_character;
		peek_character = -1;
		return ch;
	}

	read(0,&ch,1);
	return ch;
}
int _putch(int c) {

	putchar(c);
	fflush(stdout);
	return c;
}
int x=START_X;//student's x position
int y=START_Y;//student's y position

void move_man()//call this function when create and move student
{
	int x_prev=x,y_prev=y;
	int ch,i=0;;
	init_keyboard();
	
	initscr();
	clear();
	signal(SIGINT,finish_loop);
	move(x,y);
	addstr("O");
	while (1) {
		move(x,y);
		addstr("O");
		if (_kbhit()) {

			ch = _getch();
		
			//_putch(ch);
			
			switch(ch){
			
				case 'w'://up
					if(x!=0)
					{
						y_prev=y;
						x_prev=x;
						x--;
					}
					break;
				case 's'://down
						y_prev=y;
						x_prev=x;
						x++;
					break;
				case 'a'://left
					if(y!=0)
					{
						x_prev=x;
						y_prev=y;
						y--;
					}
					break;
				case 'd'://right
						x_prev=x;
						y_prev=y;
						y++;
					break;
			}
		}
		refresh();
		move(x_prev,y_prev);
		addstr(" ");
		

	}	
	endwin();
	close_keyboard();

}
int main()
{
	
	move_man();
	
	return 0;
}
