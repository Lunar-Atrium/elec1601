#ifndef PATH_TRACK_H_INCLUDED
#define PATH_TRACK_H_INCLUDED
#include <robot.h>

#define TABLE_BUFFER 256

typedef struct CurrentCoordinate {
    int x;
    int y;
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
int existsCoordinate(PathHashTable* table, int x, int y);
void freeHashTable(PathHashTable* table);
void drawCoordinates(PathHashTable* table, SDL_Renderer *renderer);
#endif