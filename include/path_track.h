#ifndef PATH_TRACK_H_INCLUDED
#define PATH_TRACK_H_INCLUDED
#include <stdlib.h>

typedef struct CurrentCoordinate {
    int x;
    int y;
    int angle;
} CurrentCoordinate;

typedef struct PathTrackNode {
    CurrentCoordinate coord;
    struct PathTrackNode* next;
} PathTrackNode;

typedef struct PathHashTable {
    int size;
    int coordinatesAccessedCount;
    PathTrackNode** nodes;
} PathHashTable;

PathHashTable *createPathHashTable(int size);
void insertCoordinate(PathHashTable *table, int x, int y, int angle);
int existsCoordinate(PathHashTable* table, int x, int y, int angle);
void freeHashTable(PathHashTable* table);
#endif