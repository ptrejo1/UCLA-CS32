#ifndef globals_h
#define globals_h

#define MAXROWS  20               // max number of rows in a city
#define MAXCOLS  30               // max number of columns in a city
#define MAXFLATULANS  100         // max number of Flatulans allowed
#define INITIAL_PLAYER_HEALTH  10

#define UP       0
#define DOWN     1
#define LEFT     2
#define RIGHT    3
#define NUMDIRS  4

int randInt(int min, int max);
int decodeDirection(char dir);
void clearScreen();

#endif
