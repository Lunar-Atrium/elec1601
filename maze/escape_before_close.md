# ESCAPE BEFORE CLOSE

* base on complex maze
```c
// start：
    robot->x =  OVERALL_WINDOW_WIDTH/2 - 100;
    robot->y = OVERALL_WINDOW_HEIGHT - 20;
    robot->true_x = OVERALL_WINDOW_WIDTH / 2 - 100;
    robot->true_y = OVERALL_WINDOW_HEIGHT - 20;
    robot->width = ROBOT_WIDTH;
    robot->height = ROBOT_HEIGHT;
    robot->direction = 0;
    robot->angle = 0;

// end：
    robot->x =  OVERALL_WINDOW_WIDTH - 20;
    robot->y = OVERALL_WINDOW_HEIGHT/2-190;
    robot->true_x = OVERALL_WINDOW_WIDTH - 20;
    robot->true_y = OVERALL_WINDOW_HEIGHT/2-190;
    robot->width = ROBOT_WIDTH;
    robot->height = ROBOT_HEIGHT;
    robot->direction = 0;
    robot->angle = 270;

// maze setup:
    struct Wall_collection *static_wall_head = NULL;
    struct Wall_collection *dynamic_wall_head = NULL;
    struct Wall_collection *walls = NULL;
    my_custom_maze(&static_wall_head);
    create_wall(&dynamic_wall_head, -1, (int[2]){230, 200}, (int[2]){330, 200},
                (int[2]){330, 200}, 10, 0.05);

// custom maze
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
```
