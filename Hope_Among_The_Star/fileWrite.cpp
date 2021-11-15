#define _CRT_SECURE_NO_WARNINGS 1
#include "fileWrite.h"
#include "cursorSetting.h"
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
		p.score = point;
		fptr = fopen("ScoreRecord.txt", "w");
		fwrite(&p, sizeof(struct player), 1, fptr);
	fclose(fptr);
}
void scoreRead() {
	FILE* fptr; int noffset;
	struct player
	{
		char name[20];
		int level;
		int score;
	}p;
	fptr = fopen("ScoreRecord.txt", "r");
	for (int i = 0; i < 10; i++) {
		noffset = i * sizeof(struct player);
		if (fseek(fptr, noffset, 0) == 0) {
			if (fread(&p, sizeof(struct player), 1, fptr) != 0) {
				cursorPos(30, 10+i);
				printf("Name : %s\t", p.name);
				printf("Level : %d\t", p.level);
				printf("Score : %d\n", p.score);
			}
		}
	}
	fclose(fptr);

}
