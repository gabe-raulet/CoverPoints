#include "cover.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

double PointDistance(Point pt1, Point pt2)
{
    double dx = pt1.x - pt2.x;
    double dy = pt1.y - pt2.y;

    return sqrt(dx*dx + dy*dy);
}

int CoverSetInit(CoverSet *set)
{
    if (set == NULL)
        return -1;

    set->seed = -1;
    set->size = 0;
    set->mem = 4;
    set->ready = 1;

    set->points = malloc(set->mem * sizeof(Point));
    set->perm = malloc(set->mem * sizeof(int));

    if (set->points == NULL || set->perm == NULL)
        return -1;

    return 0;
}

int CoverSetFree(CoverSet *set)
{
    if (set == NULL)
        return -1;

    if (set->points)
        free(set->points);

    if (set->perm)
        free(set->perm);

    memset(set, 0, sizeof(*set));

    return 0;
}

int CoverSetMovePoint(CoverSet *set, int index, Point dest)
{
    if (set == NULL || index < 0 || index >= set->size)
        return -1;

    set->points[index] = dest;
    set->ready = 0;

    return 0;
}

int CoverSetAddPoint(CoverSet *set, Point point)
{
    if (set == NULL)
        return -1;

    if (set->size + 1 == set->mem)
    {
        set->mem <<= 1;
        set->points = realloc(set->points, set->mem * sizeof(Point));
        set->perm = realloc(set->perm, set->mem * sizeof(int));

        if (set->points == NULL || set->perm == NULL)
            return -1;
    }

    if (set->size == 0)
        set->seed = 0;

    set->points[set->size++] = point;
    set->ready = 0;

    return 0;
}

int CoverSetRemovePoint(CoverSet *set, int index)
{
    if (set == NULL || index < 0 || index >= set->size)
        return -1;

    for (int i = index; i < set->size-1; ++i)
        set->points[i] = set->points[i+1];

    if (set->seed > index)
        set->seed--;

    set->size--;
    set->ready = 0;

    return 0;
}

int CoverSetNearestNeighbor(const CoverSet *set, Point query, double radius, int *inside)
{
    if (set == NULL || radius < 0)
        return -1;

    double mindist = 1e20;
    int index = -1;

    for (int i = 0; i < set->size; ++i)
    {
        double dist = PointDistance(set->points[i], query);

        if (dist < mindist)
        {
            mindist = dist;
            index = i;
        }
    }

    if (inside != NULL && mindist <= radius)
        *inside = 1;
    else if (inside != NULL)
        *inside = 0;

    return index;
}

int CoverSetSeedSelect(CoverSet *set, int index)
{
    if (set == NULL || index < 0 || index >= set->size)
        return -1;

    set->seed = index;
    set->ready = 0;

    return 0;
}

int FarthestFrom(CoverSet *set, int last)
{
    /*
     * Find the point farthest from perm[0..last].
     */

    if (set == NULL || last < 0 || last >= set->size)
        return -1;

    Point *points = set->points;
    int *perm = set->perm;
    int size = set->size;

    double maxdist = 0.0;
    int maxid = -1;

    for (int i = 0; i < size; ++i)
    {
        Point query = points[i];
        double mindist = 1e20;
        int minid = -1;

        for (int j = 0; j <= last; ++j)
        {
            Point pt = points[perm[j]];
            double dist = PointDistance(query, pt);

            if (dist < mindist)
            {
                mindist = dist;
                minid = perm[j];
            }
        }

        if (mindist >= maxdist)
        {
            maxdist = mindist;
            maxid = i;
        }
    }

    return maxid;
}

int ComputeGreedyPermutation(CoverSet *set)
{
    if (set == NULL)
        return -1;

    int *perm = set->perm;
    int size = set->size;
    int seed = set->seed;

    perm[0] = seed;

    for (int i = 1; i < size; ++i)
    {
        perm[i] = FarthestFrom(set, i-1);
    }

    fprintf(stderr, "Recomputed Greedy Permutation:\n");

    for (int i = 0; i < size; ++i)
    {
        fprintf(stderr, "greedy[%d] = %d\n", i, perm[i]);
    }

    fprintf(stderr, "\n");
    return 0;
}

int CoverSetUpdate(CoverSet *set)
{
    if (set == NULL)
        return -1;

    if (set->ready)
        return 0;

    ComputeGreedyPermutation(set);

    set->ready = 1;
    return 0;
}

int CoverSetDraw(CoverSet *set, const CoverState *state)
{
    if (set == NULL || state == NULL)
        return -1;

    if (!set->ready)
    {
        CoverSetUpdate(set);
    }

    assert(set->ready);

    Point *points = set->points;
    int size = set->size;
    int seed = set->seed;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i = 0; i < size; ++i)
    {
        double radius = i == state->hoverid? RADIUS_LARGE : RADIUS_SMALL;
        Color color = i == seed? RED : BLACK;
        DrawCircleV(points[i], radius, color);
    }

    EndDrawing();
    return 0;
}

int CoverStateInit(CoverState *state)
{
    if (state == NULL)
        return -1;

    memset(state, 0, sizeof(*state));
    state->dragid = state->nearestid = -1;

    return 0;
}

int CoverStateUpdate(CoverState *state, const CoverSet *set)
{
    if (state == NULL || set == NULL)
        return -1;

    state->add = state->remove = state->setseed = 0;

    int inside;
    int lpress, rpress;
    int ldown, cdown;

    state->cursor = GetMousePosition();
    state->nearestid = CoverSetNearestNeighbor(set, state->cursor, RADIUS_LARGE, &inside);
    state->hoverid = inside? state->nearestid : -1;

    lpress = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    rpress = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
    ldown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    cdown = IsKeyDown(KEY_LEFT_SUPER);

    if (!ldown)
        state->dragid = -1;
    else if (state->dragid >= 0)
        return 0;

    if (inside && rpress)
    {
        state->remove = 1;
    }
    else if (inside && lpress)
    {
        if (cdown)
        {
            state->setseed = 1;
        }
        else
        {
            state->dragid = state->hoverid;
        }
    }
    else if (!inside && lpress)
    {
        state->add = 1;
    }

    return 0;
}

