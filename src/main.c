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
  struct Wall_collection *static_walls_head = NULL;
  insertAndSetFirstWall(&static_walls_head, 1, OVERALL_WINDOW_WIDTH / 2,
                        OVERALL_WINDOW_HEIGHT / 2, 10,
                        OVERALL_WINDOW_HEIGHT / 2);
  insertAndSetFirstWall(&static_walls_head, 2, OVERALL_WINDOW_WIDTH / 2 - 100,
                        OVERALL_WINDOW_HEIGHT / 2 + 100, 10,
                        OVERALL_WINDOW_HEIGHT / 2 - 100);
  insertAndSetFirstWall(&static_walls_head, 3, OVERALL_WINDOW_WIDTH / 2 - 250,
                        OVERALL_WINDOW_HEIGHT / 2 + 100, 150, 10);
  insertAndSetFirstWall(&static_walls_head, 4, OVERALL_WINDOW_WIDTH / 2 - 150,
                        OVERALL_WINDOW_HEIGHT / 2, 150, 10);
  insertAndSetFirstWall(&static_walls_head, 5, OVERALL_WINDOW_WIDTH / 2 - 250,
                        OVERALL_WINDOW_HEIGHT / 2 - 200, 10, 300);
  insertAndSetFirstWall(&static_walls_head, 6, OVERALL_WINDOW_WIDTH / 2 - 150,
                        OVERALL_WINDOW_HEIGHT / 2 - 100, 10, 100);
  insertAndSetFirstWall(&static_walls_head, 7, OVERALL_WINDOW_WIDTH / 2 - 250,
                        OVERALL_WINDOW_HEIGHT / 2 - 200, 450, 10);
  insertAndSetFirstWall(&static_walls_head, 8, OVERALL_WINDOW_WIDTH / 2 - 150,
                        OVERALL_WINDOW_HEIGHT / 2 - 100, 250, 10);
  insertAndSetFirstWall(&static_walls_head, 9, OVERALL_WINDOW_WIDTH / 2 + 200,
                        OVERALL_WINDOW_HEIGHT / 2 - 200, 10, 300);
  insertAndSetFirstWall(&static_walls_head, 10, OVERALL_WINDOW_WIDTH / 2 + 100,
                        OVERALL_WINDOW_HEIGHT / 2 - 100, 10, 300);
  insertAndSetFirstWall(&static_walls_head, 11, OVERALL_WINDOW_WIDTH / 2 + 100,
                        OVERALL_WINDOW_HEIGHT / 2 + 200,
                        OVERALL_WINDOW_WIDTH / 2 - 100, 10);
  insertAndSetFirstWall(&static_walls_head, 12, OVERALL_WINDOW_WIDTH / 2 + 200,
                        OVERALL_WINDOW_HEIGHT / 2 + 100,
                       OVERALL_WINDOW_WIDTH / 2 - 100, 10);

  // draw dynamic wall
  struct Wall_collection *dynamic_walls_head1 = NULL;
  create_wall(&dynamic_walls_head1, 13, (int[2]){200, 200}, (int[2]){300, 200},
              (int[2]){250, 250}, 10, 0.01);
  struct Wall_collection *dynamic_walls_head2 = NULL;
  create_wall(&dynamic_walls_head2, 13, (int[2]){400, 200}, (int[2]){500, 200},
              (int[2]){450, 250}, 10, 0.01);

  // simulation main loop
  while (!DONE) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderClear(renderer);

    // Move robot based on user input commands/auto commands
    if (robot.auto_mode == 1)
      robotAutoMotorMove(&robot, front_centre_sensor, left_sensor, right_sensor,
                         exist_coordinate);
    robotMotorMove(&robot, crashed);

    // Check if robot reaches endpoint. and check sensor values
    if (checkRobotReachedEnd(&robot, OVERALL_WINDOW_WIDTH,
                             OVERALL_WINDOW_HEIGHT / 2 + 100, 10, 100)) {
      end_time = clock();
      end_linuxTimestamp = time(NULL);
      // msec = (end_time - start_time) * 1000 / CLOCKS_PER_SEC;
      int sec = end_linuxTimestamp - start_linuxTimestamp;
      int msec = end_time - start_time;
      moveTerminate = robotSuccess(&robot, sec, msec);

    } else if (crashed == 1 || checkRobotHitWalls(&robot, static_walls_head)) {
      robotCrash(&robot);
      crashed = 1;

    } else {  // Otherwise compute sensor information
      front_centre_sensor =
          checkRobotSensorFrontCentreAllWalls(&robot, static_walls_head);
      left_sensor = checkRobotSensorLeftAllWalls(&robot, static_walls_head);
      right_sensor = checkRobotSensorRightAllWalls(&robot, static_walls_head);
    }

    robotUpdate(renderer, &robot);
    updateAllWalls(static_walls_head, renderer);

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
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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
      if (!exist_coordinate) {
        drawCoordinates(table, renderer);
      } else {
        // destroy paths(set paths as background color)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        // free memory and re-generate hashtable
        freeHashTable(table);
        table = createPathHashTable(TABLE_BUFFER);
      }
      if (!moveTerminate) {
        printf(
            "current coordinates: %3d %3d if have gone through: %d\tsensor "
            "detect: %d %d %d\n",
            coordinate.x, coordinate.y, exist_coordinate, left_sensor,
            front_centre_sensor, right_sensor);
      }
    }

    // update dynamic wall
    // update_dynamic_wall(renderer, dynamic_walls_head1, 1);

    // render
    SDL_RenderPresent(renderer);
    SDL_Delay(120);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}