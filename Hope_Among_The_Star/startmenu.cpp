#define _CRT_SECURE_NO_WARNINGS 1
#include "startmenu.h"
#include "fileWrite.h"
#include "cursorSetting.h"
#define screen_x 100
#define screen_y 30
#define maxS 50
#include <stdio.h>
#include <windows.h>
#include <time.h>
int  i = 26;
int shipX = 7;
int exitCount = 0;
int jumpCount = 0;
int jumpCountE[3] = {0,0,0};
COORD enemy[3] = { {6,20},{10,20},{8,18}};
COORD starS[maxS];
COORD laser[5] = { {3,1},{20,15},{43,10},{67,22},{89,5} };
int state = 0;
void random() {
	srand(time(NULL));
}
void start_page() {
	int x = 0, y = 0;
		if (state == 0) {
			for (x = 0; x < screen_x; x++) {
				cursorPos(x, 0);
				setcolor(7, 7);
				printf("_");
				Sleep(10);
			}
			state++;
		}
		if (state == 1) {
			for (y = 0; y < screen_y - 1; y++) {
				cursorPos(screen_x - 1, y);
				setcolor(7, 7);
				printf("|");
				Sleep(10);
			}
			state++;
		}
		if (state == 2) {
			for (x = screen_x - 1; x >= 0; x--) {
				cursorPos(x, screen_y - 1);
				setcolor(7, 7);
				printf("_");
				Sleep(10);
			}
			state++;
		}
		if (state == 3) {
			for (y = screen_y - 1; y >= 0; y--) {
				cursorPos(0, y);
				setcolor(7, 7);
				printf("|");
				Sleep(10);
			}
			state++;
		}
		if (state == 4) {
			init_starS();
			state++;
		}
		enemyShip_draw(enemy[0].X, enemy[0].Y-=1);
		enemyShip_draw(enemy[1].X, enemy[1].Y -= 1);
		enemyShip_draw(enemy[2].X, enemy[2].Y -= 1);
		for (int i = 0; i < 5; i++) {
			laser_draw(laser[i].X, laser[i].Y += 1);
		}
		starShip_draw(48, 5);
		starShip_draw(shipX, i -= 1);
		for (int k = 0; k < 3; k++) {
			if (enemy[k].Y == 5) {
				deleteEnemy(enemy[k].X, enemy[k].Y - 1);
				jumpCountE[k]++;
				if (jumpCountE[k] == 3) {
					enemy[k].X += 40;
				}
				else if (jumpCountE[k] == 5) {
					if(k==0) enemy[k].X = 6;
					if(k==1) enemy[k].X = 10;
					if (k == 2) enemy[k].X = 8;
					jumpCountE[k] = 0;
				}
				else { enemy[k].X += 10; }
				enemy[k].Y = 28;
			}
		}
		if (i == 5) {
			deleteShip(shipX-1,i);
			jumpCount++;
			if (jumpCount == 5) {
				shipX = 7;
				jumpCount = 0;
			}
			else if (jumpCount == 3) {
				shipX = shipX + 40;
			}
			else { shipX = shipX + 10; }
			i = 28;
		}
		for (int i = 0; i < 5; i++) {
			if (laser[i].Y == 27) {
				deletelaser(laser[i].X, laser[i].Y + 1);
				laser[i] = { SHORT(rand() % (screen_x - 3) + 2),1 };
			}
		}
		setcolor(3, 0);
		cursorPos(41,8);
		printf("HOPE AMONG THE STAR");
		setcolor(10, 0);
		cursorPos(43, 15);
		printf("PRESS G : START");
		setcolor(6, 0);
		cursorPos(43, 17);
		printf("PRESS B : LEADER BOARD");
		cursorPos(43, 19);
		printf("PRESS C : CREDIT");
		setcolor(4, 0);
		cursorPos(43, 21);
		printf("PRESS ESC : EXIT");
		Sleep(200);
}
void credit_page() {
	if (state == 5) {
		init_starS();
		state--;
		setcolor(0, 0);
		for (int y = 7; y < 16; y++) {
			for (int x = 33; x < 63; x++) {
				cursorPos(x, y);
				printf(" ");
			}
		}
	}
	setcolor(0, 0);
	setcolor(6, 0);
	cursorPos(47, 8);
	printf("CREDIT!");
	cursorPos(39, 9);
	printf("=======================");
	setcolor(14, 0);
	cursorPos(44, 10);
	printf("-Game Creator");
	setcolor(3, 0);
	cursorPos(36, 12);
	printf("64010815 WIROON SOMPHAOTHONG");
	setcolor(9, 0);
	cursorPos(37, 14);
	printf("COMPUTER ENGINEERING,KMITL");
	setcolor(8, 0);
	cursorPos(15, 28);
	printf("This project is the part of Programming Fundamental Class | CE, KMITL");
}
void playerName() {
	if (state == 5) {
		init_starS();
		state--;
		setcolor(0, 0);
		for (int y = 7; y < 19; y++) {
			for (int x = 38; x < 63; x++) {
				cursorPos(x, y);
				printf(" ");
			}
		}
	}
	setcolor(2, 0);
	cursorPos(43, 8);
	printf("ENTER YOUR NAME");
	cursorPos(40, 10);
	printf("Max : 15 Characters!");
	cursorPos(39, 11);
	printf("=======================");
	cursorPos(43, 13);
}
void highScore() {
	if (state == 5) {
		init_starS();
		state--;
	}
	cursorPos(45, 8);
	setcolor(2, 0);
	printf("Leaderboard");
	cursorPos(39, 10);
	printf("=======================");
	scoreRead();

}
void gameEnd_page(char n[20], int lv,int sc) {
	setcolor(0, 0);
	for (int y = 7; y < 19; y++) {
		for (int x = 38;x < 63; x++) {
			cursorPos(x, y);
			printf(" ");
		}
	}
	setcolor(7, 0);
	cursorPos(46, 8);
	printf("GAME OVER");
	setcolor(4, 0);
	cursorPos(39, 9);
	printf("=======================");
	cursorPos(46, 10);
	setcolor(7, 0);
	printf("Your Score");
	setcolor(4, 0);
	cursorPos(42, 12);
	printf("Name : %s", n);
	cursorPos(42, 14);
	printf("Wave : %d", lv);
	cursorPos(42, 16);
	printf("Score : %d", sc);
	setcolor(7, 0);
	cursorPos(43, 18);
	printf("PRESS X TO EXIT");
	for (int i = 0; i < 76; i++) {
		setcolor(1, 0);
		cursorPos(i, 20);
		printf(" !!GAME OVER!! |-- <-0-> ");
		Sleep(200);
		if (i == 75) {
			exitCount++;
			for (i = 75; i >= 0; i--) {
				setcolor(4, 0);
				cursorPos(i, 20);
				printf(" <-0-> --| !!GAME OVER!! ");
				Sleep(200);
			}
		}
	}
}
void gameWinner_page(char n[20], int lv, int sc) {
	setcolor(0, 0);
	for (int y = 7; y < 19; y++) {
		for (int x = 38; x < 63; x++) {
			cursorPos(x, y);
			printf(" ");
		}
	}
	setcolor(7, 0);
	cursorPos(39, 8);
	printf("THANK YOU FOR SAVING US");
	setcolor(2, 0);
	cursorPos(39, 9);
	printf("=======================");
	cursorPos(46, 10);
	setcolor(7, 0);
	printf("Your Score");
	setcolor(2, 0);
	cursorPos(39, 12);
	printf("Name : %s", n);
	cursorPos(39, 14);
	printf("Wave : %d", lv);
	cursorPos(39, 16);
	printf("Score : %d", sc);
	setcolor(7, 0);
	cursorPos(43, 18);
	printf("PRESS X TO EXIT");
	for (int i = 0; i < 70; i++) {
		setcolor(6, 0);
		cursorPos(i, 20);
		printf(" !!MISSION COMPLETE!! |-- <-0-> ");
		Sleep(200);
		if (i == 69) {
			for (i = 69; i >= 0; i--) {
				setcolor(3, 0);
				cursorPos(i, 20);
				printf(" <-0-> --| !!MISSION COMPLETE!! ");
				Sleep(200);
		}
		}
	}
}
void starShip_draw(int x,int y) {
	deleteShip(x--, y--);
	cursorPos(x, y);
	setcolor(0, 0); printf(" ");
	setcolor(6, 4); printf("^");
	setcolor(7, 7); printf(" ");
	setcolor(7, 1); printf(" ");
	setcolor(7, 7); printf(" ");
	setcolor(6, 4); printf("^");
	setcolor(0, 0); printf(" ");
	cursorPos(x+1, y-1);
	setcolor(0,0); printf(" ");
	setcolor(7, 7); printf(" ");
	setcolor(7, 9); printf(" ");
	setcolor(7, 7); printf(" ");
	setcolor(0, 0); printf(" ");
	cursorPos(x+2, y-2);
	setcolor(0, 0); printf(" ");
	setcolor(6, 4); printf("^");
	setcolor(0, 0); printf(" ");
	cursorPos(x+1, y+1);
	setcolor(0, 0); printf(" ");
	setcolor(6, 0); printf("W");
	setcolor(0, 0); printf(" ");
	setcolor(6, 0); printf("W");
	setcolor(0, 0); printf(" ");
}
void deleteShip(int x, int y) {
	cursorPos(x, y);
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	cursorPos(x + 1, y - 1);
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	cursorPos(x + 2, y - 2);
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	cursorPos(x + 3, y - 3);
	setcolor(0, 0); printf(" ");
	cursorPos(x + 1, y + 1);
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
}
void enemyShip_draw(int x, int y) {
	deleteEnemy(x, y--);
	cursorPos(x, y);
	setcolor(2, 0); printf("<");
	setcolor(2, 5); printf("0");
	setcolor(2, 0); printf(">");
}
void deleteEnemy(int x, int y) {
	cursorPos(x, y);
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
	setcolor(0, 0); printf(" ");
}
void laser_draw(int x, int y) {
	deletelaser(x, y++);;
	cursorPos(x, y);
	setcolor(0, 3); printf("V");

}
void deletelaser(int x, int y) {
	cursorPos(x, y);
	setcolor(0, 0); printf(" ");
}
void init_starS() {
	for (int i = 0; i < maxS; i++) {
		starS[i] = { SHORT(rand() % (((screen_x-2) - 2) - 2 + 1) + 2),SHORT(rand() % (((screen_y - 2) - 2) - 2 + 1) + 2)};
		cursorPos(starS[i].X, starS[i].Y);
		setcolor(7, 0);
		printf("*");
	}

}