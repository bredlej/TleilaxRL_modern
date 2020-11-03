//
// Created by geoco on 01.11.2020.
//
#include <../extern/C_StateMachine/StateMachine.h>

typedef struct Coordinates
{
    int x;
    int y;
    int z;
} Coordinates;

typedef struct TleilaxStateData
{
    Coordinates galaxy;
    Coordinates starsystem;
};

enum States
{
    ST_INTRO,
    ST_MENU,
    ST_GALAXY_VIEW,
    ST_STARSYSTEM_VIEW,
    ST_PLANET_VIEW,
    ST_SHIP_VIEW,
    ST_MAX_STATES
};

