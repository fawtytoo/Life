//  life

#include "common.h"
#include "system.h"

int     gameRunning = 1;

EVENT   Ticker = WorldTicker;

int main()
{
    int         key;

    SystemInit();
    WorldCreate();

    while (gameRunning)
    {
        while (SystemEvent(&key))
        {
            if (key == KEY_QUIT)
                gameRunning = 0;
            else if (key != KEY_NONE)
                WorldResponder(key);
        }

        Ticker(SystemTicks());
        WorldDrawer();
        SystemUpdate();
    }

    SystemQuit();

    return 0;
}

//  life
