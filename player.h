#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

struct Map;

// struct Player has:
// double x -         x position of the player
// double y -         y position of the player
// double movespeed - how far the player moves per cycle
// double angle -     which direction the player is facing, clockwise from +x
// double turnspeed - how much the player turns per cycle
struct Player { 
    double x;
    double y;
    double movespeed;
    double angle;
    double turnspeed;
};

// Takes: struct Player*, struct Map*
// Moves player based on keyboard state, and checks collision with map
void Player_move(struct Player*, struct Map* );

#endif