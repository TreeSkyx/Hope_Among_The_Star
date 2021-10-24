#define _CRT_SECURE_NO_WARNINGS 1
#include"gameFont.h"
#include<stdio.h>
#include<windows.h>
#include<string.h>
void fontChange()
{
	int newWidth = 9, newHeight = 16;
	CONSOLE_FONT_INFOEX fontStructure = { 0 };
	fontStructure.cbSize = sizeof(fontStructure);
	fontStructure.dwFontSize.X = newWidth;
	fontStructure.dwFontSize.Y = newHeight;
	wcscpy(fontStructure.FaceName, L"Press Start");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentConsoleFontEx(hConsole, true, &fontStructure);
}
