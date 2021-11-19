#define _CRT_SECURE_NO_WARNINGS 1
#include "startmenu.h"
#include "fileWrite.h"
#include "cursorSetting.h"
#define screen_x 100
#define screen_y 30
#include <stdio.h>
#include <windows.h>
void start_page() {
	int state = 0;
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
		setcolor(7, 0);
		cursorPos(40,8);
		printf("HOPE AMONG THE STAR");
		cursorPos(42, 15);
		printf("PRESS G : START");
		cursorPos(42, 17);
		printf("PRESS B : LEADER BOARD");
		cursorPos(42, 19);
		printf("PRESS C : CREDIT");
}
void credit_page() {
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
}
void playerName() {
	setcolor(7, 0);
	cursorPos(40, 8);
	printf("ENTER YOUR NAME");
	cursorPos(40, 10);
	printf("---------------");
	cursorPos(40, 12);
}
void highScore() {
	cursorPos(40, 8);
	printf("Leader Board");
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