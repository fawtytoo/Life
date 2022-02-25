//  life

#include "common.h"
#include "system.h"

enum
{
    GS_CREATE,
    GS_GENERATE,
    GS_QUIT
};

int         game_State = GS_CREATE;
int         game_Action = GA_NOTHING;

void ticker(int ticks)
{
    switch (game_Action)
    {
      case GA_GENERATE:
        game_State = GS_GENERATE;
        break;

      case GA_CREATE:
        game_State = GS_CREATE;
        break;
    }

    game_Action = GA_NOTHING;

    switch (game_State)
    {
      case GS_CREATE:
        WorldTicker(ticks);
        break;

      case GS_GENERATE:
        WorldGenerate(ticks);
        break;
    }
}

int main()
{
    int         key;

    SystemInit();
    WorldCreate();

    while (game_State != GS_QUIT)
    {
        while (SystemEvent(&key))
        {
            if (key == KEY_QUIT)
                game_State = GS_QUIT;
            else if (key != KEY_NONE)
                WorldResponder(key);
        }

        ticker(SystemTicks());
        WorldDrawer();
        SystemUpdate();
    }

    WorldDestroy();
    SystemQuit();

    return 0;
}

//  life
