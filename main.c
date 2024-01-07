#include "raylib.h"
#include "raypoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define SMALL_RADIUS 3.0f
#define LARGE_RADIUS 6.0f

int DrawPoints(PointSet set);

int main(int argc, char *argv[])
{
    PointSet set = POINT_SET_NEW;

    InitWindow(1000, 600, "PointSet");

    while (!WindowShouldClose())
    {
        Point cursor = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            AddPoint(&set, cursor);
        }

        DrawPoints(set);
    }

    CloseWindow();
    return 0;
}

int DrawPoints(PointSet set)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i = 0; i < set.size; ++i)
    {
        Point pt = set.points[i];
        DrawCircleV(pt, SMALL_RADIUS, BLACK);
    }

    EndDrawing();

    return 0;
}
