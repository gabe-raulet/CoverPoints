#ifndef COVER_H_
#define COVER_H_

#include "raylib.h"

typedef Vector2 Point;

typedef struct
{
    Point *points;
    int *perm;
    int seed;
    int size, mem;
    int ready;
} CoverSet;

typedef struct
{
    Point cursor;
    int nearestid, dragid, hoverid;
    int add, remove, setseed;
} CoverState;


#define RADIUS_SMALL 3.0f
#define RADIUS_LARGE 6.0f

double PointDistance(Point pt1, Point pt2);

int CoverSetInit(CoverSet *set);
int CoverSetFree(CoverSet *set);
int CoverSetMovePoint(CoverSet *set, int index, Point dest);
int CoverSetAddPoint(CoverSet *set, Point point);
int CoverSetRemovePoint(CoverSet *set, int index);
int CoverSetNearestNeighbor(const CoverSet *set, Point query, double radius, int *inside);
int CoverSetSeedSelect(CoverSet *set, int index);
int CoverSetUpdate(CoverSet *set);
int CoverSetDraw(CoverSet *set, const CoverState *state);

int CoverStateInit(CoverState *state);
int CoverStateUpdate(CoverState *state, const CoverSet *set);

#endif
