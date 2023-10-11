#include "path_track.h"

// TODO: draw a graph
int hash(PathHashTable* table, int x, int y) {
  unsigned int h = 0;
  h = 31 * h + x;
  h = 31 * h + y;
  return h % table->size;
}

PathHashTable* createPathHashTable(int size) {
  PathHashTable* newTable = (PathHashTable*)malloc(sizeof(PathHashTable));
  newTable->size = size;
  newTable->coordinatesAccessedCount = 0;
  newTable->nodes = malloc(sizeof(PathTrackNode*) * size);
  for (int i = 0; i < size; i++) newTable->nodes[i] = NULL;  // initialization
  return newTable;
}

void insertCoordinate(PathHashTable* table, int x, int y, int angle) {
  int index = hash(table, x, y);
  PathTrackNode* newNode = malloc(sizeof(PathTrackNode));
  newNode->coord.x = x;
  newNode->coord.y = y;
  newNode->coord.angle = angle;
  newNode->next =
      table
          ->nodes[index];  // table->table[index] == NULL because initialization
  table->nodes[index] = newNode;
}

int existsCoordinate(PathHashTable* table, int x, int y, int angle) {
  int index = hash(table, x, y);
  PathTrackNode* currentNode = table->nodes[index];
  while (currentNode) {
    if (currentNode->coord.x == x && currentNode->coord.y == y &&
        (currentNode->coord.angle <= angle - 150 &&
         currentNode->coord.angle >= angle - 210)) {
      table->coordinatesAccessedCount++;
      if (table->coordinatesAccessedCount >= 5) return 1;  // found
    }
    currentNode = currentNode->next;
  }
  return 0;  // not found
}

void freeHashTable(PathHashTable* table) {
  for (int i = 0; i < table->size; i++) {
    PathTrackNode* currentNode = table->nodes[i];
    while (currentNode) {
      PathTrackNode* temp = currentNode;
      currentNode = currentNode->next;
      free(temp);
    }
  }
  free(table->nodes);
  free(table);
}