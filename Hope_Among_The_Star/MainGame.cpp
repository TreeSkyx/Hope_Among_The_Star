#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <string.h>
#include "gameFont.h"
#include "startmenu.h"
#include "fileWrite.h"
#include "cursorSetting.h"
#define max_star 30 
#define max_item 5 
#define screen_x 100
#define screen_y 30
#define bullet_amount 6
#define enemy_bullet_amount 3
//windows_setting
HANDLE rHnd;
HANDLE wHnd;
DWORD fdwMode;
CHAR_INFO consoleBuffer[screen_x * screen_y];
COORD bufferSize = { screen_x,screen_y };
COORD characterPos = { 0,0 };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
//game_var.
bool play = true;
bool pause = false;
bool start = false;
int p_count = 0;
int score = 0;
int lifepoint = 5;
int MSlifepoint = 30;
int wave_state = 0;
int gCount = 0;
int bCount = 0;
int cCount = 0;
bool clr_state = true;
//item var.
int item_id[max_item];
int upRate = 25;
int shield_item = 0;
int shield_state = 0;
COORD shield = { 10,1 };
int heal_state = 0;
COORD heal = { 20,1 };
int bomb_state = 0;
COORD bomb = { 10,1 };
int extraBullet_state = 0;
COORD extraB = { 10,1 };
//ship var.
COORD mainShip = { 32,26 };
char direct = 'n';
//player var.
char pName[20];
//bullet var.
int extraB_plus = 0;
int clickStat = 0;
int bulletState[bullet_amount+3];
int bcount = 0;
COORD bulletPos[bullet_amount+3];
//wave & star var.
int star_id[max_star];
int star_des = 0;
int star_state[max_star];
int wave = 1;
int wave_star[11] = {0,10,15,10,10,10,10,10,15,15,0};
int enemy_left;
int fighter_hp[max_star];
int destoryer_hp[max_star];
int normalEnemy_count = 0;
int fighterEnemy_count = 0;
int destoryerEnemy_count = 0;
int enemy_speed = 1;
COORD star[max_star];
//leaderboard var.
char topName[5][20] = { {},{},{},{},{}};
int topLevel[5] = { 0,0,0,0,0 };
int topScore[5]  = { 0,0,0,0,0 };
void setcursor(bool visible) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
int setConsole(int x, int y) {
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}
int setMode() {
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_ECHO_INPUT | ENABLE_INSERT_MODE | ENABLE_LINE_INPUT ;
	SetConsoleMode(rHnd, fdwMode);
	return 0;
}
void draw_ship_to_buffer(COORD ship) {
	consoleBuffer[ship.X + screen_x * ship.Y].Char.AsciiChar = ' ';
	consoleBuffer[ship.X - 1 + screen_x * ship.Y].Char.AsciiChar = '_';
	consoleBuffer[ship.X + 1 + screen_x * ship.Y].Char.AsciiChar = '_';
	consoleBuffer[ship.X - 2 + screen_x * ship.Y].Char.AsciiChar = '^';
	consoleBuffer[ship.X + 2 + screen_x * ship.Y].Char.AsciiChar = '^';
	consoleBuffer[ship.X + screen_x * int(ship.Y - 1)].Char.AsciiChar = ' '; 
	consoleBuffer[ship.X + 1 + screen_x * int(ship.Y - 1)].Char.AsciiChar = ' ';
	consoleBuffer[ship.X - 1 + screen_x * int(ship.Y - 1)].Char.AsciiChar = ' ';
	consoleBuffer[ship.X + screen_x * int(ship.Y - 2)].Char.AsciiChar = '^'; 
	consoleBuffer[ship.X - 1 + screen_x * int(ship.Y + 1)].Char.AsciiChar = 'W'; 
	consoleBuffer[ship.X + 1 + screen_x * int(ship.Y + 1)].Char.AsciiChar = 'W'; 
	consoleBuffer[ship.X + screen_x * ship.Y].Attributes = 23;
	consoleBuffer[ship.X + 1 + screen_x * ship.Y].Attributes = 120;//white
	consoleBuffer[ship.X - 1 + screen_x * ship.Y].Attributes = 120;
	consoleBuffer[ship.X + 2 + screen_x * ship.Y].Attributes = 78;//red
	consoleBuffer[ship.X - 2 + screen_x * ship.Y].Attributes = 78;
	consoleBuffer[ship.X + 1 + screen_x * int(ship.Y - 1)].Attributes = 120;
	consoleBuffer[ship.X - 1 + screen_x * int(ship.Y - 1)].Attributes = 120;
	consoleBuffer[ship.X + screen_x * int(ship.Y - 1)].Attributes = 48;//light blue
	consoleBuffer[ship.X + screen_x * int(ship.Y - 2)].Attributes = 78;
	consoleBuffer[ship.X + 1 + screen_x * int(ship.Y + 1)].Attributes = 6;//yellow 
	consoleBuffer[ship.X - 1 + screen_x * int(ship.Y + 1)].Attributes = 6;
}
void shipMovement(char dir) {
	if (dir == 'l') {
		if (mainShip.X != 5) {
			mainShip.X--;
		}
		else { dir = 'n'; mainShip.X = 5; }
	}
	if (dir == 'r') {
		if (mainShip.X != screen_x - 6) {
			mainShip.X++;
		}
		else { dir = 'n'; mainShip.X = screen_x - 6; }
	}
	if (dir == 'n') {
		mainShip.X = mainShip.X;
		mainShip.Y = mainShip.Y;
	}
}
void wave_change(int S_destroy) {
	if (S_destroy == wave_star[wave]) {
		wave++;
		normalEnemy_count = 0;
		fighterEnemy_count = 0;
		destoryerEnemy_count = 0;
		if (wave == 3) { upRate *= 2; }
		if (wave == 5) { upRate *= 3; }
		if (wave == 7) { upRate *= 4; }
		star_des = 0;
		wave_state = 1;
	}
}
void itemFall() {
	if (shield_item == 1) {
		if (shield.Y >= 28) {
			shield_item = 0;
		}
		else {
			shield = { shield.X,SHORT(shield.Y + 1) };
		}
	}
	if (heal_state == 1) {
		if (heal.Y >= 28) {
			heal_state = 0;
		}
		else {
			heal = { heal.X,SHORT(heal.Y + 1) };
		}
	}
	if (bomb_state == 1) {
		if (bomb.Y >= 28) {
			bomb_state = 0;
		}
		else {
			bomb = { bomb.X,SHORT(bomb.Y + 1) };
		}
	}
	if (extraBullet_state == 1) {
		if (extraB.Y >= 28) {
			extraBullet_state = 0;
		}
		else {
			extraB = { extraB.X,SHORT(extraB.Y + 1) };
		}
	}
}
void init_item(){
	for (int i = 0; i < max_item; i++) {
		if (shield_item == 0 && shield_state == 0) {
			int j = (rand() % (225 - upRate));
			if (j == 2) {
				shield_item = 1;
				item_id[i] = 1;
				shield = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 };
			}
		}
		if (heal_state == 0) {
			if (lifepoint < 5 || MSlifepoint < 30) {
				int k = (rand() % (275 - upRate));
				if (k == 2) {
					heal_state = 1;
					item_id[i] = 2;
					heal = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 };
				}
			}
		}
		if (bomb_state == 0) {
			int l = (rand() % (125 - upRate));
			if (l == 2) {
				bomb_state = 1;
				item_id[i] = 3;
				bomb = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 };
			}
		}
		if (extraBullet_state == 0 && wave>4 && extraB_plus<3) {
			int l = (rand() % (500 - upRate));
			if (l == 2) {
				extraBullet_state = 1;
				item_id[i] = 4;
				extraB = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 };
			}
		}
	}
}
void fill_item_to_buffer(int id) {
	if (id == 1) {
		if (shield_item == 1) {
			consoleBuffer[shield.X + screen_x * shield.Y].Char.AsciiChar = 'S';
			consoleBuffer[shield.X + screen_x * shield.Y].Attributes = 48;
		}
		if (shield_state == 1) {
			consoleBuffer[mainShip.X + 5 + screen_x * mainShip.Y].Char.AsciiChar = '_';
			consoleBuffer[mainShip.X + 5 + screen_x * mainShip.Y].Attributes = 48;
			consoleBuffer[mainShip.X - 5 + screen_x * mainShip.Y].Char.AsciiChar = '_';
			consoleBuffer[mainShip.X - 5 + screen_x * mainShip.Y].Attributes = 48;
			consoleBuffer[mainShip.X + 4 + screen_x * int(mainShip.Y - 1)].Char.AsciiChar = '\\';
			consoleBuffer[mainShip.X - 4 + screen_x * int(mainShip.Y - 1)].Char.AsciiChar = '/';
			consoleBuffer[mainShip.X + 3 + screen_x * int(mainShip.Y - 2)].Char.AsciiChar = '\\';
			consoleBuffer[mainShip.X - 3 + screen_x * int(mainShip.Y - 2)].Char.AsciiChar = '/';
			consoleBuffer[mainShip.X + 2 + screen_x * int(mainShip.Y - 3)].Char.AsciiChar = '\\';
			consoleBuffer[mainShip.X - 2 + screen_x * int(mainShip.Y - 3)].Char.AsciiChar = '/';
			consoleBuffer[mainShip.X + 1 + screen_x * int(mainShip.Y - 4)].Char.AsciiChar = '\\';
			consoleBuffer[mainShip.X - 1 + screen_x * int(mainShip.Y - 4)].Char.AsciiChar = '/';
			consoleBuffer[mainShip.X + screen_x * int(mainShip.Y - 4)].Char.AsciiChar = '-';
			consoleBuffer[mainShip.X + screen_x * int(mainShip.Y - 4)].Attributes = 48;
		}
	}
	if (id == 2) {
		if (heal_state == 1) {
			consoleBuffer[heal.X + screen_x * heal.Y].Char.AsciiChar = 'H';
			consoleBuffer[heal.X + screen_x * heal.Y].Attributes = 160;
		}
	}
	if (id == 3) {
		if (bomb_state == 1) {
			consoleBuffer[bomb.X + screen_x * bomb.Y].Char.AsciiChar = 'B';
			consoleBuffer[bomb.X + screen_x * bomb.Y].Attributes = 71;
		}
	}
	if (id == 4) {
		if (extraBullet_state == 1) {
			consoleBuffer[extraB.X + screen_x * extraB.Y].Char.AsciiChar = 'E';
			consoleBuffer[extraB.X + screen_x * extraB.Y].Attributes = 135;
		}
	}
}
void scoreBoard(int s) {
	int u = 0, t = 0, h = 0, th = 0;
	th = s / 1000;
	h = (s % 1000) / 100;
	t = (s % 100) / 10;
	u = s % 10;
	consoleBuffer[83 + screen_x * 0].Char.AsciiChar = 'S';
	consoleBuffer[84 + screen_x * 0].Char.AsciiChar = 'C';
	consoleBuffer[85 + screen_x * 0].Char.AsciiChar = 'O';
	consoleBuffer[86 + screen_x * 0].Char.AsciiChar = 'R';
	consoleBuffer[87 + screen_x * 0].Char.AsciiChar = 'E';
	consoleBuffer[88 + screen_x * 0].Char.AsciiChar = ':';
	consoleBuffer[89 + screen_x * 0].Char.AsciiChar = th + 48;
	consoleBuffer[90 + screen_x * 0].Char.AsciiChar = h + 48;
	consoleBuffer[91 + screen_x * 0].Char.AsciiChar = t + 48;
	consoleBuffer[92 + screen_x * 0].Char.AsciiChar = u + 48;
	//lifepoint 
	consoleBuffer[71 + screen_x * 0].Char.AsciiChar = 'H';
	consoleBuffer[72 + screen_x * 0].Char.AsciiChar = 'P';
	consoleBuffer[73 + screen_x * 0].Char.AsciiChar = ':';
	consoleBuffer[74 + screen_x * 0].Char.AsciiChar = lifepoint+48;
	for (int i = 75; i < lifepoint+75; i++) {
		consoleBuffer[i + screen_x * 0].Attributes = 168;// light green
	}
	// Mother Ship Lifepoint
	consoleBuffer[5 + screen_x * 29].Char.AsciiChar = 'L';
	consoleBuffer[6 + screen_x * 29].Char.AsciiChar = 'P';
	consoleBuffer[7 + screen_x * 29].Char.AsciiChar = ':';
	consoleBuffer[8 + screen_x * 29].Char.AsciiChar = (MSlifepoint/10) + 48;
	consoleBuffer[9 + screen_x * 29].Char.AsciiChar = (MSlifepoint%10) + 48;
	for (int j = 10; j < MSlifepoint + 9; j++) {
		consoleBuffer[j + screen_x * 29].Attributes = 168;// light green
	}
	//wave
	consoleBuffer[5 + screen_x * 0].Char.AsciiChar = 'W';
	consoleBuffer[6 + screen_x * 0].Char.AsciiChar = 'A';
	consoleBuffer[7 + screen_x * 0].Char.AsciiChar = 'V';
	consoleBuffer[8 + screen_x * 0].Char.AsciiChar = 'E';
	consoleBuffer[9 + screen_x * 0].Char.AsciiChar = ':';
	consoleBuffer[10 + screen_x * 0].Char.AsciiChar = wave+48;
	//bullet count
	consoleBuffer[81 + screen_x * 29].Char.AsciiChar = 'M';
	consoleBuffer[82 + screen_x * 29].Char.AsciiChar = 'A';
	consoleBuffer[83 + screen_x * 29].Char.AsciiChar = 'X';
	consoleBuffer[84 + screen_x * 29].Char.AsciiChar = ' ';
	consoleBuffer[85 + screen_x * 29].Char.AsciiChar = 'B';
	consoleBuffer[86 + screen_x * 29].Char.AsciiChar = 'U';
	consoleBuffer[87 + screen_x * 29].Char.AsciiChar = 'L';
	consoleBuffer[88 + screen_x * 29].Char.AsciiChar = 'L';
	consoleBuffer[89 + screen_x * 29].Char.AsciiChar = 'E';
	consoleBuffer[90 + screen_x * 29].Char.AsciiChar = 'T';
	consoleBuffer[91 + screen_x * 29].Char.AsciiChar = ':';
	consoleBuffer[92 + screen_x * 29].Char.AsciiChar = (bullet_amount+extraB_plus) + 48;
}
void clear_bullet(int x, int y) {
	consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
	consoleBuffer[x + screen_x * y].Attributes = 0;
}
void fill_bullet_to_buffer(int x, int y) {
	consoleBuffer[x + screen_x * y].Char.AsciiChar = '^';
	consoleBuffer[x + screen_x * y].Attributes = 48;
}
void insertionSort() {
	int i, j, temp, levelTemp;
	char nameTemp[20];
	for (i = 0; i < 5; i++) {
		temp = topScore[i];
		levelTemp = topLevel[i];
		strcpy(nameTemp, topName[i]);
		j = i - 1;

		while (j >= 0 && temp >= topScore[j])
		{
			topScore[j + 1] = topScore[j];
			topLevel[j + 1] = topLevel[j];
			strcpy(topName[j + 1], topName[j]);
			j = j - 1;
		}
		topScore[j + 1] = temp;
		topLevel[j + 1] = levelTemp;
		strcpy(topName[j + 1], nameTemp);
	}
}
void leaderBoard_write(char name[20], int lv, int sc) {
	insertionSort();
	if (sc > topScore[4])
	{
		strcpy(topName[4], name);
		topLevel[4] = lv;
		topScore[4] = sc;
		insertionSort();
	}
}
void winner() {
	if (wave >= 10) {
		leaderBoard_write(pName, wave, score);
		for (int k = 0; k < 5; k++) {
			scoreWrite(topName[k], topLevel[k], topScore[k], k);
		}
		pause = true;
		gameWinner_page(pName, wave, score);
	}
}
void collectLeaderboard() {
	FILE* fptr2; int noffset2;
	struct playerSave
	{
		char name[20];
		int level;
		int score;
	}ps[5];
	fptr2 = fopen("ScoreRecord.txt", "r");
	for (int i = 0; i < 5; i++) {
		noffset2 = i * sizeof(struct playerSave);
		if (fseek(fptr2, noffset2, 0) == 0) {
			if (fread(&ps[i], sizeof(struct playerSave), 1, fptr2) != 0) {
				strcpy(topName[i], ps[i].name);
				topLevel[i] = ps[i].level;
				topScore[i] = ps[i].score;
			}
		}
	}
	fclose(fptr2);
}
void hitChecker() {
	for (int i = 0; i < wave_star[wave]; i++) {
		//bullet check
		for (int j = 0; j < bullet_amount + extraB_plus; j++) {
			if (bulletState[j] == 1 && star_id[i] == 1) {
				if (((star[i].X == bulletPos[j].X) || (star[i].X+1 == bulletPos[j].X) || (star[i].X - 1 == bulletPos[j].X))&& (star[i].Y == bulletPos[j].Y || star[i].Y == bulletPos[j].Y + 1)) {
					if (star[i].X == bulletPos[j].X) {
						score += 2;
					}
					else { score++; }
					star_state[i]=0;
					star[i] = { 50,0 };
					star_des++;
					bulletState[j] = 0;
					Beep(1200, 25);
				}
			}
			if (bulletState[j] == 1 && star_id[i] == 2 ) {
				if (((star[i].X == bulletPos[j].X) || (star[i].X + 1 == bulletPos[j].X) || (star[i].X - 1 == bulletPos[j].X) || (star[i].X + 2 == bulletPos[j].X) || (star[i].X - 2 == bulletPos[j].X)) && (star[i].Y == bulletPos[j].Y)) {
					if (star[i].X == bulletPos[j].X) {
						fighter_hp[i]--;
						bulletState[j] = 0;
						Beep(1200, 25);
						if (fighter_hp[i] == 0) score += 4;
					}
					else {
						fighter_hp[i]--;
						bulletState[j] = 0;
						Beep(1200, 25);
						if (fighter_hp[i] == 0) score += 2;
					}
					if (fighter_hp[i] == 0) {
					star_state[i] = 0;
					star[i] = { 50,0 };
					star_des++;
					}
				}
			}
			if (bulletState[j] == 1 && star_id[i] == 3) {
				if (((star[i].X == bulletPos[j].X) || (star[i].X + 1 == bulletPos[j].X) || (star[i].X - 1 == bulletPos[j].X) || (star[i].X + 2 == bulletPos[j].X) || (star[i].X - 2 == bulletPos[j].X)) && (star[i].Y == bulletPos[j].Y)) {
					if (star[i].X == bulletPos[j].X) {
						destoryer_hp[i]--;
						bulletState[j] = 0;
						Beep(1200, 25);
						if (destoryer_hp[i] == 0) score += 10;
					}
					else {
						destoryer_hp[i]--;
						bulletState[j] = 0;
						Beep(1200, 25);
						if (destoryer_hp[i] == 0) score += 5;
					}
					if (destoryer_hp[i] == 0) {
						star_state[i] = 0;
						star[i] = { 50,0 };
						star_des++;
					}
				}
			}
		}
		//ship check
		if ((star[i].X == mainShip.X && star[i].Y == mainShip.Y-2)||(star[i].X == mainShip.X+1 && star[i].Y == mainShip.Y - 1) || (star[i].X == mainShip.X - 1 && star[i].Y == mainShip.Y - 1)
			|| (star[i].X == mainShip.X + 2 && star[i].Y == mainShip.Y) || (star[i].X == mainShip.X - 2 && star[i].Y == mainShip.Y) || (star[i].X == mainShip.X && star[i].Y == mainShip.Y - 1)
			|| (star[i].X == mainShip.X && star[i].Y == mainShip.Y )){
			star[i] = { 50,0 };
			star_state[i] = 0;
			star_des++;
			Beep(800, 25);
			lifepoint--;
		}
		//lifepoint check
		if(lifepoint <= 0){
			leaderBoard_write(pName,wave,score);
			for (int k = 0; k < 5; k++) {
				scoreWrite(topName[k], topLevel[k], topScore[k], k);
			}
			gameEnd_page(pName, wave, score);
			
		}
		//MSlifepoint checker
		if (MSlifepoint <= 0) {
			leaderBoard_write(pName, wave, score);
			for (int k = 0; k < 5; k++) {
				scoreWrite(topName[k], topLevel[k], topScore[k], k);
			}
			gameEnd_page(pName, wave, score);
			
		}
		//shield check
		if ((shield.X == mainShip.X && shield.Y == mainShip.Y - 2) || (shield.X == mainShip.X + 1 && shield.Y == mainShip.Y - 1) || (shield.X == mainShip.X - 1 && shield.Y == mainShip.Y - 1)
			|| (shield.X == mainShip.X + 2 && shield.Y == mainShip.Y) || (shield.X == mainShip.X - 2 && shield.Y == mainShip.Y) || (shield.X == mainShip.X && shield.Y == mainShip.Y-1)) {
			shield_item = 0;
			shield = { 50,0 };
			Beep(800, 25);
			shield_state = 1;
		}
		//shield hit check
		if (shield_state == 1) {
			if ((star[i].X == mainShip.X && star[i].Y == mainShip.Y - 4) || (star[i].X == mainShip.X + 1 && star[i].Y == mainShip.Y - 4) || (star[i].X - 1 == mainShip.X && star[i].Y == mainShip.Y - 4) ||
				(star[i].X == mainShip.X + 2 && star[i].Y == mainShip.Y - 3) || (star[i].X == mainShip.X - 2 && star[i].Y == mainShip.Y - 3)
				|| (star[i].X == mainShip.X + 3 && star[i].Y == mainShip.Y - 2) || (star[i].X == mainShip.X - 3 && star[i].Y == mainShip.Y - 2)
				|| (star[i].X == mainShip.X + 4 && star[i].Y == mainShip.Y - 1) || (star[i].X == mainShip.X - 4 && star[i].Y == mainShip.Y - 1)
				|| (star[i].X == mainShip.X + 5 && star[i].Y == mainShip.Y) || (star[i].X == mainShip.X - 5 && star[i].Y == mainShip.Y)) {
				shield_item = 0;
				shield_state = 0;
				star_state[i] = 0;
				star[i] = { 50,0 };
				star_des++;
				score++; 
				Beep(1200, 25);
			}
		}
		//heal check
		if ((heal.X == mainShip.X && heal.Y == mainShip.Y - 2) || (heal.X == mainShip.X + 1 && heal.Y == mainShip.Y - 1) || (heal.X == mainShip.X - 1 && heal.Y == mainShip.Y - 1)
			|| (heal.X == mainShip.X + 2 && heal.Y == mainShip.Y) || (heal.X == mainShip.X - 2 && heal.Y == mainShip.Y) || (heal.X == mainShip.X && heal.Y == mainShip.Y - 1)) {
			heal_state = 0;
			heal = { 50,0 };
			Beep(800, 25);
			if(lifepoint<5) lifepoint += 1;
			if(MSlifepoint<30) MSlifepoint += 1;
		}
		//bomb check
		if ((bomb.X == mainShip.X && bomb.Y == mainShip.Y - 2) || (bomb.X == mainShip.X + 1 && bomb.Y == mainShip.Y - 1) || (bomb.X == mainShip.X - 1 && bomb.Y == mainShip.Y - 1)
			|| (bomb.X == mainShip.X + 2 && bomb.Y == mainShip.Y) || (bomb.X == mainShip.X - 2 && bomb.Y == mainShip.Y) || (bomb.X == mainShip.X && bomb.Y == mainShip.Y - 1)) {
			bomb_state = 0;
			bomb = { 50,0 };
			Beep(450, 25);
			lifepoint--;
		}
		//extraB check
		if ((extraB.X == mainShip.X && extraB.Y == mainShip.Y - 2) || (extraB.X == mainShip.X + 1 && extraB.Y == mainShip.Y - 1) || (extraB.X == mainShip.X - 1 && extraB.Y == mainShip.Y - 1)
			|| (extraB.X == mainShip.X + 2 && extraB.Y == mainShip.Y) || (extraB.X == mainShip.X - 2 && extraB.Y == mainShip.Y) || (extraB.X == mainShip.X && extraB.Y == mainShip.Y - 1)) {
			extraBullet_state = 0;
			extraB = { 50,0 };
			Beep(900, 25);
			if (extraB_plus < 3) extraB_plus += 1;
		}
	}
}
void shooting(int stat) {
	if (bulletState[bcount] != 1 && stat == 1) {
		bulletState[bcount] = 1;
		bulletPos[bcount].X = mainShip.X;
		bulletPos[bcount].Y = mainShip.Y - 3;
		bcount++;
		clickStat = 0;
		Beep(700, 25);
	}
	else { clickStat = 0; }
	if (bcount > ((bullet_amount + extraB_plus) - 1)) { bcount = 0; }
}
void bullet_on() {
	for (int i = 0; i < bullet_amount + extraB_plus; i++) {
		if (bulletState[i] == 1) {
			clear_bullet(bulletPos[i].X, bulletPos[i].Y);
			if (bulletPos[i].Y == 1) { bulletState[i] = 0; }
			else {
				fill_bullet_to_buffer(bulletPos[i].X, bulletPos[i].Y--);
			}
		}
	}
}
void clear_buffer()
{
	for (int y = 0; y < screen_y; ++y) {
		for (int x = 0; x < screen_x; ++x) {
			consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
			consoleBuffer[x + screen_x * y].Attributes = 7;
		}
	}
}
void clear_screen() {
	for (int y = 0; y < screen_y; ++y) {
		for (int x = 0; x < screen_x; ++x) {
			setcolor(7, 0);
			printf(" ");
		}
	}
}
void init_star() {
	if (wave <= 2) {
		int i, j;
		for (i = 0; i < wave_star[wave]; i++) {
			star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 }; //(max-min+1)+min
			star_state[i] = 1;
			star_id[i] = 1;
			if (star_id[i] == 1) normalEnemy_count++;
			else if (star_id[i] == 2) fighterEnemy_count++;
			else if (star_id[i] == 3) destoryerEnemy_count++;
			for (j = 0; j < wave_star[wave]; j++) {
				if (star[i].X + 1 == star[j].X || star[i].X + 2 == star[j].X || star[i].X - 1 == star[j].X || star[i].X - 2 == star[j].X) {
					star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 };
				}
			}
		}
	}
	if (wave == 3) {
		int i, j;
		for (i = 0; i < wave_star[wave]; i++) {
			star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 }; //(max-min+1)+min
			star_state[i] = 1;
			if (normalEnemy_count < 5) star_id[i] = 1;
			else { star_id[i] = 2; fighter_hp[i] = 2; }
			if (star_id[i] == 1) normalEnemy_count++;
			else if (star_id[i] == 2) fighterEnemy_count++;
			else if (star_id[i] == 3) destoryerEnemy_count++;
			for (j = 0; j < wave_star[wave]; j++) {
				if (star[i].X + 1 == star[j].X || star[i].X + 2 == star[j].X || star[i].X - 1 == star[j].X || star[i].X - 2 == star[j].X) {
					star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 };
				}
			}
		}
	}
	if (wave == 4) {
		int i, j;
		for (i = 0; i < wave_star[wave]; i++) {
			star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 }; //(max-min+1)+min
			star_state[i] = 1;
			if (star_id[i] == 1) normalEnemy_count++;
			else if (star_id[i] == 2) fighterEnemy_count++;
			else if (star_id[i] == 3) destoryerEnemy_count++;
			star_id[i] = 2;
			fighter_hp[i] = 2;
			for (j = 0; j < wave_star[wave]; j++) {
				if (star[i].X + 1 == star[j].X || star[i].X + 2 == star[j].X || star[i].X - 1 == star[j].X || star[i].X - 2 == star[j].X) {
					star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 };
				}
			}
		}
	}
	if (wave == 5) {
		int i, j;
		for (i = 0; i < wave_star[wave]; i++) {
			star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 }; //(max-min+1)+min
			star_state[i] = 1;
			if (normalEnemy_count < 5) star_id[i] = 1;
			else if (normalEnemy_count >= 5 && fighterEnemy_count < 3) { star_id[i] = 2; fighter_hp[i] = 2; }
			else if (normalEnemy_count >= 5 && fighterEnemy_count >= 3 && destoryerEnemy_count < 2) { star_id[i] = 3; destoryer_hp[i] = 5; }
			if (star_id[i] == 1) normalEnemy_count++;
			else if (star_id[i] == 2) fighterEnemy_count++;
			else if (star_id[i] == 3) destoryerEnemy_count++;
			for (j = 0; j < wave_star[wave]; j++) {
				if (star[i].X + 1 == star[j].X || star[i].X + 2 == star[j].X || star[i].X - 1 == star[j].X || star[i].X - 2 == star[j].X) {
					star[i] = { SHORT((rand() % ((screen_x - 4) - 4 + 1)) + 4),SHORT((rand()%5)+3)};
				}
			}
		}
	}
	if (wave == 6) {
		int i, j;
		for (i = 0; i < wave_star[wave]; i++) {
			star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 }; //(max-min+1)+min
			star_state[i] = 1;
			if (normalEnemy_count < 4) star_id[i] = 1;
			else if (normalEnemy_count >= 4 && fighterEnemy_count < 2) { star_id[i] = 2; fighter_hp[i] = 2; }
			else if (normalEnemy_count >= 4 && fighterEnemy_count >= 2 && destoryerEnemy_count < 4) { star_id[i] = 3; destoryer_hp[i] = 5; }
			if (star_id[i] == 1) normalEnemy_count++;
			else if (star_id[i] == 2) fighterEnemy_count++;
			else if (star_id[i] == 3) destoryerEnemy_count++;
			for (j = 0; j < wave_star[wave]; j++) {
				if (star[i].X + 1 == star[j].X || star[i].X + 2 == star[j].X || star[i].X - 1 == star[j].X || star[i].X - 2 == star[j].X) {
					star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),SHORT((rand() % 5) + 3) };
				}
			}
		}
	}
	if (wave == 7) {
		int i, j;
		for (i = 0; i < wave_star[wave]; i++) {
			star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 }; //(max-min+1)+min
			star_state[i] = 1;
			if (normalEnemy_count < 2) star_id[i] = 1;
			else if (normalEnemy_count >= 2 && fighterEnemy_count < 3) { star_id[i] = 2; fighter_hp[i] = 2; }
			else if (normalEnemy_count >= 2 && fighterEnemy_count >= 3 && destoryerEnemy_count < 5) { star_id[i] = 3; destoryer_hp[i] = 5; }
			if (star_id[i] == 1) normalEnemy_count++;
			else if (star_id[i] == 2) fighterEnemy_count++;
			else if (star_id[i] == 3) destoryerEnemy_count++;
			for (j = 0; j < wave_star[wave]; j++) {
				if (star[i].X + 1 == star[j].X || star[i].X + 2 == star[j].X || star[i].X - 1 == star[j].X || star[i].X - 2 == star[j].X) {
					star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),SHORT((rand() % 5) + 3) };
				}
			}
		}
	}
	if (wave == 8) {
		int i, j;
		for (i = 0; i < wave_star[wave]; i++) {
			star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 }; //(max-min+1)+min
			star_state[i] = 1;
			if (normalEnemy_count < 5) star_id[i] = 1;
			else if (normalEnemy_count >= 5 && fighterEnemy_count < 5) { star_id[i] = 2; fighter_hp[i] = 2; }
			else if (normalEnemy_count >= 5 && fighterEnemy_count >= 5 && destoryerEnemy_count < 5) { star_id[i] = 3; destoryer_hp[i] = 5; }
			if (star_id[i] == 1) normalEnemy_count++;
			else if (star_id[i] == 2) fighterEnemy_count++;
			else if (star_id[i] == 3) destoryerEnemy_count++;
			for (j = 0; j < wave_star[wave]; j++) {
				if (star[i].X + 1 == star[j].X || star[i].X + 2 == star[j].X || star[i].X - 1 == star[j].X || star[i].X - 2 == star[j].X) {
					star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),SHORT((rand() % 6) + 3) };
				}
			}
		}
	}
	if (wave == 9) {
		int i, j;
		for (i = 0; i < wave_star[wave]; i++) {
			star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 }; //(max-min+1)+min
			star_state[i] = 1;
			if (fighterEnemy_count < 10) { star_id[i] = 2; fighter_hp[i] = 2; }
			else if (fighterEnemy_count >= 10 && destoryerEnemy_count < 5) { star_id[i] = 3; destoryer_hp[i] = 5; }
			if (star_id[i] == 1) normalEnemy_count++;
			else if (star_id[i] == 2) fighterEnemy_count++;
			else if (star_id[i] == 3) destoryerEnemy_count++;
			for (j = 0; j < wave_star[wave]; j++) {
				if (star[i].X + 1 == star[j].X || star[i].X + 2 == star[j].X || star[i].X - 1 == star[j].X || star[i].X - 2 == star[j].X) {
					star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),SHORT((rand() % 6) + 3) };
				}
			}
		}
	}
}
void star_fall()
{
	int pattern = (rand()% 3);
	if (pattern == 1) {
		int i;
		i = (rand() % wave_star[wave]);
		if (star_state[i] == 1) {
			if (star[i].Y >= 27) {
				star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 };
				if (star_id[i] == 1) MSlifepoint--;
				else if (star_id[i] == 2) MSlifepoint -= 2;
				else if (star_id[i] == 3) MSlifepoint -= 5;
			}
			else {
				star[i] = { star[i].X,SHORT(star[i].Y + 2) };
			}
		}
	}
	if (pattern == 2) {
		int i,j;
		i = (rand() % wave_star[wave]);
		j = (rand() % 3);
		if (star_state[i] == 1) {
			if (star[i].Y >= 27) {
				star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 };
				if (star_id[i] == 1) MSlifepoint--;
				else if (star_id[i] == 2) MSlifepoint -= 2;
				else if (star_id[i] == 3) MSlifepoint -= 5;
			}
			if (star[i].X <= 6) {
				star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 };
			}
			if (star[i].X >= (screen_x - 6)) {
				star[i] = { SHORT((rand() % ((screen_x - 6) - 6 + 1)) + 6),1 };
			}
			else {
				if(j==1)
				star[i] = {SHORT(star[i].X + (1 + enemy_speed)),SHORT(star[i].Y + 2) };
				else
				star[i] = { SHORT(star[i].X - (1 + enemy_speed)),SHORT(star[i].Y + 2) };
			}
		}
	}
}
void fill_star_to_buffer()
{
	for (int i = 0; i < wave_star[wave]; i++) {
		if (star_state[i] == 1) {
			if (star_id[i] == 1) {
				consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar = '1';
				consoleBuffer[star[i].X + 1 + screen_x * star[i].Y].Char.AsciiChar = '>';
				consoleBuffer[star[i].X - 1 + screen_x * star[i].Y].Char.AsciiChar = '<';
				consoleBuffer[star[i].X + screen_x * star[i].Y].Attributes = 90;
				consoleBuffer[star[i].X + 1 + screen_x * star[i].Y].Attributes = 2;
				consoleBuffer[star[i].X - 1 + screen_x * star[i].Y].Attributes = 2;
			}
			if (star_id[i] == 2) {
				consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar = fighter_hp[i]+48;
				consoleBuffer[star[i].X + 1 + screen_x * star[i].Y].Char.AsciiChar = '-';
				consoleBuffer[star[i].X - 1 + screen_x * star[i].Y].Char.AsciiChar = '-';
				consoleBuffer[star[i].X + 2 + screen_x * star[i].Y].Char.AsciiChar = '}';
				consoleBuffer[star[i].X - 2 + screen_x * star[i].Y].Char.AsciiChar = '{';
				consoleBuffer[star[i].X + screen_x * star[i].Y].Attributes = 95;
				consoleBuffer[star[i].X + 1 + screen_x * star[i].Y].Attributes = 4;
				consoleBuffer[star[i].X - 1 + screen_x * star[i].Y].Attributes = 4;
			}
			if (star_id[i] == 3) {
				consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar = destoryer_hp[i] + 48;
				consoleBuffer[star[i].X + 1 + screen_x * star[i].Y].Char.AsciiChar = '<';
				consoleBuffer[star[i].X - 1 + screen_x * star[i].Y].Char.AsciiChar = '>';
				consoleBuffer[star[i].X + 2 + screen_x * int(star[i].Y+1)].Char.AsciiChar = ':';
				consoleBuffer[star[i].X - 2 + screen_x * int(star[i].Y+1)].Char.AsciiChar = ':';
				consoleBuffer[star[i].X + screen_x * int(star[i].Y - 1)].Char.AsciiChar = 'M';
				consoleBuffer[star[i].X + 2 + screen_x * star[i].Y].Char.AsciiChar = '[';
				consoleBuffer[star[i].X - 2 + screen_x * star[i].Y].Char.AsciiChar = ']';
				consoleBuffer[star[i].X + screen_x * star[i].Y].Attributes = 71; //fg white bg red
				consoleBuffer[star[i].X + 1 + screen_x * star[i].Y].Attributes = 7;
				consoleBuffer[star[i].X - 1 + screen_x * star[i].Y].Attributes = 7;
				consoleBuffer[star[i].X + 2 + screen_x * star[i].Y].Attributes = 111;
				consoleBuffer[star[i].X - 2 + screen_x * star[i].Y].Attributes = 111;
				consoleBuffer[star[i].X + screen_x * int(star[i].Y - 1)].Attributes = 6;
			}
		}
	}
}
void fill_buffer_to_console()
{
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos, &windowSize);
}

int main()
{
	setcursor(0);
	setConsole(screen_x, screen_y);
	fontChange();
	setMode();
	wave_state = 1;
	collectLeaderboard();
	srand(time(NULL));
	while (play) {
		DWORD numEvents = 0;
		DWORD numEventsRead = 0;
		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead; ++i) {
				if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true) {
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
						play = false;
					}
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'P' || eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'p') {
						if (p_count == 0) {
							pause = true;
							p_count++;
							cursorPos(45, 28);
							setcolor(2, 0);
							printf("PAUSE");
						}
						else if (p_count > 0) {
							pause = false;
							p_count = 0;
						}
					}
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'A' || eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'a') {
						direct = 'l';
					}
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'S' || eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 's') {
						direct = 'n';
					}
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'D' || eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'd') {
						direct = 'r';
					}
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == ' ') {
						clickStat = 1;
						shooting(clickStat);
					}
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'G' || eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'g') {
						gCount++;
					}
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'B' || eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'b') {
						bCount++;
					}
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'C' || eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'c') {
						cCount++;
					}
				}
			}
			delete[] eventBuffer;
		}
		if (gCount == 0 && !start && bCount == 0 && cCount == 0) {
			start_page();
			clr_state = true;
		}
		if (bCount == 1 && !start && gCount == 0) {
			if (clr_state) {
				clear_screen();
				clr_state = false;
			}
			highScore();
		}
		if (cCount == 1 && !start && gCount == 0) {
			if (clr_state) {
				clear_screen();
				clr_state = false;
			}
			credit_page();
		}
		if (!start) {
			if (bCount == 2) { clear_screen(); bCount = 0; }
			if (cCount == 2) { clear_screen(); cCount = 0; }
		}
		if (gCount==1 && !start) {
			clear_screen();
			setcursor(1);
			setcolor(7, 0);
			playerName();
			scanf("%s", pName);
			cursorPos(39, 17);
			printf("This is story of %s", pName);
			Sleep(2000);
			start = true;
			setcursor(0);
		}
		if (!pause &&  start) {
			if (wave_state == 1) {
				init_star();
				wave_state = 0;
			}
			winner();
			init_item();
			itemFall();
			star_fall();
			clear_buffer();
			wave_change(star_des);
			scoreBoard(score);
			shipMovement(direct);
			hitChecker();
			bullet_on();
			draw_ship_to_buffer(mainShip);
			for(int itemR = 0;itemR<max_item;itemR++)			
				fill_item_to_buffer(itemR);
			fill_star_to_buffer();
			fill_buffer_to_console();
			Sleep(100);
		}
	}
	return 0;
}