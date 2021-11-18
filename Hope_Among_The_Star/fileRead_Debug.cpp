#include <stdio.h>
int main(){
FILE *fptr; int i; int noffset;
struct player{
	char name[20];
	int level;
	int score;
}p[5];
fptr = fopen("ScoreRecord.txt","r");
for(i=0;i<5;i++)
{
noffset = i * sizeof(struct player);
if(fseek(fptr,noffset,0)==0){	
	if(fread(&p[i],sizeof(struct player),1,fptr)!=0){
		printf("Name : %s\t",p[i].name);
		printf("Level : %d\t",p[i].level);
		printf("Score : %d\n",p[i].score);
	}
}
}
fclose(fptr);
return 0;
}
