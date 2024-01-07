#ifndef RAYPOINT_H_
#define RAYPOINT_H_

#include "raylib.h"

typedef Vector2 Point;

typedef struct
{
    Point *points;
    int size, mem;
} PointSet;

#define POINT_SET_NEW (PointSet){0}

int AddPoint(PointSet *set, Point point);
int RemovePoint(PointSet *set, int index);
int MovePoint(PointSet *set, int index, Point dest);
Point GetPoint(PointSet *set, int index);
double PointDistance(Point pt1, Point pt2);
int NearestNeighbor(PointSet *set, Point query, double radius, int *inside);

#endif
