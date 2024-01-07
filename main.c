#include "raylib.h"
#include "cover.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

int main(int argc, char *argv[])
{
    if (argc != 1 && argc != 3)
    {
        fprintf(stderr, "Usage: %s [options: <width> <height>]\n", argv[0]);
        return -1;
    }

    int width = argc != 3? 800 : atoi(argv[1]);
    int height = argc != 3? 450 : atoi(argv[2]);

    InitWindow(width, height, "CoverSet");

    CoverSet set;
    CoverSetInit(&set);

    CoverState state;
    CoverStateInit(&state);

    while (!WindowShouldClose())
    {
        CoverStateUpdate(&state, &set);

        if (state.dragid >= 0)
        {
            CoverSetMovePoint(&set, state.dragid, state.cursor);
        }
        else if (state.add)
        {
            CoverSetAddPoint(&set, state.cursor);
        }
        else if (state.remove)
        {
            CoverSetRemovePoint(&set, state.hoverid);
        }
        else if (state.setseed)
        {
            CoverSetSeedSelect(&set, state.hoverid);
        }

        CoverSetDraw(&set);
    }

    CoverSetFree(&set);
    CloseWindow();
    return 0;
}
