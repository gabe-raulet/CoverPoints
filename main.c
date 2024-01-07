#include "raylib.h"
#include "raypoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define SMALL_RADIUS 3.0f
#define LARGE_RADIUS 6.0f

int DrawPoints(PointSet set, int hover_index);

int main(int argc, char *argv[])
{
    PointSet set = POINT_SET_NEW;

    InitWindow(1000, 600, "PointSet");

    Point cursor;
    int inside;
    int hover_index;
    int left_pressed, right_pressed, pressed;

    while (!WindowShouldClose())
    {
        cursor = GetMousePosition();
        left_pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        right_pressed = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
        pressed = left_pressed || right_pressed;
        hover_index = NearestNeighbor(&set, cursor, LARGE_RADIUS, &inside);

        if (pressed)
        {
            if (inside)
            {
                if (right_pressed)
                {
                    RemovePoint(&set, hover_index);
                }
            }
            else
            {
                if (left_pressed)
                {
                    AddPoint(&set, cursor);
                }
            }
        }

        hover_index = inside? hover_index : -1;
        DrawPoints(set, hover_index);
    }

    CloseWindow();
    return 0;
}

int DrawPoints(PointSet set, int hover_index)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i = 0; i < set.size; ++i)
    {
        Point pt = set.points[i];
        double radius = i == hover_index? LARGE_RADIUS : SMALL_RADIUS;
        DrawCircleV(pt, radius, BLACK);
    }

    EndDrawing();

    return 0;
}
