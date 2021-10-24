#include "startmenu.h"
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
		printf("PRESS G TO START");
}