//  life

#include "common.h"
#include "system.h"

int     gameRunning = 1;

EVENT   Ticker = WorldTicker;
EVENT   Responder = WorldCreateResponder;

int     gameKey;

int main()
{
    SystemInit();
    WorldCreate();

    while (gameRunning)
    {
        while (SystemEvent(&gameKey))
        {
            Responder();
        }

        Ticker();
        WorldDrawer();
        SystemUpdate();
    }

    SystemQuit();

    return 0;
}

//  life
