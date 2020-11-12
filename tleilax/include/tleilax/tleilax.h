#ifndef MYPROJECT_H_
#define MYPROJECT_H_
#define VERSION 0.1
#include <../extern/C_StateMachine/StateMachine.h>
#include <galaxy/galaxy.h>
#include <random/random.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
};
extern struct Tleilax Tleilax;

typedef struct Coordinates {
  int x;
  int y;
  int z;
} Coordinates;

typedef struct TleilaxUI {
  void (*Render)(void);
  void (*Update)(void);
  void (*HandleInput)(void);
} TleilaxUI;

typedef struct TleilaxUIData {
  void (*Render)(void);
  void (*Update)(void);
  void (*HandleInput)(void);
} TleilaxUIData;

/* Prepare initial state of Tleilax State Machine */
static TleilaxUI tleilaxUI;

/* Define Tleilax State Machine */
static SM_DEFINE(TleilaxUISM, &tleilaxUI);

enum States { ST_INTRO, ST_GALAXY_VIEW, ST_MAX_STATES };

EVENT_DECLARE(TLX_ShowGalaxy, TleilaxUIData)
EVENT_DECLARE(TLX_ShowIntro, TleilaxUIData)

STATE_DECLARE(Intro, TleilaxUIData)
STATE_DECLARE(GalaxyView, TleilaxUIData)

BEGIN_STATE_MAP(TleilaxUI)
STATE_MAP_ENTRY(ST_Intro)
STATE_MAP_ENTRY(ST_GalaxyView)
END_STATE_MAP(TleilaxUI)

#endif