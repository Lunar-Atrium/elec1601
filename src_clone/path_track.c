#include "path_track.h"

int COORDINATE_OFFSET = 10;
int OVERLAP_COUNT = 1;

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

int existsCoordinate(PathHashTable* table, int x, int y) {
  int index = hash(table, x, y);
  PathTrackNode* currentNode = table->nodes[index];
  int coordinate_overlap;
  int angle_overlap;
  while (currentNode) {
    coordinate_overlap = currentNode->coord.x <= x + COORDINATE_OFFSET &&
                         currentNode->coord.x >= x - COORDINATE_OFFSET &&
                         currentNode->coord.y <= y + COORDINATE_OFFSET &&
                         currentNode->coord.y >= y - COORDINATE_OFFSET;
    if (coordinate_overlap) {
      table->coordinatesAccessedCount++;
      if (table->coordinatesAccessedCount == OVERLAP_COUNT) return 1;  // found
    }
    currentNode = currentNode->next;
  }
  PathTrackNode* newNode = malloc(sizeof(PathTrackNode));
  newNode->coord.x = x;
  newNode->coord.y = y;
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

void drawCoordinates(PathHashTable* table, SDL_Renderer *renderer) {
    for (int i = 0; i < table->size; i++) {
        PathTrackNode* currentNode = table->nodes[i];
        while (currentNode) {
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
          SDL_RenderDrawPoint(renderer, currentNode->coord.x, currentNode->coord.y);
          currentNode = currentNode->next;
        }
    }
}