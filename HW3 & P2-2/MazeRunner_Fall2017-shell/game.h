#pragma once

#include "physics.h"
#include "doublely_linked_list.h"

/* A structure for holding all the game inputs */
struct GameInputs
{
    // Measurements from the accelerometer
    double ax, ay, az;
};

//////////////////////////
// Arena Element structs
//////////////////////////
/** The basic ArenaElement structure.
    Every item in an arena DLinkedList should be able to be cast to this struct.
    The type member is used to tell what other struct this element might be.
*/
struct ArenaElement {
    int type;
};

// Element types
#define WALL        0
#define BALL        1
#define POTHOLE     2
#define GOAL        3
#define SCORE       4
#define WORMHOLE    5

/** An ArenaElement struct representing the ball. */
struct Ball {
    // ArenaElement type (must be first element)
    int type;
    // Drawing info
    int x, y;
};

struct Pothole {
    int type;
    int x, y;
};
void draw_pothole(Pothole* pothole);

struct Goal {
    int type;
    int x, y;
};

void draw_Goal(Goal* goal);

struct Score {
    int type;
    int s;
    int s_old;
};

struct Wormhole {
    int type;
    int index;
    int x, y;
};

void draw_wormhole(Wormhole* wormhole);

/////////////////////////
// ArenaElement helpers
/////////////////////////
/** Erases the ball */
void erase_ball(Ball* ball);
/** Draws the ball at the current state */
void draw_ball(Ball* ball, Physics* state);

/* Add additional helpers for other ArenaElement types here */

///////////////////////////
// Game control functions 
///////////////////////////
/* Reads all game inputs */
GameInputs read_inputs();

/* Performs a single physics update. */
int update_game(DLinkedList* arena, Physics* prev, GameInputs inputs, float delta);

/* Implements the game loop */
int run_game(DLinkedList* arena, Physics* state);
