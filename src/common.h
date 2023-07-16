//  life

#ifndef __COMMON__
#define __COMMON__

#define WIDTH       80
#define HEIGHT      60

#define CELLSIZE    8

enum
{
    KEY_NONE,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_SPACE,
    KEY_START,
    KEY_STOP,
    KEY_CLEAR,
    KEY_QUIT,
    KEY_ANY
};

typedef void (*EVENT)(int);

extern EVENT    Ticker;

extern int      gameKey;

void WorldCellPosChange(int, int);
void WorldGenerate(int);
void WorldResponder(void);
void WorldTicker(int);
void WorldDrawer(void);
void WorldCreate(void);

#endif

//  life
