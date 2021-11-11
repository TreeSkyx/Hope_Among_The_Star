#define _CRT_SECURE_NO_WARNINGS 1
#include "fileWrite.h"
#include <string.h>
#include <stdio.h>
void scoreWrite(char n[20], int lv, int point) {
	FILE* fptr; int noffset;
	struct player
	{
		char name[20];
		int level;
		int score;
	}p;
	strcpy(p.name, n);
	p.level = lv;
	p.score= point;
	fptr = fopen("ScoreRecord.txt", "w");
	fwrite(&p, sizeof(struct player), 1, fptr);
	fclose(fptr);
}

