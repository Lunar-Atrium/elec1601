/*https://github.com/Lunar-Atrium/elec1601*/

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
  // struct Wall_collection *dynamic_wall_head = NULL;
  struct Wall_collection *walls = NULL;
  my_custom_maze(&walls);
  // create_wall(&dynamic_wall_head, -1, (int[2]){230, 200}, (int[2]){330, 200},
              // (int[2]){330, 200}, 10, 0.05);

  // simulation main loop
  while (!DONE) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // update
    // struct Wall_collection *drownWall = NULL;

    robotUpdate(renderer, &robot);
    updateAllWalls(walls, renderer);

    // free_walls(walls);
    // if (robot.auto_mode == 1)
    //   drownWall = dynamicWallUpdate(renderer, dynamic_wall_head, 50, 0);
    // walls = merge_walls(static_wall_head, drownWall);

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
    if (checkRobotReachedEnd(&robot, 0, 180, 10, 60)) { //Maze 5
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
        // re-generate dynamic wall
        // dynamicWallUpdate(renderer, dynamic_wall_head, 60, 1);
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
  int name_index = 1;
  insertAndSetFirstWall(head, name_index++, 0, 0, 10, 130);

  insertAndSetFirstWall(head, name_index++, 80, 0, 10, 30);

  insertAndSetFirstWall(head, name_index++, 80, 30, 30, 10);

  insertAndSetFirstWall(head, name_index++, 0, 130, 110, 10);

  int i, a, b, c, d, e, f, g, h, k, l, m;

  double j;

  a = 30;

  b = 30;

  c = 10;

  d = 3;

  e = 110;

  f = 130;

  g = b;

  h = c;

  k = d;

  l = e;

  m = 90;

  for (i = 0; i < m; i++) {
    j = i;

    insertAndSetFirstWall(head, name_index++,

                          (i * d) + e,

                          a + b * sin(c * j * M_PI / 180),

                          10, 10);

    insertAndSetFirstWall(head, name_index++,

                          (i * k) + l,

                          f + g * sin(h * j * M_PI / 180),

                          10, 10);
  }

  for (i = 0; i < 100; i++) {
    j = i;

    insertAndSetFirstWall(head, name_index++,

                          // the most important bit is below.

                          // increase the 20 for a tighter bend

                          // descrease for a more meandering flow

                          380 + 130 * sin(1.8 * j * M_PI / 180),

                          // increase the 5 for a spacier curve

                          (i * 2) + 130,

                          10, 10);

    insertAndSetFirstWall(head, name_index++,

                          // the most important bit is below.

                          // increase the 20 for a tighter bend

                          // descrease for a more meandering flow

                          380 + 230 * sin(1.8 * j * M_PI / 180),

                          // increase the 5 for a spacier curve

                          (i * 4) + 30,

                          10, 10);
  }

  float aa, bb;

  a = 200;

  aa = 0.5;

  bb = 1;

  c = 180;

  d = 1;

  e = a + 100;

  f = c;

  m = 250;

  for (i = 100; i < m; i++) {
    insertAndSetFirstWall(head, name_index++, a + i * aa, c + i * bb, 10, 10);
  }

  insertAndSetFirstWall(head, name_index++, 330, 430, 60, 10);

  m = 150;

  for (i = 0; i < m; i++) {
    insertAndSetFirstWall(head, name_index++, e + i * aa, f + i * bb, 10, 10);
  }

  insertAndSetFirstWall(head, name_index++, 150, 180, 150, 10);

  e = a - 50;

  aa = 0.25;

  for (i = 0; i < m; i++) {
    insertAndSetFirstWall(head, name_index++, e - i * aa, f + i * bb, 10, 10);
  }

  a = a + 50;

  c = c + 100;

  aa = 0.75;

  for (i = 0; i < m; i++) {
    insertAndSetFirstWall(head, name_index++, a - i * aa, c + i * bb, 10, 10);
  }

  insertAndSetFirstWall(head, name_index++, 40, 430, 100, 10);

  insertAndSetFirstWall(head, name_index++, 40, 240, 10, 190);

  insertAndSetFirstWall(head, name_index++, 110, 180, 10, 160);

  insertAndSetFirstWall(head, name_index++, 0, 240, 40, 10);

  insertAndSetFirstWall(head, name_index++, 0, 180, 110, 10);
}
