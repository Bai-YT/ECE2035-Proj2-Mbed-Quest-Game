/* Gatech ECE2035 2017 FALL MAZE RUNNER
* Copyright (c) 2017 Gatech ECE2035
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

// Include header files for platform
#include "mbed.h"

// Include header files for pacman project
#include "globals.h"
#include "math_extra.h"
#include "physics.h"
#include "game.h"
#include "wall.h"
#include "doublely_linked_list.h"

// Hardware initialization
DigitalIn left_pb(p21);  // push button
DigitalIn right_pb(p22); // push button
DigitalIn up_pb(p23);    // push button
DigitalIn down_pb(p24);  // push button
uLCD_4DGL uLCD(p9,p10,p11); // LCD (serial tx, serial rx, reset pin;)
Serial pc(USBTX,USBRX);     // used by Accelerometer
MMA8452 acc(p28, p27, 100000); // Accelerometer
SDFileSystem sd(p5, p6, p7, p8, "sd"); // SD card and filesystem (mosi, miso, sck, cs)
PwmOut speaker(p25);    // Speaker

// Level creation method declaration
DLinkedList* create_blank_level1();
DLinkedList* create_blank_level2();
DLinkedList* create_blank_level3();
DLinkedList* create_blank_level4();

// Parameters. Declared in globals.h
const float mass = 0.001;
const int radius = 4;
const float bounce=0.5;

/** Main() is where you start your implementation */
int main()
{
    ////////////////////////////
    // Power-on initialization
    ////////////////////////////
    // Turn up the serial data rate so we don't lag
    uLCD.baudrate(3000000);
    pc.baud(115200);

    // Initialize the buttons
    // Each *_pb variable is 0 when pressed
    left_pb.mode(PullUp);
    right_pb.mode(PullUp);
    up_pb.mode(PullUp);
    down_pb.mode(PullUp);
    
    // Other hardware initialization here (SD card, speaker, etc.)

    ///////////////
    // Reset loop
    ///////////////
    // This is where control between major phases of the game happens
    // This is a good place to add choose levels, add the game menu, etc.
    
    // Create the Game menu
    uLCD.background_color(BLACK); 
    uLCD.text_string("Level 1",1,1,FONT_5X7,RED);
    uLCD.text_string("Level 2",1,3,FONT_5X7,RED);
    uLCD.text_string("Level 3",1,5,FONT_5X7,RED);
    uLCD.text_string("Level 4",1,7,FONT_5X7,RED);
    int Count = 0;
    // use up and down button to navigate the menu, and press left button to choose the level
    while(left_pb) {
        while(up_pb && down_pb && left_pb) {
            switch(Count){
                case 0:
                    uLCD.text_string("Level 1",1,1,FONT_5X7,GREEN);
                    break;
                case 1:
                    uLCD.text_string("Level 2",1,3,FONT_5X7,GREEN);
                    break;
                case 2:
                    uLCD.text_string("Level 3",1,5,FONT_5X7,GREEN);
                    break;
                case 3:
                    uLCD.text_string("Level 4",1,7,FONT_5X7,GREEN);
                    break;
            }
        }
        if(left_pb==0) {
            break;
        }
        if(up_pb == 0) {
            wait(0.2);
            Count ++;
            if(Count > 3) {
                Count = Count - 4;
            }
        }
        if(down_pb == 0) {
            wait(0.2);
            Count--;
            if(Count < 0) {
                Count = Count + 4;
            }
        }
        uLCD.text_string("Level 1",1,1,FONT_5X7,RED);
        uLCD.text_string("Level 2",1,3,FONT_5X7,RED);
        uLCD.text_string("Level 3",1,5,FONT_5X7,RED);
        uLCD.text_string("Level 4",1,7,FONT_5X7,RED);
        // the following code add sound effect to the menu
        speaker.period(1.0/500.0);
        speaker =0.02;
        wait(0.05);
        speaker=0.0;
        wait(0.05);
    }
    speaker.period(1.0/1000.0);
    speaker =0.02; 
    wait(0.05);
    speaker=0.0;
    wait(0.05);
    
    // create 4 different arena with different level of difficulty
    DLinkedList* arena1 = create_blank_level1();
    DLinkedList* arena2 = create_blank_level2();
    DLinkedList* arena3 = create_blank_level3();
    DLinkedList* arena4 = create_blank_level4();
    // create a Linked List containing 4 arenas
    DLinkedList* level = create_dlinkedlist();
    insertTail(level, (void*)arena1);
    insertTail(level, (void*)arena2);
    insertTail(level, (void*)arena3);
    insertTail(level, (void*)arena4);
    DLinkedList* arena;
    while(1) 
    {
        // this switch statement determine which level to start with base on the input from the user
        switch(Count) {
            case 0:
                arena = (DLinkedList*)getHead(level);
                break;
            case 1:
                arena = (DLinkedList*)getHead(level);
                arena = (DLinkedList*)getNext(level);
                break;
            case 2:
                arena = (DLinkedList*)getTail(level);
                arena = (DLinkedList*)getPrevious(level);
                break;
            case 3:
                arena = (DLinkedList*)getTail(level);
                break;
        }
        // create a Struct score, and set its inital value s_old and s to 0
        Score* score = (Score*) malloc(sizeof(Score));
        score->type = SCORE;
        score->s_old = 0;
        score->s = 0;
        // this do while loop itinerate through the arena linked list, so that when the previous level is finished, the next level will be the new arena until there's no more new level
        do {
        // add node score to the head of the current level's linked list
        insertHead(arena, (void*)score);
        // Initialze game state
        Physics state = {0};
        state.px = 120.0;        // Position in the center of the screen
        state.py = 120.0;
        state.vx = 0.0;         // Initially unmoving
        state.vy = 0.0;

        // Delegate to the game loop to execute the level
        // run_game() is in game.cpp
        run_game(arena, &state);
        speaker.period(1.0/500.0); 
        speaker =0.02; 
        wait(0.2);
        speaker.period(1.0/1000.0);
        wait(0.2);
        speaker.period(1.0/1500.0);
        wait(0.2);
        speaker=0.0;
        wait(0.05);
        } while(arena = (DLinkedList*)getNext(level));  //check if there's new level
        uLCD.cls(); // clear the uLCD
        uLCD.background_color(BLACK); // set the backgroud color to black
        while(1) {
            uLCD.text_string("Game Over",5,5,FONT_5X7,WHITE);   // display Game Over on the screen
        }
        // Destory the arena and entities once we're done with the level
        
        destroyList(arena);
    }
}

/** Creates a level with only bounding walls and a ball. */
DLinkedList* create_blank_level1()
{
    DLinkedList* arena = create_dlinkedlist();

    // Initialize the walls
    Wall* walls[5];
    walls[0] = create_wall(HORIZONTAL, 0, 0, 127, bounce);  // top
    walls[1] = create_wall(HORIZONTAL, 0, 127, 127, bounce);// bottom
    walls[2] = create_wall(VERTICAL, 0, 0, 127, bounce);    // left
    walls[3] = create_wall(VERTICAL, 127, 0, 127, bounce);  // right
    walls[4] = create_wall(HORIZONTAL, 0, 64, 127, bounce);// bottom
    // Add the walls to the arena
    for (int i = 0; i < 5; i++) {
        insertTail(arena, (void*)walls[i]);
    }
    Goal* goal = (Goal*) malloc(sizeof(Goal));
    goal->type = GOAL;
    goal->x = 32;
    goal->y = 32;
    insertTail(arena, (void*)goal);

    Pothole* pothole = (Pothole*) malloc(sizeof(Pothole));
    pothole->type = POTHOLE;
    pothole->x = 90;
    pothole->y = 90;
    insertTail(arena, (void*)pothole);
    
    Wormhole* wormhole1 = (Wormhole*) malloc(sizeof(Wormhole));
    wormhole1->type = WORMHOLE;
    wormhole1->index = 1;
    wormhole1->x = 5;
    wormhole1->y = 70;
    insertTail(arena, (void*)wormhole1);
    
    Wormhole* wormhole2 = (Wormhole*) malloc(sizeof(Wormhole));
    wormhole2->type = WORMHOLE;
    wormhole2->index = 2;
    wormhole2->x = 122;
    wormhole2->y = 58;
    insertTail(arena, (void*)wormhole2);
    // Initialize the ball
    Ball* ball = (Ball*) malloc(sizeof(Ball));
    ball->type = BALL;
    ball->x = 120;
    ball->y = 120;
    
    // Add ball to the arena 
    // NOTE: The ball should always be last in the arena list, so that the other 
    // ArenaElements have a chance to compute the Physics updates before the 
    // ball applies forward euler method.
    insertTail(arena, (void*)ball);

    
    return arena;
}

DLinkedList* create_blank_level2()
{
    DLinkedList* arena = create_dlinkedlist();

    // Initialize the walls
    Wall* walls[6];
    walls[0] = create_wall(HORIZONTAL, 0, 0, 127, bounce);  // top
    walls[1] = create_wall(HORIZONTAL, 0, 127, 127, bounce);// bottom
    walls[2] = create_wall(VERTICAL, 0, 0, 127, bounce);    // left
    walls[3] = create_wall(VERTICAL, 127, 0, 127, bounce);  // right
    walls[4] = create_wall(VERTICAL, 40, 0, 100, bounce);
    walls[5] = create_wall(VERTICAL, 80, 27, 100, bounce);
    // Add the walls to the arena
    for (int i = 0; i < 6; i++) {
        insertTail(arena, (void*)walls[i]);
    }
    Goal* goal = (Goal*) malloc(sizeof(Goal));
    goal->type = GOAL;
    goal->x = 10;
    goal->y = 10;
    insertTail(arena, (void*)goal);

    Pothole* pothole = (Pothole*) malloc(sizeof(Pothole));
    pothole->type = POTHOLE;
    pothole->x = 64;
    pothole->y = 64;
    insertTail(arena, (void*)pothole);
    // Initialize the ball
    Ball* ball = (Ball*) malloc(sizeof(Ball));
    ball->type = BALL;
    ball->x = 120;
    ball->y = 120;
    
    // Add ball to the arena 
    // NOTE: The ball should always be last in the arena list, so that the other 
    // ArenaElements have a chance to compute the Physics updates before the 
    // ball applies forward euler method.
    insertTail(arena, (void*)ball);


    
    return arena;
}

DLinkedList* create_blank_level4()
{
    DLinkedList* arena = create_dlinkedlist();

    // Initialize the walls
    Wall* walls[12];
    walls[0] = create_wall(HORIZONTAL, 0, 0, 127, bounce);  // top
    walls[1] = create_wall(HORIZONTAL, 0, 127, 127, bounce);// bottom
    walls[2] = create_wall(VERTICAL, 0, 0, 127, bounce);    // left
    walls[3] = create_wall(VERTICAL, 127, 0, 127, bounce);  // right
    walls[4] = create_wall(HORIZONTAL, 0, 25, 20, bounce);
    walls[5] = create_wall(HORIZONTAL, 0, 50, 45, bounce);
    walls[6] = create_wall(HORIZONTAL, 0, 75, 70, bounce);
    walls[7] = create_wall(HORIZONTAL, 0, 100, 95, bounce);
    walls[8] = create_wall(HORIZONTAL, 128-95, 25, 95, bounce);
    walls[9] = create_wall(HORIZONTAL, 128-70, 50, 70, bounce);
    walls[10] = create_wall(HORIZONTAL, 128-45, 75, 45, bounce);
    walls[11] = create_wall(HORIZONTAL, 128-20, 100, 20, bounce);
    // Add the walls to the arena
    for (int i = 0; i < 12; i++) {
        insertTail(arena, (void*)walls[i]);
    }
    Goal* goal = (Goal*) malloc(sizeof(Goal));
    goal->type = GOAL;
    goal->x = 10;
    goal->y = 10;
    insertTail(arena, (void*)goal);

    Pothole* pothole = (Pothole*) malloc(sizeof(Pothole));
    pothole->type = POTHOLE;
    pothole->x = 60;
    pothole->y = 60;
    insertTail(arena, (void*)pothole);
    // Initialize the ball
    Ball* ball = (Ball*) malloc(sizeof(Ball));
    ball->type = BALL;
    ball->x = 120;
    ball->y = 120;
    
    // Add ball to the arena 
    // NOTE: The ball should always be last in the arena list, so that the other 
    // ArenaElements have a chance to compute the Physics updates before the 
    // ball applies forward euler method.
    insertTail(arena, (void*)ball);


    
    return arena;
}

DLinkedList* create_blank_level3()
{
    DLinkedList* arena = create_dlinkedlist();

    // Initialize the walls
    Wall* walls[14];
    walls[0] = create_wall(HORIZONTAL, 0, 0, 127, bounce);  // top
    walls[1] = create_wall(HORIZONTAL, 0, 127, 127, bounce);// bottom
    walls[2] = create_wall(VERTICAL, 0, 0, 127, bounce);    // left
    walls[3] = create_wall(VERTICAL, 127, 0, 127, bounce);  // right
    walls[4] = create_wall(VERTICAL, 49, 49, 30, bounce);  
    walls[5] = create_wall(HORIZONTAL, 49, 49, 30, bounce);  
    walls[6] = create_wall(VERTICAL, 79, 49, 30, bounce);  
    walls[7] = create_wall(HORIZONTAL, 49, 79, 10, bounce);  
    walls[8] = create_wall(HORIZONTAL, 69, 79, 10, bounce);  
    walls[9] = create_wall(VERTICAL, 31, 31, 66, bounce);  
    walls[10] = create_wall(HORIZONTAL, 31, 97, 66, bounce);  
    walls[11] = create_wall(VERTICAL, 97, 31, 66, bounce);  
    walls[12] = create_wall(HORIZONTAL, 31, 31, 27, bounce);  
    walls[13] = create_wall(HORIZONTAL, 70, 31, 27, bounce);  
    

    // Add the walls to the arena
    for (int i = 0; i < 14; i++) {
        insertTail(arena, (void*)walls[i]);
    }
    Goal* goal = (Goal*) malloc(sizeof(Goal));
    goal->type = GOAL;
    goal->x = 64;
    goal->y = 64;
    insertTail(arena, (void*)goal);

    Pothole* pothole = (Pothole*) malloc(sizeof(Pothole));
    pothole->type = POTHOLE;
    pothole->x = 40;
    pothole->y = 40;
    insertTail(arena, (void*)pothole);
    // Initialize the ball
    Ball* ball = (Ball*) malloc(sizeof(Ball));
    ball->type = BALL;
    ball->x = 120;
    ball->y = 120;
    
    // Add ball to the arena 
    // NOTE: The ball should always be last in the arena list, so that the other 
    // ArenaElements have a chance to compute the Physics updates before the 
    // ball applies forward euler method.
    insertTail(arena, (void*)ball);


    
    return arena;
}

