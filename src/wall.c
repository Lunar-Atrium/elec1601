#include "wall.h"

void wallSetPosition(struct Wall * wall, int x, int y, int width, int height) {
    wall->x = x;
    wall->y = y;
    wall->width = width;
    wall->height = height;
}

void wallUpdate(SDL_Renderer * renderer, struct Wall * wall){
    SDL_Rect rect = {wall->x, wall->y, wall->width, wall->height};
    SDL_SetRenderDrawColor(renderer, 111, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderDrawRect(renderer, &rect);
}

//insert link at the first location
void insertFirstWall(struct Wall_collection ** head, int key, struct Wall * wall) {
   //create a link
   struct Wall_collection *link = (struct Wall_collection*) malloc(sizeof(struct Wall_collection));

   link->key = key;
   link->wall = *wall;

   //point it to old first node
   link->next = *head;

   //point first to new first node
   *head = link;
}

void insertAndSetFirstWall(struct Wall_collection ** head, int key, int x, int y, int width, int height){
   //create a link
   struct Wall *wall = (struct Wall*) malloc(sizeof(struct Wall));
   wallSetPosition(wall, x, y, width, height);
   insertFirstWall(head, key, wall);

}

void updateAllWalls(struct Wall_collection * head, SDL_Renderer * renderer) {
   struct Wall_collection *ptr = head;

   //start from the beginning
   while(ptr != NULL) {
      //printf("(%d)",ptr->key);
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
  return ++current_wall_keys;
}

void update_dynamic_wall(SDL_Renderer *renderer, struct Wall_collection *head,
                         int speed) {
  static int initialize = 0;
  static int frameCounter = 0;

  // initialize
  static struct Wall_collection *walls_headTrack;
  if (!initialize) {
    walls_headTrack = head;
    initialize = 1;
  }

  struct Wall_collection *current_wall = head;
  if (frameCounter % speed == 0) {
    if (walls_headTrack == NULL) {
      walls_headTrack = head;
      frameCounter = 0;
    }
    walls_headTrack = walls_headTrack->next;
  }
  while (current_wall != NULL && current_wall != walls_headTrack) {
    SDL_SetRenderDrawColor(renderer, 207, 99, 85, 255);
    SDL_Rect rect = {(current_wall->wall).x, (current_wall->wall).y,
                     (current_wall->wall).width, (current_wall->wall).height};
    SDL_RenderFillRect(renderer, &rect);
    current_wall = current_wall->next;
  }
  frameCounter++;
}
