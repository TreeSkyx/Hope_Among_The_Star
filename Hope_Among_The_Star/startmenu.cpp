#define _CRT_SECURE_NO_WARNINGS 1
#include "startmenu.h"
#include "fileWrite.h"
#include "cursorSetting.h"
#define screen_x 100
#define screen_y 30
#define maxS 50
#include <stdio.h>
#include <windows.h>
int  i = 26;
int shipX = 7;
int jumpCount = 0;
int jumpCountE[3] = {0,0,0};
COORD enemy[3] = { {6,20},{10,20},{8,18}};
COORD starS[maxS];
int state = 0;
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
		starShip_draw(47, 5);
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
		setcolor(7, 0);
		cursorPos(40,8);
		printf("HOPE AMONG THE STAR");
		cursorPos(42, 15);
		printf("PRESS G : START");
		cursorPos(42, 17);
		printf("PRESS B : LEADER BOARD");
		cursorPos(42, 19);
		printf("PRESS C : CREDIT");
		Sleep(200);
}
void credit_page() {
	if (state == 5) {
		init_starS();
		state--;
	}
	setcolor(7, 0);
	cursorPos(45, 8);
	printf("CREDIT");
	cursorPos(39, 9);
	printf("------------------");
	cursorPos(42, 10);
	printf("Game Creator");
	cursorPos(35, 12);
	printf("64010815 WIROON SOMPHAOTHONG");
	cursorPos(36, 14);
	printf("COMPUTER ENGINEERING,KMITL");
	cursorPos(15, 28);
	printf("This project is the part of Programming Fundamental Class | CE, KMITL");
}
void playerName() {
	setcolor(7, 0);
	cursorPos(43, 8);
	printf("ENTER YOUR NAME");
	cursorPos(43, 10);
	printf("---------------");
	cursorPos(43, 12);
}
void highScore() {
	if (state == 5) {
		init_starS();
		state--;
	}
	cursorPos(44, 8);
	printf("Leader Board");
	cursorPos(41, 10);
	printf("------------------");
	scoreRead();

}
void gameEnd_page(char n[20], int lv,int sc) {
	setcolor(2, 0);
	cursorPos(45, 8);
	printf("GAME OVER");
	cursorPos(41, 9);
	printf("=================");
	cursorPos(45, 10);
	printf("Your Score");
	cursorPos(31, 12);
	printf("Name : %s\tWave : %d\tScore : %d", n, lv, sc);
}
void gameWinner_page(char n[20], int lv, int sc) {
	setcolor(2, 0);
	cursorPos(40, 8);
	printf("THANK YOU FOR SAVING US");
	cursorPos(40, 9);
	printf("=======================");
	cursorPos(45, 10);
	printf("Your Score");
	cursorPos(32, 12);
	printf("Name : %s\t Wave : %d\tScore : %d", n, lv, sc);
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
void init_starS() {
	for (int i = 0; i < maxS; i++) {
		starS[i] = { SHORT(rand() % (((screen_x-2) - 2) - 2 + 1) + 2),SHORT(rand() % (((screen_y - 2) - 2) - 2 + 1) + 2)};
		cursorPos(starS[i].X, starS[i].Y);
		setcolor(7, 0);
		printf("*");
	}

}