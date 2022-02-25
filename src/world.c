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

int         grid_Width, grid_Height;
CELL        *grid_Array[NUMGRIDS];
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
    cell_Pos = y * grid_Width + x;
    grid_Array[STARTGRID][cell_Pos].flash = 1;
}

void WorldClear()
{
    int         grid, pos;

    for (grid = 0; grid < NUMGRIDS; grid++)
        for (pos = 0; pos < grid_Width * grid_Height; pos++)
            grid_Array[grid][pos].cell = 0;
}

void WorldReset()
{
    int         x, y;

    for (y = 0; y < grid_Height; y++)
        for (x = 0; x < grid_Width; x++)
            grid_Array[0][y * grid_Width + x].cell = grid_Array[STARTGRID][y * grid_Width + x].cell;
}

void WorldGenerate(int ticks)
{
    CELL        *old, *new;
    int         pos, state;
    int         count;

    cell_Ticks += ticks;
    if (cell_Ticks < 100)
        return;

    cell_Ticks -= 100;

    old = grid_Array[grid_Index];
    new = grid_Array[1 - grid_Index];

    for (pos = 0; pos < grid_Width * grid_Height; pos++)
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

void WorldResponder(int key)
{
    if (grid_Index != STARTGRID) // the world is generating
    {
        if (key == KEY_STOP)
        {
            WorldReset();
            grid_Index = STARTGRID;
            cell_Ticks = 0;
            game_Action = GA_CREATE;
        }

        return;
    }

    if (key == KEY_START)
    {
        cell_Ticks = 0;
        grid_Index = 0;
        game_Action = GA_GENERATE;
        return;
    }
    else if (key == KEY_CLEAR)
    {
        WorldClear();
        game_Action = GA_CREATE;
        return;
    }

    if (key == KEY_SPACE)
    {
        grid_Array[STARTGRID][cell_Pos].cell ^= 1;
        grid_Array[0][cell_Pos].cell ^= 1;
        return;
    }

    grid_Array[STARTGRID][cell_Pos].flash = 0;

    switch (key)
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

void WorldTicker(int ticks)
{
    cell_Ticks += ticks;
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

    for (pos = 0; pos < grid_Width * grid_Height; pos++)
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

    for (y = 0; y < grid_Height; y++)
        for (x = 0; x < grid_Width; x++)
        {
            pos = y * grid_Width + x;
            grid[pos].x = x;
            grid[pos].y = y;

            grid[pos].cell = 0;
            grid[pos].flash = 0;
            for (dir = 0; dir < 8; dir++)
            {
                cx = x + cell_Dir[dir][0];
                cy = y + cell_Dir[dir][1];
                if (cx == -1)
                    cx = grid_Width - 1;
                else if (cx == grid_Width)
                    cx = 0;

                if (cy == -1)
                    cy = grid_Height - 1;
                else if (cy == grid_Height)
                    cy = 0;

                grid[pos].dir[dir] = cy * grid_Width + cx;
            }
        }
}

void WorldDestroy()
{
    int         g;

    for (g = 0; g < NUMGRIDS; g++)
        free(grid_Array[g]);
}

void WorldCreate()
{
    int         g;

    grid_Width = WIDTH / CELLSIZE;
    grid_Height = HEIGHT / CELLSIZE;

    for (g = 0; g < NUMGRIDS; g++)
    {
        grid_Array[g] = malloc(grid_Width * grid_Height * sizeof(CELL));
        WorldSetupGrid(grid_Array[g]);
    }

    cell_Pos = grid_Height * grid_Width / 2 + grid_Width / 2;
    cell_Ticks = 0;
    cell_Flash = 0;

    grid_Index = STARTGRID;
    grid_Array[STARTGRID][cell_Pos].flash = 1;
}

//  life
