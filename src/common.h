//  life

#ifndef __COMMON__
#define __COMMON__

#define WIDTH       80
#define HEIGHT      60

#define CELLSIZE    8

enum
{
    GA_NOTHING,
    GA_GENERATE,
    GA_CREATE
};

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

extern int          game_Action;

void WorldCellPosChange(int, int);
void WorldGenerate(int);
void WorldResponder(int);
void WorldTicker(int);
void WorldDrawer(void);
void WorldCreate(void);

#endif

//  life
