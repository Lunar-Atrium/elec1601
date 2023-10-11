#include "path_track.h"

int COORDINATE_OFFSET = 10;

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

int existsCoordinate(PathHashTable* table, int x, int y, int angle) {
  int index = hash(table, x, y);
  PathTrackNode* currentNode = table->nodes[index];
  int coordinate_overlap;
  int angle_overlap;
  while (currentNode) {
    coordinate_overlap = currentNode->coord.x <= x + COORDINATE_OFFSET &&
                         currentNode->coord.x >= x - COORDINATE_OFFSET &&
                         currentNode->coord.y <= y + COORDINATE_OFFSET &&
                         currentNode->coord.y >= y - COORDINATE_OFFSET;
    angle_overlap = (currentNode->coord.angle <= abs(angle - 120) &&
                     currentNode->coord.angle >= abs(angle - 240));
    if (coordinate_overlap) {
      table->coordinatesAccessedCount++;
      if (table->coordinatesAccessedCount == 1) return 1;  // found
    }
    currentNode = currentNode->next;
  }
  PathTrackNode* newNode = malloc(sizeof(PathTrackNode));
  newNode->coord.x = x;
  newNode->coord.y = y;
  newNode->coord.angle = angle;
  newNode->next =
      table
          ->nodes[index];  // table->table[index] == NULL because initialization
  table->nodes[index] = newNode;
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