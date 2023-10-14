#ifndef WALL_H_INCLUDED
#define WALL_H_INCLUDED

#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "stdio.h"
#include "stdlib.h"
#include "structures.h"

void wallSetPosition(struct Wall *wall, int x, int y, int width, int height);
void wallUpdate(SDL_Renderer *renderer, struct Wall *wall);

void insertFirstWall(struct Wall_collection **head, int key, struct Wall *wall);
void insertAndSetFirstWall(struct Wall_collection **head, int key, int x, int y,
                           int width, int height);
void updateAllWalls(struct Wall_collection *head, SDL_Renderer *renderer);

int create_wall(struct Wall_collection **head, int current_wall_keys,
                int start_point[2], int end_point[2], int critical_point[2],
                int thick, double precision);
struct Wall_collection *dynamicWallUpdate(SDL_Renderer *renderer,
                                          struct Wall_collection *head,
                                          int time, int mode);
struct Wall_collection *merge_walls(
    const struct Wall_collection *staticWalls,
    const struct Wall_collection *dynamicWalls);
struct Wall_collection *clone_node(const struct Wall_collection *node);
void free_walls(struct Wall_collection* head);
#endif  // WALL_H_INCLUDED
