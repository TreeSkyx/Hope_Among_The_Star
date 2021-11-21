#ifndef STARTMENU_H
#define STARTMENU_H
void start_page();
void credit_page();
void playerName();
void highScore();
void gameEnd_page(char[],int,int);
void gameWinner_page(char[],int,int);
void starShip_draw(int, int);
void deleteShip(int, int);
void enemyShip_draw(int, int);
void deleteEnemy(int, int);
void laser_draw(int, int);
void deletelaser(int, int);
void init_starS();
#endif // !STARTMENU_H
