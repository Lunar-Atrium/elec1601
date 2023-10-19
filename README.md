# USYD ELEC 1601 ROBOT MAZE SIMULATION

team memebr:

* Shaopeng Luan(Program Develop)
* Liosa Long(Robot Algorithm)
* Zixin Zhou(PPT)
* Sean Kim(Video)

## ACHIEVEMENT

![final](docs/final.gif)

## Developing Cooperation

We use git to do teamwork. Git is a common used cross-platform distributed version control system and the code is hosted in github.

## Program Construction (contributor: Shaopeng Luan)

For cross-platform and teamwork, we use CMake to contrusct our program thus can run or debugging in any OS high-efficiency.

CMake has a bunch of nice features, such as good supporting for all the C and C++ complier and good integrate with VScode.

## Improvement in time record (contributor: Shaopeng Luan)

the time record porvided by the original code template is not precise enough, I declared the variable and call `time()` from `time.h` to get linux time stamp for right time display.

## Improvement in terminal output (contributor: Shaopeng Luan)

I have improved the terminal output for better information release, it deepends on path track function which can allow us real-time analysis of the robot.

![terminal_output](docs/terminal_output.png)

## New Feature: Curve Wall (contributor: Shaopeng Luan)

the original function to build walls can only be the form of rectangle. I have adopted the Bezier Curve that is avaliable to build any shape of the wall.

```c
int create_wall(struct Wall_collection **head, int current_wall_keys,
                int start_point[2], int end_point[2], int critical_point[2],
                int thick, double precision);
```

* it accept three basic parameters: start point and end point in pixel, thick.
* critical point is used to build curve, when you specify this variable outside the line of formed by the starting and eneding, it will make this curve as close as to the stationary point for the ideal curve.
* precision, the precision of this curve should be specifyed between 0.001 to 0.01 for reasonable shape, it should be noted that the higher precision you specifed, the more effieciency consumption you need.

## New Feature: Dynamic Wall (contributor: Shaopeng Luan)

```c
void update_dynamic_wall(SDL_Renderer *renderer, struct Wall_collection *head,
                         int time， int reset)
```

![dynamic_wall](docs/dynamic_wall.gif)

* for building this wall, you need to create a collection of walls in the form of a linked chart provided by the template.
* the `time` parameter assgined the generate time of one node in the dynamic wall linked list
* It cannot draw multiple walls at the same time that need to innovate.

## New Feature: path track (contributor: Shaopeng Luan)

![path_track](docs/path_track.gif)

I wrote the path tracking function to observe the path of the robot to better optimize the algorithm.

here is the implement:

### linked list

Linked lists are commonly used data structures for dynamically expandable arrays which allowss us to record the path in a maze large enough.

```c
typedef struct PathTrackNode {
    CurrentCoordinate coord;
    struct PathTrackNode* next;
} PathTrackNode;
```

### hash

The hash function can be used to travese the current linked list faster. I use hash to replace the index of arrays not only for tracking paths, but also for faster analiysis for known paths.

 ```c
int hash(PathHashTable* table, int x, int y) {
  return (31 *  (unsigned int)x + y) % table->size;
}
 ```

### Reserved design: existsCoordinate()

Even though it has been abandoned in the actual alogrithim, I think it still has value. Just beacause I am using hash alorigthm, analyzing it will become easy.

Notice the path in red color and terminal output "if have gone through: "

![existCoordinate](docs/existCoordinate.gif)

If the robot has gone through the same coordinates, the function will return 1 and paths will be empty.

```c
int COORDINATE_OFFSET = 10;
int OVERLAP_COUNT = 1;
int existsCoordinate(PathHashTable* table, int x, int y);
```

It will accept two global variables. COORDIANTE_OFFSET can provide fuzzy matching means when the robot walks a `COORDINATE OFFSET` pixel around the path, it returns 1. `OVERLAP_COUNT` provides when the robot encounters an OVERLAP COUNT of pixels, it returns 1.

### how we use this function to improve our algorithm

When we can observe the path, improving our algorithm becomes convenient and simple.

* When we encounter a sharp corner, the path of the robot is as follows:

  ![sharp_corner](docs/sharp_corner.gif)

* When we encounter right and obtuse angles, the path of the robot is as follows:

  ![obtuse_corner](docs/obtuse_corner.gif)

* When we encounter long straights and gentle curves, the path of the robot is as follows:

  ![long_stragiht](docs/long_straights.gif)

It is clear to see that the robot is accelerating.

## Robot Algorithm (contributor: Liosa Long)

![robot_algorithim_page_1](docs/robot_algorithim/page_1.PNG)
![robot_algorithim_page_2](docs/robot_algorithim/page_2.PNG)
![robot_algorithim_page_3](docs/robot_algorithim/page_3.PNG)
![robot_algorithim_page_4](docs/robot_algorithim/page_4.PNG)
![robot_algorithim_page_5](docs/robot_algorithim/page_5.PNG)
![robot_algorithim_page_6](docs/robot_algorithim/page_6.PNG)
![robot_algorithim_page_7](docs/robot_algorithim/page_7.PNG)
![robot_algorithim_page_8](docs/robot_algorithim/page_8.PNG)
![robot_algorithim_page_9](docs/robot_algorithim/page_9.PNG)
