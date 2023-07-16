//  life

#include <stdlib.h>

#include "common.h"
#include "system.h"

#define NUMGRIDS        3
#define STARTGRID       NUMGRIDS - 1

typedef struct
{
    int         cell;
    int         dir[8];
    int         x, y;
    int         flash;
} CELL;

CELL        grid_Array[NUMGRIDS][WIDTH * HEIGHT];
int         grid_Index;
int         cell_Pos, cell_Ticks, cell_Flash;

int         cell_Dir[8][2] =
{
    {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}
};

int         count_Set[9] = {0, 0, 2, 3, 0, 0, 0, 0, 0};

void WorldCellPosChange(int x, int y)
{
    if (grid_Index != STARTGRID)
        return;

    grid_Array[STARTGRID][cell_Pos].flash = 0;
    cell_Pos = y * WIDTH + x;
    grid_Array[STARTGRID][cell_Pos].flash = 1;
}

void WorldClear()
{
    int         grid, pos;

    for (grid = 0; grid < NUMGRIDS; grid++)
        for (pos = 0; pos < WIDTH * HEIGHT; pos++)
            grid_Array[grid][pos].cell = 0;
}

void WorldReset()
{
    int         x, y;

    for (y = 0; y < HEIGHT; y++)
        for (x = 0; x < WIDTH; x++)
            grid_Array[0][y * WIDTH + x].cell = grid_Array[STARTGRID][y * WIDTH + x].cell;
}

void WorldGenerate()
{
    CELL        *old, *new;
    int         pos, state;
    int         count;

    cell_Ticks += SystemTicks();
    if (cell_Ticks < 100)
        return;

    cell_Ticks -= 100;

    old = grid_Array[grid_Index];
    new = grid_Array[1 - grid_Index];

    for (pos = 0; pos < WIDTH * HEIGHT; pos++)
    {
        count = old[old[pos].dir[0]].cell;
        count += old[old[pos].dir[1]].cell;
        count += old[old[pos].dir[2]].cell;
        count += old[old[pos].dir[3]].cell;
        count += old[old[pos].dir[4]].cell;
        count += old[old[pos].dir[5]].cell;
        count += old[old[pos].dir[6]].cell;
        count += old[old[pos].dir[7]].cell;

        state = old[pos].cell;
        new[pos].cell = (count_Set[count] & (1 << state)) >> state;
    }

    grid_Index = 1 - grid_Index;
}

void WorldGenerateResponder()
{
    if (gameKey == KEY_STOP)
    {
        WorldReset();
        grid_Index = STARTGRID;
        cell_Ticks = 0;
        Ticker = WorldTicker;
        Responder = WorldCreateResponder;
    }
}

void WorldCreateResponder()
{
    if (gameKey == KEY_START)
    {
        cell_Ticks = 0;
        grid_Index = 0;
        Ticker = WorldGenerate;
        Responder = WorldGenerateResponder;
        return;
    }
    else if (gameKey == KEY_CLEAR)
    {
        WorldClear();
        Ticker = WorldTicker;
        return;
    }

    if (gameKey == KEY_SPACE)
    {
        grid_Array[STARTGRID][cell_Pos].cell ^= 1;
        grid_Array[0][cell_Pos].cell ^= 1;
        return;
    }

    grid_Array[STARTGRID][cell_Pos].flash = 0;

    switch (gameKey)
    {
      case KEY_LEFT:
        cell_Pos = grid_Array[STARTGRID][cell_Pos].dir[6];
        break;

      case KEY_RIGHT:
        cell_Pos = grid_Array[STARTGRID][cell_Pos].dir[2];
        break;

      case KEY_UP:
        cell_Pos = grid_Array[STARTGRID][cell_Pos].dir[0];
        break;

      case KEY_DOWN:
        cell_Pos = grid_Array[STARTGRID][cell_Pos].dir[4];
        break;

      default:
        break;
    }

    grid_Array[STARTGRID][cell_Pos].flash = 1;
}

void WorldTicker()
{
    cell_Ticks += SystemTicks();
    if (cell_Ticks < 500)
        return;

    cell_Ticks -= 500;
    cell_Flash = 1 - cell_Flash;
}

void WorldDrawer()
{
    CELL        *grid;
    int         pos;

    grid = grid_Array[grid_Index];

    for (pos = 0; pos < WIDTH * HEIGHT; pos++)
    {
        if (grid_Index == STARTGRID)
            SystemDraw(grid[pos].x, grid[pos].y, grid[pos].flash & cell_Flash, 0);

        SystemDraw(grid[pos].x, grid[pos].y, 1 - grid[pos].cell, grid_Index == STARTGRID);
    }
}

void WorldSetupGrid(CELL *grid)
{
    int         x, y, pos;
    int         dir, cx, cy;

    for (y = 0; y < HEIGHT; y++)
        for (x = 0; x < WIDTH; x++)
        {
            pos = y * WIDTH + x;
            grid[pos].x = x;
            grid[pos].y = y;

            grid[pos].cell = 0;
            grid[pos].flash = 0;
            for (dir = 0; dir < 8; dir++)
            {
                cx = x + cell_Dir[dir][0];
                cy = y + cell_Dir[dir][1];
                if (cx == -1)
                    cx = WIDTH - 1;
                else if (cx == WIDTH)
                    cx = 0;

                if (cy == -1)
                    cy = HEIGHT - 1;
                else if (cy == HEIGHT)
                    cy = 0;

                grid[pos].dir[dir] = cy * WIDTH + cx;
            }
        }
}

void WorldCreate()
{
    int         g;

    for (g = 0; g < NUMGRIDS; g++)
    {
        WorldSetupGrid(grid_Array[g]);
    }

    cell_Pos = WIDTH * HEIGHT / 2 + WIDTH / 2;
    cell_Ticks = 0;
    cell_Flash = 0;

    grid_Index = STARTGRID;
    grid_Array[STARTGRID][cell_Pos].flash = 1;
}

//  life
