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

int WALL_KEYS = 1;
int DONE = 0;
int BUFFER = 256;

void my_maze(struct Wall_collection **head);
void create_wall(struct Wall_collection **head, int start_point[2],
                 int end_point[2], int critical_point[2], int thick,
                 double precision);

int main(int argc, char *argv[]) {
  SDL_Window *window;
  SDL_Renderer *renderer;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

  window = SDL_CreateWindow("Robot Maze", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, OVERALL_WINDOW_WIDTH,
                            OVERALL_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
  renderer = SDL_CreateRenderer(window, -1, 0);

  struct Robot robot;
  struct Wall_collection *head = NULL;
  int front_centre_sensor, left_sensor, right_sensor = 0;
  clock_t start_time, end_time;
  int msec;
  int crashed = 0;

  // create a hashtable to track the coordinates
  CurrentCoordinate coordinate;
  PathHashTable *table;

  // SETUP MAZE
  // create_wall(&head, (int[2]){200, 480}, (int[2]){640, 0}, (int[2]){300, 0}, 10,
  //             0.005);
  // create_wall(&head, (int[2]){300, 480}, (int[2]){640, 100}, (int[2]){400, 0},
  //             10, 0.005);
  insertAndSetFirstWall(&head, 1,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2, 10, OVERALL_WINDOW_HEIGHT/2);
  insertAndSetFirstWall(&head, 2,  OVERALL_WINDOW_WIDTH/2-100, OVERALL_WINDOW_HEIGHT/2+100, 10, OVERALL_WINDOW_HEIGHT/2-100);
  insertAndSetFirstWall(&head, 3,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2+100, 150, 10);
  insertAndSetFirstWall(&head, 4,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2, 150, 10);
  insertAndSetFirstWall(&head, 5,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2-200, 10, 300);
  insertAndSetFirstWall(&head, 6,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2-100, 10, 100);
  insertAndSetFirstWall(&head, 7,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2-200, 450, 10);
  insertAndSetFirstWall(&head, 8,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2-100, 250, 10);
  insertAndSetFirstWall(&head, 9,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2-200, 10, 300);
  insertAndSetFirstWall(&head, 10,  OVERALL_WINDOW_WIDTH/2+100, OVERALL_WINDOW_HEIGHT/2-100, 10, 300);
  insertAndSetFirstWall(&head, 11,  OVERALL_WINDOW_WIDTH/2+100, OVERALL_WINDOW_HEIGHT/2+200, OVERALL_WINDOW_WIDTH/2-100, 10);
  insertAndSetFirstWall(&head, 12,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2+100, OVERALL_WINDOW_WIDTH/2-100, 10);

  // SETUP ROBOT
  setup_robot(&robot);

  // record initial coordinate
  coordinate.x = robot.x + ROBOT_WIDTH / 2;
  coordinate.y = robot.y + ROBOT_HEIGHT / 2;

  // setup hashtable
  table = createPathHashTable(BUFFER);
  int exist_coordinate;

  // setup path
  CurrentCoordinate paths[BUFFER];
  int paths_index = 0;

  SDL_Event event;
  while (!DONE) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderClear(renderer);

    // Move robot based on user input commands/auto commands
    if (robot.auto_mode == 1)
      robotAutoMotorMove(&robot, front_centre_sensor, left_sensor,
                         right_sensor, exist_coordinate);
    robotMotorMove(&robot, crashed);

    // Check if robot reaches endpoint. and check sensor values
    if (checkRobotReachedEnd(&robot, OVERALL_WINDOW_WIDTH,
                             OVERALL_WINDOW_HEIGHT / 2 + 100, 10, 100)) {
      end_time = clock();
      msec = (end_time - start_time) * 1000 / CLOCKS_PER_SEC;
      robotSuccess(&robot, msec);
    } else if (crashed == 1 || checkRobotHitWalls(&robot, head)) {
      robotCrash(&robot);
      crashed = 1;
    } else { // Otherwise compute sensor information
      front_centre_sensor = checkRobotSensorFrontCentreAllWalls(&robot, head);
      // if (front_centre_sensor > 0)
      //   printf("Getting close on the centre. Score = %d\n",
      //          front_centre_sensor);

      left_sensor = checkRobotSensorLeftAllWalls(&robot, head);
      // if (left_sensor > 0)
      //   printf("Getting close on the left. Score = %d\n", left_sensor);

      right_sensor = checkRobotSensorRightAllWalls(&robot, head);
      // if (right_sensor > 0)
      //   printf("Getting close on the right. Score = %d\n", right_sensor);
    }
    robotUpdate(renderer, &robot);
    updateAllWalls(head, renderer);

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

        // destory paths
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        paths_index = 0;
      }
      if (state[SDL_SCANCODE_RETURN]) {
        robot.auto_mode = 1;
        start_time = clock();
      }
    }

    // update coordinate
    if (coordinate.x != robot.x + ROBOT_WIDTH / 2 || coordinate.y != robot.y + ROBOT_HEIGHT / 2) {
      coordinate.x = robot.x + ROBOT_WIDTH / 2;
      coordinate.y = robot.y + ROBOT_HEIGHT / 2;
      coordinate.angle = robot.angle;
      exist_coordinate = existsCoordinate(table, coordinate.x, coordinate.y, coordinate.angle);
      if (!exist_coordinate) {

        // insert coordiante to paths
        paths[paths_index++] = coordinate;
        // set paths as red color
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 

      } else {

        // free memory and re-generate hashtable
        freeHashTable(table);
        table = createPathHashTable(256);
        // destroy paths(set paths as background color)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
        paths_index = 0;

      }
      printf("current coordinates: %d %d angle %d if have gone through: %d\n",
              coordinate.x, coordinate.y, coordinate.angle, exist_coordinate);
    }
    // render paths
    for (int i = 0; i < paths_index; i++)
          SDL_RenderDrawPoint(renderer, paths[i].x, paths[i].y);

    SDL_RenderPresent(renderer);
    SDL_Delay(120);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  printf("DEAD\n");
}

void create_wall(struct Wall_collection **head, int start_point[2],
                 int end_point[2], int critical_point[2], int thick,
                 double precision) {
  int record_pointBeforeCalc[2] = {start_point[0], start_point[1]};
  int record_pointAfterCalc[2] = {start_point[0], start_point[1]};
  for (double t = precision; t <= 1; t += precision) {
    record_pointAfterCalc[0] = (1 - t) * (1 - t) * start_point[0] +
                               2 * (1 - t) * t * critical_point[0] +
                               t * t * end_point[0];
    record_pointAfterCalc[1] = (1 - t) * (1 - t) * start_point[1] +
                               2 * (1 - t) * t * critical_point[1] +
                               t * t * end_point[1];
    insertAndSetFirstWall(head, WALL_KEYS, record_pointBeforeCalc[0],
                          record_pointBeforeCalc[1], thick, thick);
    record_pointBeforeCalc[0] = record_pointAfterCalc[0];
    record_pointBeforeCalc[1] = record_pointAfterCalc[1];
    WALL_KEYS++;
  }
}
