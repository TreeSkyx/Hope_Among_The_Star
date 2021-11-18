#define _CRT_SECURE_NO_WARNINGS 1
#include "fileWrite.h"
#include "cursorSetting.h"
#include <string.h>
#include <stdio.h>
void scoreWrite(char n[20], int lv, int point, int k) {
	FILE* fptr; int noffset;
	struct player
	{
		char name[20];
		int level;
		int score;
	}p[5];
		strcpy(p[k].name, n);
		p[k].level = lv;
		p[k].score = point;
		if (k == 0)
			fptr = fopen("ScoreRecord.txt", "w");
		else
			fptr = fopen("ScoreRecord.txt", "a");
		fwrite(&p[k], sizeof(struct player), 1, fptr);
		fclose(fptr);
}
void scoreRead() {
	FILE* fptr; int noffset;
	struct player
	{
		char name[20];
		int level;
		int score;
	}p[5];
	fptr = fopen("ScoreRecord.txt", "r");
	for (int i = 0; i < 5; i++) {
		noffset = i * sizeof(struct player);
		if (fseek(fptr, noffset, 0) == 0) {
			if (fread(&p[i], sizeof(struct player), 1, fptr) != 0) {
				cursorPos(30, 10+i);
				printf("Name : %s\t", p[i].name);
				printf("Level : %d\t", p[i].level);
				printf("Score : %d\n", p[i].score);
			}
		}
	}
	fclose(fptr);

}
