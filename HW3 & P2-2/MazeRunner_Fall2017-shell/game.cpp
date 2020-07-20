#include "game.h"

#include "globals.h"
#include "physics.h"
#include "wall.h"


/** Erases the ball from the screen by drawing over it with the background color. */
void erase_ball(Ball* ball)
{
    uLCD.filled_circle(ball->x, ball->y, 4, BLACK);
    // TODO: Draw background color over curriously drawn ball location
}

/** Draws the ball on the screen at the updated location (according to the state) */
void draw_ball(Ball* ball, Physics* state)
{
    ball->x = state->px;
    ball->y = state->py;
    uLCD.filled_circle(ball->x, ball->y, 4, WHITE);
    // TODO: Save that updated ball position for later erasing
    // TODO: Draw ball in its updated location
}

// draw the pothole, pothole is red
void draw_pothole(Pothole* pothole) {
    uLCD.filled_circle(pothole->x, pothole->y, 5, RED);
}

void draw_wormhole(Wormhole* wormhole) {
    uLCD.filled_circle(wormhole->x, wormhole->y, 5, BLUE);
}
// draw the Goal, goal is green 
void draw_goal(Goal* goal) {
    uLCD.filled_circle(goal->x, goal->y, 5, GREEN);
}

/** Reads inputs to the game, such as accelerometer and buttons */
GameInputs read_inputs()
{
    GameInputs inputs = {0};
    //acc.activate();
    // read the  x,y,z acceleration from the acc
    double x = 0.0, y = 0.0, z = 0.0;
    int grav = acc.readXYZGravity(&x, &y, &z);
    inputs.ax = -150*x;
    inputs.ay = 150*y;
    inputs.az = 150*z;
    // TODO: Get acceleration vector from accelerometer
    // TODO: Read buttons, this one is updated in the update_game

    return inputs;
}

int update_game(DLinkedList* arena, Physics* curr, GameInputs inputs, float delta)
{
    ///////////////////////////////
    // Prepare for physics update
    ///////////////////////////////
    // Make a copy of the current state for modification
    Physics next = *curr;
    // No acceleration unless the ArenaElements apply them. (Newton's 1st law)
    next.ax = next.ay = 0.0;
    int error1, error2; // error is the difference between the position of the ball and the pothole
    
    // Loop over all arena elements
    ArenaElement* elem = (ArenaElement*)getHead(arena);
    do {
        // check if the up and down button are pressed at the same time, if so force to the next level
        if(up_pb == 0 && down_pb == 0) {
            return 1;
        }
        switch(elem->type) {
            case WALL:
                do_wall(&next, curr, (Wall*) elem, delta);
                break;
            case POTHOLE:
                error1 = (curr->px) - (((Pothole*) elem)->x);
                error2 = (curr->py) - (((Pothole*) elem)->y);
                // this check if the ball is in the pothole
                if ((error1 < 3 && error1 > -3) && (error2 < 3 && error2 > -3)) {
                    curr->px = 120.0;
                    curr->py = 120.0;
                    speaker.period(1.0/1000.0); 
                    speaker =0.02; 
                    wait(0.2);
                    speaker.period(1.0/600.0);
                    wait(0.2);
                    speaker.period(1.0/200.0);
                    wait(0.2);
                    speaker=0.0;
                    wait(0.05);
                    return 0;
                }; 
                break;
            case WORMHOLE:
                error1 = (curr->px) - (((Wormhole*) elem)->x);
                error2 = (curr->py) - (((Wormhole*) elem)->y);
                if ((error1 < 3 && error1 > -3) && (error2 < 3 && error2 > -3)) {
                    if (((Wormhole*) elem)->index == 1) {
                        curr->px = 122;
                        curr->py = 58;
                        return 0;
                    }
                }
                break;
            case GOAL:
                error1 = (curr->px) - (((Goal*) elem)->x);
                error2 = (curr->py) - (((Goal*) elem)->y);
                // this one check if the ball reach the goal
                if ((error1 < 3 && error1 > -3) && (error2 < 3 && error2 > -3)) {
                    return 1;
                }; 
                break;
            case BALL:
                // update next state using the game input
                next.ax = inputs.ax;
                next.ay = inputs.ay;
                forward_euler(&next, delta);
                break;
            default:
                break;
        };
    } while(elem = (ArenaElement*)getNext(arena));

    // Last thing! Update state, so it will be saved for the next iteration.
    *curr = next;
    
    // Zero means we aren't done yet
    return 0;
}

int run_game(DLinkedList* arena, Physics* state)
{
    // Initialize game loop timers
    int tick, phys_tick, draw_tick;
    Timer timer;
    timer.start();
    tick = timer.read_ms();
    phys_tick = tick;
    draw_tick = tick;
    Ball* ball = NULL;

    // Initialize debug counters
    int count = 0;
    int count2 = 0;

    // Initial draw of the game
    uLCD.background_color(BLACK);
    uLCD.cls();

    ///////////////////
    // Main game loop
    ///////////////////
    DLinkedList* savedState = create_dlinkedlist();
    DLinkedList* previousState = create_dlinkedlist();
    while(1) {
        
        // Read timer to determine how long the last loop took
        tick = timer.read_ms();
        
        ///////////////////
        // Physics Update
        ///////////////////
        // Rate limit: 1 ms
        int diff = tick - phys_tick;
        if (diff < 1) continue;
        phys_tick = tick;

        // Compute elapsed time in milliseconds
        float delta = diff*1e-3;

        // Read inputs
        GameInputs inputs = read_inputs();

        // Update game state
        int done = update_game(arena, state, inputs, delta);
        if (done) return done;
        // store
        if (right_pb == 0) {
            Physics* newstate = (Physics*) malloc(sizeof(Physics));
            newstate->px = state->px;
            newstate->py = state->py;
            insertTail(previousState, (void*)newstate);
            uLCD.text_string("saved",7,7,FONT_5X7,WHITE);
            wait(0.2);
            uLCD.text_string("saved",7,7,FONT_5X7,BLACK);
        }
        // restore
        if (left_pb == 0 && up_pb == 1) {
            state->px = ((Physics*)getTail(previousState))->px;
            state->py = ((Physics*)getTail(previousState))->py;
            uLCD.text_string("restored",7,7,FONT_5X7,WHITE);
            wait(0.2);
            uLCD.text_string("restored",7,7,FONT_5X7,BLACK);
        }
        // rewind
        if (up_pb == 0 && left_pb == 0) {
            state->px = ((Physics *) getHead(savedState))->px;
            state->py = ((Physics *)getHead(savedState))->py;
            Physics* temp = (Physics *) getTail(savedState);
            while(temp) {
                erase_ball(ball);
                draw_ball(ball, temp);
                temp = (Physics *) getPrevious(savedState);
                wait(0.1);
            }
            destroyList(savedState);
            savedState = create_dlinkedlist();
        }
        // Debug: Count physics updates
        count2++;

        //////////////////
        // Render update
        //////////////////
        // Rate limit: 40ms
        if(tick - draw_tick < 40) continue;
        draw_tick = tick;
        // this code save the current state and prepare to be rewind
        Physics* rewindstate = (Physics*) malloc(sizeof(Physics));
        rewindstate->px = state->px;
        rewindstate->py = state->py;
        insertTail(savedState, (void*)rewindstate);
         
        if(getSize(savedState) > 50) {
            getHead(savedState);
            deleteBackward(savedState);
        }
    
        // Erase moving stuff
        ArenaElement* elem = (ArenaElement*)getHead(arena);
        do {
            switch(elem->type) {
                case SCORE:
                    break;
                case BALL:
                    erase_ball((Ball*) elem);
                    break;
                default: break;
            }
        } while(elem = (ArenaElement*)getNext(arena));
        
        // Draw everything
        elem = (ArenaElement*)getHead(arena);
        do {
            switch(elem->type) {
                case SCORE:
                    // this block implement the display of the score
                    ((Score*)elem)->s = ((Score*)elem)->s + 1;
                    if (((Score*)elem)->s - ((Score*)elem)->s_old > 9) {
                        char a[20];
                        char b[20];
                        sprintf(a, "%d", ((Score*)elem)->s/40);
                        sprintf(b, "%d", ((Score*)elem)->s_old/40);
                        uLCD.text_string(b,15,0,FONT_5X7,BLACK);
                        uLCD.text_string(a,15,0,FONT_5X7,WHITE);
                        ((Score*)elem)->s_old = ((Score*)elem)->s;
                    }
                    //print_score((Score*) elem);
                    break;
                case WALL:
                    draw_wall((Wall*) elem);
                    break;
                case POTHOLE:
                    draw_pothole((Pothole*) elem);
                    break;
                case WORMHOLE:
                    draw_wormhole((Wormhole*) elem);
                    break;
                case GOAL:
                    draw_goal((Goal*) elem);
                    break;
                case BALL:
                    draw_ball((Ball*) elem, state);
                    ball = (Ball*) elem;
                    break;
                default:
                    break;
            }
        } while(elem = (ArenaElement*)getNext(arena));

        ///////////////
        // Debug info
        ///////////////
        // Displays rate info in the top corner
        //  First number is total time to update and render this frame
        //  Second number is how many physics iterations between drawing frames
        //  Only displayed every 10th render update (roughly 2.5 Hz)
        // TODO: Take this out before you turn your code in!
        //if ((count = (count+1)%10) == 0) {
            //uLCD.locate(0, 0);
            //uLCD.printf("%d %d \r\n", timer.read_ms()-tick, count2);
        //}
        
        // Reset physics iteration counter after every render update
        count2 = 0;
    }

}