#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "formulas.h"
#include "robot.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "wall.h"

int WALL_KEYS = 1;
int DONE = 0;
void my_maze(struct Wall_collection **head);
void create_wall(struct Wall_collection **head, int start_point[2],
                 int end_point[2], int critical_point[2], int thick,
                 double precision);
int MAP_DOWN[2] = {OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT};
int MAP_RIGHT[2] = {OVERALL_WINDOW_WIDTH - 10, OVERALL_WINDOW_HEIGHT/2};
int MAP_UP[2] = {OVERALL_WINDOW_WIDTH/2, 0};
int MAP_LEFT[2] = {0, OVERALL_WINDOW_HEIGHT/2};
int MAP_LEFT_UP[2] = {0, 0};
int MAP_LEFT_DOWN[2] = {0, OVERALL_WINDOW_HEIGHT};
int MAP_RIGHT_UP[2] = {OVERALL_WINDOW_WIDTH, 0};
int MAP_RIGHT_DOWN[2] = {OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT};

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

  //basic
  insertAndSetFirstWall(&head, 1,  OVERALL_WINDOW_WIDTH/2-50, OVERALL_WINDOW_HEIGHT/2 + 50, 10, OVERALL_WINDOW_HEIGHT/2 -50);
  insertAndSetFirstWall(&head, 2,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2+150, 10, OVERALL_WINDOW_HEIGHT/2-150);
  insertAndSetFirstWall(&head, 3,  OVERALL_WINDOW_WIDTH/2-300, OVERALL_WINDOW_HEIGHT/2+150, 150, 10);
  insertAndSetFirstWall(&head, 4,  OVERALL_WINDOW_WIDTH/2-200, OVERALL_WINDOW_HEIGHT/2 + 50, 150, 10);
  insertAndSetFirstWall(&head, 5,  OVERALL_WINDOW_WIDTH/2-300, OVERALL_WINDOW_HEIGHT/2-200, 10, 350);
  insertAndSetFirstWall(&head, 6,  OVERALL_WINDOW_WIDTH/2-200, OVERALL_WINDOW_HEIGHT/2-100, 10, 150);
  insertAndSetFirstWall(&head, 7,  OVERALL_WINDOW_WIDTH/2-300, OVERALL_WINDOW_HEIGHT/2-200, 275, 10);
  insertAndSetFirstWall(&head, 8,  OVERALL_WINDOW_WIDTH/2-200, OVERALL_WINDOW_HEIGHT/2-100, 75, 10);
  insertAndSetFirstWall(&head, 9,  OVERALL_WINDOW_WIDTH/2+300, OVERALL_WINDOW_HEIGHT/2-200, 10, 300);
  insertAndSetFirstWall(&head, 10,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2-100, 10, 300);
  insertAndSetFirstWall(&head, 11,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2+200, OVERALL_WINDOW_WIDTH/2-200, 10);
  insertAndSetFirstWall(&head, 12,  OVERALL_WINDOW_WIDTH/2+300, OVERALL_WINDOW_HEIGHT/2+100, OVERALL_WINDOW_WIDTH/2-100, 10);
  insertAndSetFirstWall(&head, 13,  OVERALL_WINDOW_WIDTH/2 + 85, OVERALL_WINDOW_HEIGHT/2-200, 215, 10);
  insertAndSetFirstWall(&head, 14,  OVERALL_WINDOW_WIDTH/2 + 185, OVERALL_WINDOW_HEIGHT/2-100, 15, 10);
  insertAndSetFirstWall(&head, 15,  OVERALL_WINDOW_WIDTH/2-125, OVERALL_WINDOW_HEIGHT/2-100, 10, 100);
  insertAndSetFirstWall(&head, 16,  OVERALL_WINDOW_WIDTH/2-25, OVERALL_WINDOW_HEIGHT/2-200, 10, 100);
  insertAndSetFirstWall(&head, 17,  OVERALL_WINDOW_WIDTH/2 - 25, OVERALL_WINDOW_HEIGHT/2-100, 115, 10);
  insertAndSetFirstWall(&head, 18,  OVERALL_WINDOW_WIDTH/2 - 125, OVERALL_WINDOW_HEIGHT/2, 315, 10);
  insertAndSetFirstWall(&head, 19,  OVERALL_WINDOW_WIDTH/2 + 85, OVERALL_WINDOW_HEIGHT/2-200, 10, 110);
  insertAndSetFirstWall(&head, 20,  OVERALL_WINDOW_WIDTH/2 + 185, OVERALL_WINDOW_HEIGHT/2-100, 10, 110);

  //complex
  // insertAndSetFirstWall(&head, 1,  OVERALL_WINDOW_WIDTH/2 - 50, OVERALL_WINDOW_HEIGHT/2 + 200, 10, OVERALL_WINDOW_HEIGHT/2 - 200);
  // insertAndSetFirstWall(&head, 2,  OVERALL_WINDOW_WIDTH/2 - 150, OVERALL_WINDOW_HEIGHT/2+ 200, 10, OVERALL_WINDOW_HEIGHT/2 - 200);
  // create_wall(&head, (int[2]){OVERALL_WINDOW_WIDTH/2-200, OVERALL_WINDOW_HEIGHT/2 + 100}, (int[2]){OVERALL_WINDOW_WIDTH/2 - 50, OVERALL_WINDOW_HEIGHT/2 + 200}, (int[2]){OVERALL_WINDOW_WIDTH/2-200, OVERALL_WINDOW_HEIGHT/2 + 100},
  //  8, 0.005);
  // create_wall(&head, (int[2]){OVERALL_WINDOW_WIDTH/2-300, OVERALL_WINDOW_HEIGHT/2 + 100}, (int[2]){OVERALL_WINDOW_WIDTH/2 - 150, OVERALL_WINDOW_HEIGHT/2 + 200}, (int[2]){OVERALL_WINDOW_WIDTH/2-300, OVERALL_WINDOW_HEIGHT/2 + 100},
  //  8, 0.005);
  // insertAndSetFirstWall(&head, 3,  OVERALL_WINDOW_WIDTH/2-200, OVERALL_WINDOW_HEIGHT/2 + 50, 10, 50);  
  // insertAndSetFirstWall(&head, 4,  OVERALL_WINDOW_WIDTH/2-300, OVERALL_WINDOW_HEIGHT/2 + 50, 10, 50);
  
  // insertAndSetFirstWall(&head, 5,  OVERALL_WINDOW_WIDTH/2-300, OVERALL_WINDOW_HEIGHT/2-220, 10, 175);
  // insertAndSetFirstWall(&head, 6,  OVERALL_WINDOW_WIDTH/2-230, OVERALL_WINDOW_HEIGHT/2-160, 10, 25);
  // create_wall(&head, (int[2]){OVERALL_WINDOW_WIDTH/2-200, OVERALL_WINDOW_HEIGHT/2 + 50}, (int[2]){OVERALL_WINDOW_WIDTH/2 - 80, OVERALL_WINDOW_HEIGHT/2 - 135}, (int[2]){OVERALL_WINDOW_WIDTH/2-200, OVERALL_WINDOW_HEIGHT/2 + 50},
  //  8, 0.005);
  // create_wall(&head, (int[2]){OVERALL_WINDOW_WIDTH/2-230, OVERALL_WINDOW_HEIGHT/2 - 45}, (int[2]){OVERALL_WINDOW_WIDTH/2-300, OVERALL_WINDOW_HEIGHT/2 + 50}, (int[2]){OVERALL_WINDOW_WIDTH/2-230, OVERALL_WINDOW_HEIGHT/2 - 45},
  //  8, 0.005);
  // insertAndSetFirstWall(&head, 7,  OVERALL_WINDOW_WIDTH/2-300, OVERALL_WINDOW_HEIGHT/2 - 45, 70, 10);
  // insertAndSetFirstWall(&head, 8,  OVERALL_WINDOW_WIDTH/2-230, OVERALL_WINDOW_HEIGHT/2 - 135, 150, 10);
  
  // insertAndSetFirstWall(&head, 9,  OVERALL_WINDOW_WIDTH/2-230, OVERALL_WINDOW_HEIGHT/2-160, 180, 10);
  // insertAndSetFirstWall(&head, 10,  OVERALL_WINDOW_WIDTH/2-300, OVERALL_WINDOW_HEIGHT/2-220, 330, 10);
  
  // insertAndSetFirstWall(&head, 11,  OVERALL_WINDOW_WIDTH/2 + 30, OVERALL_WINDOW_HEIGHT/2-220, 10, 60);
  
  // insertAndSetFirstWall(&head, 12,  OVERALL_WINDOW_WIDTH/2+240, OVERALL_WINDOW_HEIGHT/2-160, OVERALL_WINDOW_WIDTH/2-240, 10);
  // insertAndSetFirstWall(&head, 13,  OVERALL_WINDOW_WIDTH/2+100, OVERALL_WINDOW_HEIGHT/2-220, OVERALL_WINDOW_WIDTH/2-100, 10);
  
  // insertAndSetFirstWall(&head, 14,  OVERALL_WINDOW_WIDTH/2+100, OVERALL_WINDOW_HEIGHT/2-220, 10, 175);
  // insertAndSetFirstWall(&head, 15,  OVERALL_WINDOW_WIDTH/2+240, OVERALL_WINDOW_HEIGHT/2-160, 10, 25);

  // insertAndSetFirstWall(&head, 16,  OVERALL_WINDOW_WIDTH/2+100, OVERALL_WINDOW_HEIGHT/2-45, 50, 10);
  // insertAndSetFirstWall(&head, 17,  OVERALL_WINDOW_WIDTH/2+240, OVERALL_WINDOW_HEIGHT/2-135, 70, 10);

  // insertAndSetFirstWall(&head, 18,  OVERALL_WINDOW_WIDTH/2+150, OVERALL_WINDOW_HEIGHT/2-45, 10, 140);
  // insertAndSetFirstWall(&head, 19,  OVERALL_WINDOW_WIDTH/2+310, OVERALL_WINDOW_HEIGHT/2-135, 10, 350);

  // insertAndSetFirstWall(&head, 20, OVERALL_WINDOW_WIDTH/2 + 60, OVERALL_WINDOW_HEIGHT/2+95, 100, 10);
  // insertAndSetFirstWall(&head, 21,  OVERALL_WINDOW_WIDTH/2-10, OVERALL_WINDOW_HEIGHT/2+215, 330, 10);

  // create_wall(&head, (int[2]){OVERALL_WINDOW_WIDTH/2 + 30, OVERALL_WINDOW_HEIGHT/2-160}, (int[2]){OVERALL_WINDOW_WIDTH/2 + 50, OVERALL_WINDOW_HEIGHT/2-85}, (int[2]){OVERALL_WINDOW_WIDTH/2 + 110, OVERALL_WINDOW_HEIGHT/2 - 120},
  //  8, 0.005); //c1
  // create_wall(&head, (int[2]){OVERALL_WINDOW_WIDTH/2 + 70, OVERALL_WINDOW_HEIGHT/2}, (int[2]){OVERALL_WINDOW_WIDTH/2 + 60, OVERALL_WINDOW_HEIGHT/2+95}, (int[2]){OVERALL_WINDOW_WIDTH/2 + 130, OVERALL_WINDOW_HEIGHT/2 + 50},
  //  8, 0.005); //c3
  // create_wall(&head, (int[2]){OVERALL_WINDOW_WIDTH/2 + 50, OVERALL_WINDOW_HEIGHT/2-85}, (int[2]){OVERALL_WINDOW_WIDTH/2 + 70, OVERALL_WINDOW_HEIGHT/2}, (int[2]){OVERALL_WINDOW_WIDTH/2 - 20, OVERALL_WINDOW_HEIGHT/2 - 25},
  //  8, 0.005); //c5

  // create_wall(&head, (int[2]){OVERALL_WINDOW_WIDTH/2-50, OVERALL_WINDOW_HEIGHT/2-160}, (int[2]){OVERALL_WINDOW_WIDTH/2 - 40, OVERALL_WINDOW_HEIGHT/2 - 85}, (int[2]){OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2 -130},
  //  8, 0.005); //c2  
  // create_wall(&head, (int[2]){OVERALL_WINDOW_WIDTH/2 - 20, OVERALL_WINDOW_HEIGHT/2 + 20}, (int[2]){OVERALL_WINDOW_WIDTH/2 - 60, OVERALL_WINDOW_HEIGHT/2 + 125}, (int[2]){OVERALL_WINDOW_WIDTH/2 + 30, OVERALL_WINDOW_HEIGHT/2+50},
  //  8, 0.005); //c4
  // create_wall(&head, (int[2]){OVERALL_WINDOW_WIDTH/2 - 40, OVERALL_WINDOW_HEIGHT/2 - 85}, (int[2]){OVERALL_WINDOW_WIDTH/2 - 20, OVERALL_WINDOW_HEIGHT/2 + 20}, (int[2]){OVERALL_WINDOW_WIDTH/2 - 130, OVERALL_WINDOW_HEIGHT/2 - 25},
  //  8, 0.005); //c6

  // create_wall(&head, (int[2]){OVERALL_WINDOW_WIDTH/2 - 60, OVERALL_WINDOW_HEIGHT/2 + 125}, (int[2]){ OVERALL_WINDOW_WIDTH/2-10, OVERALL_WINDOW_HEIGHT/2+215}, (int[2]){ OVERALL_WINDOW_WIDTH/2 - 100, OVERALL_WINDOW_HEIGHT/2+155},
  //  8, 0.005); //c7


  //Dead end: 
  // insertAndSetFirstWall(&head, 1,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2, 10, OVERALL_WINDOW_HEIGHT/2);
  // insertAndSetFirstWall(&head, 2,  OVERALL_WINDOW_WIDTH/2-100, OVERALL_WINDOW_HEIGHT/2, 10, OVERALL_WINDOW_HEIGHT/2);
  // insertAndSetFirstWall(&head, 3,  OVERALL_WINDOW_WIDTH/2-100, OVERALL_WINDOW_HEIGHT/2, 100, 10);

  // T-intersection:
  // insertAndSetFirstWall(&head, 1,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2 + 100, 10, OVERALL_WINDOW_HEIGHT/2 - 100);
  // insertAndSetFirstWall(&head, 2,  OVERALL_WINDOW_WIDTH/2-100, OVERALL_WINDOW_HEIGHT/2+100, 10, OVERALL_WINDOW_HEIGHT/2-100);
  // insertAndSetFirstWall(&head, 3,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2+100, 150, 10);
  // insertAndSetFirstWall(&head, 4,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2, 400, 10);
  // insertAndSetFirstWall(&head, 5,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2+100, 150, 10);

  // side branch:
  // insertAndSetFirstWall(&head, 1,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2 + 150, 10, OVERALL_WINDOW_HEIGHT/2 -150);
  // insertAndSetFirstWall(&head, 2,  OVERALL_WINDOW_WIDTH/2-100, OVERALL_WINDOW_HEIGHT/2, 10, OVERALL_WINDOW_HEIGHT/2);
  // insertAndSetFirstWall(&head, 3,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2 + 150, 100, 10);
  // insertAndSetFirstWall(&head, 4,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2 + 50, 100, 10);
  // insertAndSetFirstWall(&head, 5,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2, 10, OVERALL_WINDOW_HEIGHT/2 - 180);

  setup_robot(&robot);
  updateAllWalls(head, renderer);

  SDL_Event event;
  while (!DONE) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Move robot based on user input commands/auto commands
    if (robot.auto_mode == 1)
      robotAutoMotorMove(&robot, front_centre_sensor, left_sensor,
                         right_sensor);
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
    }
    // Otherwise compute sensor information
    else {
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
      printf("Front = %d, right = %d, left = %d\n", front_centre_sensor, right_sensor, left_sensor);
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
      }
      if (state[SDL_SCANCODE_RETURN]) {
        robot.auto_mode = 1;
        start_time = clock();
      }
    }

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