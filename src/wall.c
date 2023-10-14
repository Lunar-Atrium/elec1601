#include "wall.h"

void wallSetPosition(struct Wall *wall, int x, int y, int width, int height) {
  wall->x = x;
  wall->y = y;
  wall->width = width;
  wall->height = height;
}

void wallUpdate(SDL_Renderer *renderer, struct Wall *wall) {
  SDL_Rect rect = {wall->x, wall->y, wall->width, wall->height};
  SDL_SetRenderDrawColor(renderer, 111, 0, 255, 255);
  SDL_RenderFillRect(renderer, &rect);
  SDL_RenderDrawRect(renderer, &rect);
}

// insert link at the first location
void insertFirstWall(struct Wall_collection **head, int key,
                     struct Wall *wall) {
  // create a link
  struct Wall_collection *link =
      (struct Wall_collection *)malloc(sizeof(struct Wall_collection));

  link->key = key;
  link->wall = *wall;

  // point it to old first node
  link->next = *head;

  // point first to new first node
  *head = link;
}

void insertAndSetFirstWall(struct Wall_collection **head, int key, int x, int y,
                           int width, int height) {
  // create a link
  struct Wall *wall = (struct Wall *)malloc(sizeof(struct Wall));
  wallSetPosition(wall, x, y, width, height);
  insertFirstWall(head, key, wall);
}

void updateAllWalls(struct Wall_collection *head, SDL_Renderer *renderer) {
  struct Wall_collection *ptr = head;

  // start from the beginning
  while (ptr != NULL) {
    if (ptr->key < 0) ptr = ptr->next;  // for dynamic wall
    wallUpdate(renderer, &ptr->wall);
    ptr = ptr->next;
  }
}

int create_wall(struct Wall_collection **head, int current_wall_keys,
                int start_point[2], int end_point[2], int critical_point[2],
                int thick, double precision) {
  int record_pointBeforeCalc[2] = {start_point[0], start_point[1]};
  int record_pointAfterCalc[2] = {start_point[0], start_point[1]};
  for (double t = precision; t <= 1; t += precision) {
    record_pointAfterCalc[0] = (1 - t) * (1 - t) * start_point[0] +
                               2 * (1 - t) * t * critical_point[0] +
                               t * t * end_point[0];
    record_pointAfterCalc[1] = (1 - t) * (1 - t) * start_point[1] +
                               2 * (1 - t) * t * critical_point[1] +
                               t * t * end_point[1];
    insertAndSetFirstWall(head, current_wall_keys, record_pointBeforeCalc[0],
                          record_pointBeforeCalc[1], thick, thick);
    record_pointBeforeCalc[0] = record_pointAfterCalc[0];
    record_pointBeforeCalc[1] = record_pointAfterCalc[1];
    current_wall_keys++;
  }
  return current_wall_keys;
}

struct Wall_collection *clone_node(const struct Wall_collection *node) {
  if (!node) return NULL;

  struct Wall_collection *new_node =
      (struct Wall_collection *)malloc(sizeof(struct Wall_collection));
  if (!new_node) {
    printf("Failed to allocate memory for wall node");
    exit(EXIT_FAILURE);
  }

  new_node->key = node->key;
  new_node->wall.x = node->wall.x;
  new_node->wall.y = node->wall.y;
  new_node->wall.width = node->wall.width;
  new_node->wall.height = node->wall.height;
  new_node->next = NULL;

  return new_node;
}

struct Wall_collection *merge_walls(
    const struct Wall_collection *staticWalls,
    const struct Wall_collection *dynamicWalls) {
  struct Wall_collection *mergedHead = NULL, *mergedTail = NULL;

  while (staticWalls) {
    if (!mergedHead) {
      mergedHead = clone_node(staticWalls);
      mergedTail = mergedHead;
    } else {
      mergedTail->next = clone_node(staticWalls);
      mergedTail = mergedTail->next;
    }
    staticWalls = staticWalls->next;
  }

  while (dynamicWalls) {
    if (!mergedHead) {
      mergedHead = clone_node(dynamicWalls);
      mergedTail = mergedHead;
    } else {
      mergedTail->next = clone_node(dynamicWalls);
      mergedTail = mergedTail->next;
    }
    dynamicWalls = dynamicWalls->next;
  }
  return mergedHead;
}

struct Wall_collection *dynamicWallUpdate(SDL_Renderer *renderer,
                                          struct Wall_collection *head,
                                          int time, int reset) {
  // initialize
  static int initialize = 0;
  static int frameCounter = 0;
  static struct Wall_collection *walls_headTrack;
  struct Wall_collection *drawnWallsHead = NULL, *drawnWallsTail = NULL;

  if (reset)
    walls_headTrack = head;

  if (!initialize) {
    walls_headTrack = head;
    initialize = 1;
  }

  if (frameCounter == time && walls_headTrack->next != NULL) {
    walls_headTrack = walls_headTrack->next;
    frameCounter = 0;
  }

  // update
  struct Wall_collection *current_wall = head;
  while (current_wall != NULL && current_wall != walls_headTrack) {
    wallUpdate(renderer, &(current_wall->wall));
    if (!drawnWallsHead) { // first copy
      drawnWallsHead = clone_node(current_wall);
      drawnWallsTail = drawnWallsHead;
    } else {
      drawnWallsTail->next = clone_node(current_wall);
      drawnWallsTail = drawnWallsTail->next;
    }
    current_wall = current_wall->next;
  }
  frameCounter++;
  return drawnWallsHead;
}

void free_walls(struct Wall_collection* head) {
    while (head) {
        struct Wall_collection* current_wall = head;
        head = head->next;
        free(current_wall);
    }
}