#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "formulas.h"
#include "robot.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "wall.h"

// track coordinates
#include "path_track.h"

void my_custom_maze(struct Wall_collection **head);
int DONE = 0;

int main(int argc, char *argv[]) {
  // setup window and renderer
  SDL_Window *window;
  SDL_Renderer *renderer;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
  window = SDL_CreateWindow("Robot Maze", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, OVERALL_WINDOW_WIDTH,
                            OVERALL_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
  renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_Event event;

  // setup robot
  struct Robot robot;
  int front_centre_sensor, left_sensor, right_sensor = 0;
  int crashed = 0;
  int moveTerminate = 0;
  setup_robot(&robot);

  // get real time
  clock_t start_time, end_time;
  time_t start_linuxTimestamp, end_linuxTimestamp;

  // setup robot path track
  PathHashTable *table;
  table = createPathHashTable(TABLE_BUFFER);
  int exist_coordinate;
  CurrentCoordinate coordinate;
  coordinate.x = robot.x + ROBOT_WIDTH / 2;
  coordinate.y = robot.y + ROBOT_HEIGHT / 2;

  // SETUP MAZE
  struct Wall_collection *static_wall_head = NULL;
  struct Wall_collection *dynamic_wall_head = NULL;
  struct Wall_collection *walls = NULL;
  my_custom_maze(&static_wall_head);
  create_wall(&dynamic_wall_head, -1, (int[2]){230, 200}, (int[2]){330, 200},
              (int[2]){330, 200}, 10, 0.05);

  // simulation main loop
  while (!DONE) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // update
    struct Wall_collection *drownWall = NULL;
    
    robotUpdate(renderer, &robot);
    free_walls(walls);
    updateAllWalls(static_wall_head, renderer);
    
    if (robot.auto_mode == 1) 
      drownWall = dynamicWallUpdate(renderer, dynamic_wall_head, 30, 0);
    walls = merge_walls(static_wall_head, drownWall);
    
    if (!exist_coordinate) {
      drawCoordinates(table, renderer);
    } else {
      // free memory and re-generate hashtable
      freeHashTable(table);
      table = createPathHashTable(TABLE_BUFFER);
    }

    // Move robot based on user input commands/auto commands
    if (robot.auto_mode == 1)
      robotAutoMotorMove(&robot, front_centre_sensor, left_sensor,
                         right_sensor);
    robotMotorMove(&robot, crashed);

    // Check if robot reaches endpoint. and check sensor values
    if (checkRobotReachedEnd(&robot, OVERALL_WINDOW_WIDTH - 20,
                             OVERALL_WINDOW_HEIGHT / 2 - 190, 10, 100)) {
      end_time = clock();
      end_linuxTimestamp = time(NULL);
      // msec = (end_time - start_time) * 1000 / CLOCKS_PER_SEC;
      int sec = end_linuxTimestamp - start_linuxTimestamp;
      int msec = end_time - start_time;
      moveTerminate = robotSuccess(&robot, sec, msec);

    } else if (crashed == 1 || checkRobotHitWalls(&robot, walls)) {
      robotCrash(&robot);
      crashed = 1;

    } else {  // Otherwise compute sensor information
      front_centre_sensor = checkRobotSensorFrontCentreAllWalls(&robot, walls);
      left_sensor = checkRobotSensorLeftAllWalls(&robot, walls);
      right_sensor = checkRobotSensorRightAllWalls(&robot, walls);
    }

    // Check for user input
    SDL_RenderPresent(renderer);
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        DONE = 1;
      }
      const Uint8 *state = SDL_GetKeyboardState(NULL);
      if (state[SDL_SCANCODE_UP] && robot.direction != DOWN) {
        robot.direction = UP;
      }
      if (state[SDL_SCANCODE_DOWN] && robot.direction != UP) {
        robot.direction = DOWN;
      }
      if (state[SDL_SCANCODE_LEFT] && robot.direction != RIGHT) {
        robot.direction = LEFT;
      }
      if (state[SDL_SCANCODE_RIGHT] && robot.direction != LEFT) {
        robot.direction = RIGHT;
      }
      if (state[SDL_SCANCODE_SPACE]) {
        setup_robot(&robot);
        crashed = 0;
        // free memory and re-generate hashtable
        freeHashTable(table);
        table = createPathHashTable(TABLE_BUFFER);
        //re-generate dynamic wall
        dynamicWallUpdate(renderer, dynamic_wall_head, 60, 1);
      }
      if (state[SDL_SCANCODE_RETURN]) {
        robot.auto_mode = 1;
        start_time = clock();
        start_linuxTimestamp = time(NULL);
      }
    }

    // update coordinate
    if (coordinate.x != robot.x + ROBOT_WIDTH / 2 ||
        coordinate.y != robot.y + ROBOT_HEIGHT / 2) {
      coordinate.x = robot.x + ROBOT_WIDTH / 2;
      coordinate.y = robot.y + ROBOT_HEIGHT / 2;
      exist_coordinate = existsCoordinate(table, coordinate.x, coordinate.y);
      if (!moveTerminate)
        printf(
            "current coordinates: %3d %3d if have gone through: %d\tsensor "
            "detect: %d %d %d\n",
            coordinate.x, coordinate.y, exist_coordinate, left_sensor,
            front_centre_sensor, right_sensor);
    }

    // render
    SDL_RenderPresent(renderer);
    SDL_Delay(120);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void my_custom_maze(struct Wall_collection **head) {
  int wall_keys = 0;
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 - 50,
                        OVERALL_WINDOW_HEIGHT / 2 + 200, 10,
                        OVERALL_WINDOW_HEIGHT / 2 - 200);
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 - 150,
                        OVERALL_WINDOW_HEIGHT / 2 + 200, 10,
                        OVERALL_WINDOW_HEIGHT / 2 - 200);
  wall_keys = create_wall(
      head, ++wall_keys,
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 200, OVERALL_WINDOW_HEIGHT / 2 + 100},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 50, OVERALL_WINDOW_HEIGHT / 2 + 200},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 200, OVERALL_WINDOW_HEIGHT / 2 + 100},
      8, 0.005);
  wall_keys = create_wall(
      head, ++wall_keys,
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 300, OVERALL_WINDOW_HEIGHT / 2 + 100},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 150, OVERALL_WINDOW_HEIGHT / 2 + 200},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 300, OVERALL_WINDOW_HEIGHT / 2 + 100},
      8, 0.005);
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 - 200,
                        OVERALL_WINDOW_HEIGHT / 2 + 50, 10, 50);
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 - 300,
                        OVERALL_WINDOW_HEIGHT / 2 + 50, 10, 50);
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 - 300,
                        OVERALL_WINDOW_HEIGHT / 2 - 220, 10, 175);
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 - 230,
                        OVERALL_WINDOW_HEIGHT / 2 - 160, 10, 25);
  wall_keys = create_wall(
      head, ++wall_keys,
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 200, OVERALL_WINDOW_HEIGHT / 2 + 50},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 80, OVERALL_WINDOW_HEIGHT / 2 - 135},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 200, OVERALL_WINDOW_HEIGHT / 2 + 50},
      8, 0.005);
  wall_keys = create_wall(
      head, ++wall_keys,
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 230, OVERALL_WINDOW_HEIGHT / 2 - 45},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 300, OVERALL_WINDOW_HEIGHT / 2 + 50},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 230, OVERALL_WINDOW_HEIGHT / 2 - 45},
      8, 0.005);
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 - 300,
                        OVERALL_WINDOW_HEIGHT / 2 - 45, 70, 10);
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 - 230,
                        OVERALL_WINDOW_HEIGHT / 2 - 135, 150, 10);
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 - 230,
                        OVERALL_WINDOW_HEIGHT / 2 - 160, 180, 10);
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 - 300,
                        OVERALL_WINDOW_HEIGHT / 2 - 220, 330, 10);
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 + 30,
                        OVERALL_WINDOW_HEIGHT / 2 - 220, 10, 60);
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 + 240,
                        OVERALL_WINDOW_HEIGHT / 2 - 160,
                        OVERALL_WINDOW_WIDTH / 2 - 240, 10);
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 + 100,
                        OVERALL_WINDOW_HEIGHT / 2 - 220,
                        OVERALL_WINDOW_WIDTH / 2 - 100, 10);
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 + 100,
                        OVERALL_WINDOW_HEIGHT / 2 - 220, 10, 175);
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 + 100,
                        OVERALL_WINDOW_HEIGHT / 2 - 45, 50, 10);
  insertAndSetFirstWall(head, ++wall_keys, OVERALL_WINDOW_WIDTH / 2 + 240,
                        OVERALL_WINDOW_HEIGHT / 2 - 135, 70, 10);
  wall_keys = create_wall(
      head, ++wall_keys,
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 240, OVERALL_WINDOW_HEIGHT / 2 - 160},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 240, OVERALL_WINDOW_HEIGHT / 2 - 135},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 125, OVERALL_WINDOW_HEIGHT / 2 - 150},
      10, 0.005);
  wall_keys = create_wall(
      head, ++wall_keys,
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 150, OVERALL_WINDOW_HEIGHT / 2 - 45},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 60, OVERALL_WINDOW_HEIGHT / 2 + 95},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 200, OVERALL_WINDOW_HEIGHT / 2 + 50},
      10, 0.005);
  wall_keys = create_wall(
      head, ++wall_keys,
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 10, OVERALL_WINDOW_HEIGHT / 2 + 215},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 310, OVERALL_WINDOW_HEIGHT / 2 - 135},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 310, OVERALL_WINDOW_HEIGHT / 2 + 150},
      10, 0.005);
  wall_keys = create_wall(
      head, ++wall_keys,
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 30, OVERALL_WINDOW_HEIGHT / 2 - 160},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 50, OVERALL_WINDOW_HEIGHT / 2 - 85},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 110, OVERALL_WINDOW_HEIGHT / 2 - 120},
      8, 0.005);  // c1
  wall_keys = create_wall(
      head, ++wall_keys,
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 70, OVERALL_WINDOW_HEIGHT / 2},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 60, OVERALL_WINDOW_HEIGHT / 2 + 95},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 130, OVERALL_WINDOW_HEIGHT / 2 + 50},
      8, 0.005);  // c3
  wall_keys = create_wall(
      head, ++wall_keys,
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 50, OVERALL_WINDOW_HEIGHT / 2 - 85},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 70, OVERALL_WINDOW_HEIGHT / 2},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 20, OVERALL_WINDOW_HEIGHT / 2 - 25},
      8, 0.005);  // c5
  wall_keys = create_wall(
      head, ++wall_keys,
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 50, OVERALL_WINDOW_HEIGHT / 2 - 160},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 40, OVERALL_WINDOW_HEIGHT / 2 - 85},
      (int[2]){OVERALL_WINDOW_WIDTH / 2, OVERALL_WINDOW_HEIGHT / 2 - 130}, 8,
      0.005);  // c2
  wall_keys = create_wall(
      head, ++wall_keys,
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 20, OVERALL_WINDOW_HEIGHT / 2 + 20},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 60, OVERALL_WINDOW_HEIGHT / 2 + 125},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 + 30, OVERALL_WINDOW_HEIGHT / 2 + 50},
      8, 0.005);  // c4
  wall_keys = create_wall(
      head, ++wall_keys,
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 40, OVERALL_WINDOW_HEIGHT / 2 - 85},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 20, OVERALL_WINDOW_HEIGHT / 2 + 20},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 130, OVERALL_WINDOW_HEIGHT / 2 - 25},
      8, 0.005);  // c6
  wall_keys = create_wall(
      head, ++wall_keys,
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 60, OVERALL_WINDOW_HEIGHT / 2 + 125},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 10, OVERALL_WINDOW_HEIGHT / 2 + 215},
      (int[2]){OVERALL_WINDOW_WIDTH / 2 - 100, OVERALL_WINDOW_HEIGHT / 2 + 155},
      8, 0.005);  // c7
}