#include <stdio.h>
int main(){
FILE *fptr; int i; int noffset;
struct player{
	char name[20];
	int level;
	int score;
}p;
fptr = fopen("scoreRecord.txt","r");
noffset = 0 * sizeof(struct player);
if(fseek(fptr,noffset,0)==0){	
	if(fread(&p,sizeof(struct player),1,fptr)!=0){
		printf("Name : %s\t",p.name);
		printf("Level : %d\t",p.level);
		printf("Score : %d\n",p.score);
	}
}
fclose(fptr);
return 0;
}
