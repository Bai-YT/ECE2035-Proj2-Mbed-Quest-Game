#include "physics.h"
#include "math_extra.h"

void forward_euler(Physics* state, float delta)
{ 
    // TODO: Implement proper forward euler updates for position and velocity
    

    // Compute random motion
    float damp = 0.5;
    float dx = damp*coin_flip();
    float dy = damp*coin_flip();

    // Update position and velocity
    dx = state->vx * delta;
    dy = state->vy * delta;
    state->px = state->px + dx;
    state->py = state->py + dy;
    dx = state->ax * delta;
    dy = state->ay * delta;
    state->vx = state->vx + dx;
    state->vy = state->vy + dy;
}