#ifndef MYPROJECT_H_
#define MYPROJECT_H_
#define VERSION 0.1
#include <../extern/C_StateMachine/StateMachine.h>
#include <galaxy/galaxy.h>
#include <random/random.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define AMOUNT_SECTORS_X 50
#define AMOUNT_SECTORS_Y 50
#define AMOUNT_SECTORS_Z 50

void print_version(void);
int my_library_function(void);

enum STAR_NAME_CLASS { GREEK = 0, INDIAN = 1 };
struct Star *StarAt(uint32_t x, uint32_t y, uint32_t z);

struct TleilaxConfig {
  float zoomSpeed;
  float scrollSpeed;
};

extern struct TleilaxConfig TleilaxConfig;

struct Tleilax {
  void (*Initialize)(void);
  void (*Destroy)(void);
  struct TleilaxConfig config;
  struct StarNames **starNames;
  Coordinates *selectedCoordinates;
  struct Star *selectedStar;
};
extern struct Tleilax Tleilax;

typedef struct TleilaxState {
  StarSystem *starSystem;
} TleilaxState;

typedef struct TleilaxStarSystemData {
  StarSystem *starSystem;
} StarSystemData;

/* Prepare initial state of Tleilax State Machine */
static TleilaxState tleilaxState;

/* Define Tleilax State Machine */
static SM_DEFINE(TleilaxStateMachine, &tleilaxState);

enum States { ST_INTRO, ST_GALAXY_VIEW, ST_STARSYSTEM_VIEW, ST_MAX_STATES };

EVENT_DECLARE(TLX_ShowGalaxy, NoEventData)
EVENT_DECLARE(TLX_ShowIntro, NoEventData)
EVENT_DECLARE(TLX_ShowStarSystem, StarSystemData)

STATE_DECLARE(Intro, NoEventData)
STATE_DECLARE(GalaxyView, NoEventData)
STATE_DECLARE(StarSystemView, NoEventData)
ENTRY_DECLARE(StarSystemView, StarSystemData)
GUARD_DECLARE(StarSystemView, NoEventData)
EXIT_DECLARE(StarSystemView)

BEGIN_STATE_MAP_EX(TleilaxState)
  STATE_MAP_ENTRY_EX(ST_Intro)
  STATE_MAP_ENTRY_EX(ST_GalaxyView)
  STATE_MAP_ENTRY_ALL_EX(ST_StarSystemView, GD_StarSystemView, EN_StarSystemView, EX_StarSystemView)
END_STATE_MAP_EX(TleilaxState)

#endif