#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <time.h>

//game window LeftUp(12,1)~RightLow(LINE-2,COLS-2)

int main()
{
	int i, t=0;
	clock_t s,f;	
	
	initscr();	// reset curses
	clear();	// window clear
	curs_set(0);	// curser invisible
	noecho();	// echo invisible

	// window frame
	for(i=1;i<COLS-1;i++)
	{
		move(0,i);
		addch('=');
		move(6,i);
     	       	addch('=');
		move(8,i);
		addch('=');
		move(11,i);
		addch('=');
		move(LINES-1,i);
		addch('=');
	}
	for(i=0;i<LINES;i++)
	{
		move(i,0);
		addch('|');
		move(i,COLS-1);
		addch('|');
	}
	move(2,5);
	addstr("Grade Theft Auto");
	move(4,5);
	addstr("This game is to get A grade while avoiding F grades");
	refresh();

	//time calculate and step appear
	s = clock();
	while(1){		
		move(7,5);
		if(t<50)
               		printw("| Stage %d | Time %3d |",1,t);
		else if(t<60)
			printw("| Stage %d | Time %3d | Boss Detected |",1,t); 
		else if(t<160)
			printw("| Stage %d | Time %3d |                    ",2,t);
		else if(t<180)
			printw("| Stage %d | Time %3d | Boss Detected |",2,t);
		else if(t<330)
			printw("| Stage %d | Time %3d |                    ",3,t);
		else if(t<360)
			printw("| Stage %d | Time %3d | Boss Detected |",3,t);
		else if(t<560)
			printw("| Stage %d | Time %3d |                    ",4,t);
		else if(t<600)
			printw("| Stage %d | Time %3d | Boss Detected |",4,t);
		else
			break;
		refresh();
		f = clock();
	 	t = (int)(f-s)/1000000;
	}
	//result print
	move(9,5);
	if(t<60)
		printw("You got F grade, time : %d",t);
	else if(t<180)
		printw("You got D grade, time : %d",t);
	else if(t<360)
		printw("You got C grade, time : %d",t);
	else if(t<600)
		printw("You got B grade, time : %d",t);
	else
		printw("You got A grade, Congratulations");
	
	move(10,5);
	printw("When you press any key, you get out.");	
	
	refresh();
	getch();
	endwin();
	echo();	
}
