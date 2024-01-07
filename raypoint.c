#include "raypoint.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

int AddPoint(PointSet *set, Point point)
{
    if (set == NULL)
        return -1;

    if (set->points == NULL)
    {
        set->size = 0;
        set->mem = 4;
        set->points = malloc(set->mem * sizeof(Point));

        if (set->points == NULL)
            return -1;
    }

    if (set->size + 1 >= set->mem)
    {
        set->mem <<= 1;
        set->points = realloc(set->points, set->mem * sizeof(Point));

        if (set->points == NULL)
            return -1;
    }

    set->points[set->size++] = point;

    return 0;
}

int RemovePoint(PointSet *set, int index)
{
    if (set == NULL || index < 0 || index >= set->size)
        return -1;

    for (int i = index; i < set->size-1; ++i)
        set->points[i] = set->points[i+1];

    set->size--;

    return 0;
}

int MovePoint(PointSet *set, int index, Point dest)
{
    if (set == NULL || index < 0 || index >= set->size)
        return -1;

    set->points[index] = dest;

    return 0;
}

Point GetPoint(PointSet *set, int index)
{
    if (set == NULL || index < 0 || index >= set->size)
        return (Point){0,0};

    return set->points[index];
}

double PointDistance(Point pt1, Point pt2)
{
    double dx = pt1.x - pt2.x;
    double dy = pt1.y - pt2.y;

    return sqrt(dx*dx + dy*dy);
}

int NearestNeighbor(PointSet *set, Point query, double radius, int *inside)
{
    if (set == NULL || radius < 0)
        return -1;

    double mindist = 1e20;
    int index = -1;

    for (int i = 0; i < set->size; ++i)
    {
        Point pt = set->points[i];
        double dist = PointDistance(pt, query);

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
