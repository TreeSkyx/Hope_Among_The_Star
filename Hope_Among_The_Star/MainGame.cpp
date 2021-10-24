#include <stdio.h>
#include <windows.h>
#include <time.h>
//#include "Scoreboard.h"
#define max_star 30
#define screen_x 100
#define screen_y 30
#define bullet_amount 6
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
int p_count = 0;
int score = 0;
int lifepoint = 5;
//ship var.
COORD mainShip = { 32,26 };
char direct = 'n';

//bullet var.
int clickStat = 0;
int bulletState[bullet_amount];
int bcount = 0;
COORD bulletPos[bullet_amount];

//wave & star var.
int wave_star = 15;
int star_des = 0;
int wave = 0;
COORD star[max_star];

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
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	SetConsoleMode(rHnd, fdwMode);
	return 0;
}
void cursorPos(int x, int y) {
	COORD c = { x, y };
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void setcolor(int fg, int bg) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
void draw_ship_to_buffer(COORD ship) {
	consoleBuffer[ship.X + screen_x * ship.Y].Char.AsciiChar = ' ';//0
	consoleBuffer[ship.X - 1 + screen_x * ship.Y].Char.AsciiChar = '_';// /
	consoleBuffer[ship.X + 1 + screen_x * ship.Y].Char.AsciiChar = '_';
	consoleBuffer[ship.X - 2 + screen_x * ship.Y].Char.AsciiChar = ' '; // }
	consoleBuffer[ship.X + 2 + screen_x * ship.Y].Char.AsciiChar = ' ';
	consoleBuffer[ship.X + screen_x * int(ship.Y - 1)].Char.AsciiChar = ' '; // A
	consoleBuffer[ship.X + 1 + screen_x * int(ship.Y - 1)].Char.AsciiChar = ' ';// >
	consoleBuffer[ship.X - 1 + screen_x * int(ship.Y - 1)].Char.AsciiChar = ' ';
	consoleBuffer[ship.X + screen_x * int(ship.Y - 2)].Char.AsciiChar = '^'; // ^
	consoleBuffer[ship.X - 1 + screen_x * int(ship.Y + 1)].Char.AsciiChar = 'W'; // W
	consoleBuffer[ship.X + 1 + screen_x * int(ship.Y + 1)].Char.AsciiChar = 'W'; // W
	consoleBuffer[ship.X + screen_x * ship.Y].Attributes = 23;
	consoleBuffer[ship.X + 1 + screen_x * ship.Y].Attributes = 120;//white
	consoleBuffer[ship.X - 1 + screen_x * ship.Y].Attributes = 120;
	consoleBuffer[ship.X + 2 + screen_x * ship.Y].Attributes = 78;//red
	consoleBuffer[ship.X - 2 + screen_x * ship.Y].Attributes = 78;
	consoleBuffer[ship.X + 1 + screen_x * int(ship.Y - 1)].Attributes = 120;
	consoleBuffer[ship.X - 1 + screen_x * int(ship.Y - 1)].Attributes = 120;
	consoleBuffer[ship.X + screen_x * int(ship.Y - 1)].Attributes = 48;//light blue
	consoleBuffer[ship.X + screen_x * int(ship.Y - 2)].Attributes = 78;
	consoleBuffer[ship.X + 1 + screen_x * int(ship.Y + 1)].Attributes = 6;// 108 yellow 
	consoleBuffer[ship.X - 1 + screen_x * int(ship.Y + 1)].Attributes = 6;
}
void shipMovement(char dir) {
	if (dir == 'l') {
		if (mainShip.X != 3) {
			mainShip.X--;
		}
		else { dir == 'n'; mainShip.X = 3; }
	}
	if (dir == 'r') {
		if (mainShip.X != screen_x - 4) {
			mainShip.X++;
		}
		else { dir == 'n'; mainShip.X = screen_x - 4; }
	}
	if (dir == 'n') {
		mainShip.X = mainShip.X;
		mainShip.Y = mainShip.Y;
	}
}
void scoreBoard(int s) {
	int u = 0, t = 0, h = 0, th = 0;
	th = s / 1000;
	h = (s % 1000) / 100;
	t = (s % 100) / 10;
	u = s % 10;
	consoleBuffer[80 + screen_x * 0].Char.AsciiChar = 'S';
	consoleBuffer[81 + screen_x * 0].Char.AsciiChar = 'C';
	consoleBuffer[82 + screen_x * 0].Char.AsciiChar = 'O';
	consoleBuffer[83 + screen_x * 0].Char.AsciiChar = 'R';
	consoleBuffer[84 + screen_x * 0].Char.AsciiChar = 'E';
	consoleBuffer[85 + screen_x * 0].Char.AsciiChar = ':';
	consoleBuffer[86 + screen_x * 0].Char.AsciiChar = th + 48;
	consoleBuffer[87 + screen_x * 0].Char.AsciiChar = h + 48;
	consoleBuffer[88 + screen_x * 0].Char.AsciiChar = t + 48;
	consoleBuffer[89 + screen_x * 0].Char.AsciiChar = u + 48;
	//lifepoint 
	consoleBuffer[70 + screen_x * 0].Char.AsciiChar = 'L';
	consoleBuffer[71 + screen_x * 0].Char.AsciiChar = 'P';
	consoleBuffer[72 + screen_x * 0].Char.AsciiChar = ':';
	for (int i = 73; i < lifepoint+73; i++) {
		consoleBuffer[i + screen_x * 0].Attributes = 78;
	}

}
void clear_bullet(int x, int y) {
	consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
	consoleBuffer[x + screen_x * y].Attributes = 0;
}
void fill_bullet_to_buffer(int x, int y) {
	consoleBuffer[x + screen_x * y].Char.AsciiChar = '^';
	consoleBuffer[x + screen_x * y].Attributes = 48;
}
void hitChecker() {
	for (int i = 0; i < wave_star; i++) {
		//bullet check
		for (int j = 0; j < bullet_amount; j++) {
			if (bulletState[j] == 1) {
				if (star[i].X == bulletPos[j].X && (star[i].Y == bulletPos[j].Y || star[i].Y == bulletPos[j].Y + 1)) {
					star[i] = { SHORT(rand() % screen_x),1 };
					bulletState[j] = 0;
					Beep(1200, 25);
					//lifepoint--;
					score++;
				}
			}
		}
		if(lifepoint == 0){
			play = false;
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
	if (bcount > (bullet_amount - 1)) { bcount = 0; }
}
void bullet_on() {
	for (int i = 0; i < bullet_amount; i++) {
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
void init_star() {
	int i;
	for (i = 0; i < wave_star; i++) {
		star[i] = { SHORT(rand() % screen_x),1 };
	}
}
void star_fall()
{
	int i;
	i = (rand() % wave_star);
	//for (i = 0; i < wave_star; i++) {
		if (star[i].Y >= 21) {
			star[i] = { SHORT(rand() % screen_x),1 };
		}
		else {
			star[i] = { star[i].X,SHORT(star[i].Y + 1) };
		}
	//}
}
void fill_star_to_buffer()
{
	for (int i = 0; i < wave_star; i++) {
		consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar = '*';
		consoleBuffer[star[i].X + screen_x * star[i].Y].Attributes = 7;
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
	setMode();
	srand(time(NULL));
	init_star();
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
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == ' ' /*|| eventBuffer[i].Event.KeyEvent.uChar.AsciiChar=='v'*/) {
						clickStat = 1;
						shooting(clickStat);
					}
					/*else if (eventBuffer[i].EventType == MOUSE_EVENT){
						if (eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
							clickStat = 1;
							shooting(clickStat);
							}
					}*/
				}
			}
			delete[] eventBuffer;
		}
		if (!pause) {
			star_fall();
			clear_buffer();
			scoreBoard(score);
			shipMovement(direct);
			hitChecker();
			bullet_on();
			draw_ship_to_buffer(mainShip);
			fill_star_to_buffer();
			fill_buffer_to_console();
			Sleep(100);
		}
	}
	return 0;
}