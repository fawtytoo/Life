//  life

#include "common.h"
#include "system.h"

int     gameRunning = 1;

EVENT   Ticker = WorldTicker;

int     gameKey;

int main()
{
    SystemInit();
    WorldCreate();

    while (gameRunning)
    {
        while (SystemEvent(&gameKey))
        {
            if (gameKey == KEY_QUIT)
                gameRunning = 0;
            else if (gameKey != KEY_NONE)
                WorldResponder();
        }

        Ticker();
        WorldDrawer();
        SystemUpdate();
    }

    SystemQuit();

    return 0;
}

//  life
