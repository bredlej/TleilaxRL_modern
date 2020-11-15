#include <tleilax/tleilax.h>

void print_version(void) { printf("Tleilax (version %0.2f)\n", VERSION); }

/*
        Function used for initial unit test of this library.
        See ../tests/test_mylibrary.c"
*/
int my_library_function(void) { return 1; }

struct Star *StarAt(uint32_t x, uint32_t y, uint32_t z) {
  Random.RandomizeSeedXYZ(x, y, z);
  uint32_t starOccurenceChance = Random.RndIntRange(0, 2000);
  if (starOccurenceChance == 1) {
    enum STAR_TYPE starType;
    uint32_t randomType = Random.RndIntRange(0, 100);
    if (randomType < 10)
      starType = STAR_YELLOW;
    else if (randomType >= 10 && randomType < 50)
      starType = STAR_RED;
    else if (randomType >= 50 && randomType < 90) {
      starType = STAR_BLUE;
    } else {
      starType = STAR_BROWN;
    }
    char *starName;
    if (Random.RndIntRange(0, 10) < 5) {
      starName =
          Tleilax.starNames[GREEK]
              ->names[Random.RndIntRange(0, Tleilax.starNames[GREEK]->size)];
    } else {
      starName =
          Tleilax.starNames[INDIAN]
              ->names[Random.RndIntRange(0, Tleilax.starNames[INDIAN]->size)];
    }
    return Galaxy.CreateStar(starName, (float)Random.RndDoubleRange(0.1f, 0.9f),
                             starType);
  } else
    return NULL;
}

void init() {
  struct TleilaxConfig tleilaxConfig = {.zoomSpeed = 2.0f, .scrollSpeed = 1.0f};
  Tleilax.config = tleilaxConfig;
  Tleilax.starNames = malloc(sizeof(struct StarNames *) * 2);
  if (Tleilax.starNames) {
    Tleilax.starNames[GREEK] = LoadNames("scripts/lua/old/names/greek");
    Tleilax.starNames[INDIAN] = LoadNames("scripts/lua/old/names/indian");
  }
  Galaxy.offset.x = 0.0f;
  Galaxy.offset.y = 0.0f;
  Galaxy.offset.z = 0.0f;
}

void destroy() {
  free(Tleilax.starNames[GREEK]);
  free(Tleilax.starNames[INDIAN]);
  free(Tleilax.starNames);
}

struct Tleilax Tleilax = {.Initialize = init, .Destroy = destroy};

EVENT_DEFINE(TLX_ShowGalaxy, NoEventData){
    BEGIN_TRANSITION_MAP TRANSITION_MAP_ENTRY(ST_GALAXY_VIEW) // ST_Intro
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)                       // ST_GalaxyView
    TRANSITION_MAP_ENTRY(ST_GALAXY_VIEW) // ST_StarSystemView
    END_TRANSITION_MAP(TleilaxState, NULL)}

EVENT_DEFINE(TLX_ShowIntro, NoEventData){
    BEGIN_TRANSITION_MAP TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_Intro
    TRANSITION_MAP_ENTRY(ST_INTRO)                           // ST_GalaxyView
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_StarSystemView
    END_TRANSITION_MAP(TleilaxState, NULL)}

EVENT_DEFINE(TLX_ShowStarSystem, StarSystemData){
    BEGIN_TRANSITION_MAP TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_Intro
    TRANSITION_MAP_ENTRY(ST_STARSYSTEM_VIEW)                 // ST_GalaxyView
    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_StarSystemView
    END_TRANSITION_MAP(TleilaxState, pEventData)}

STATE_DEFINE(Intro, NoEventData) {
  TleilaxState *tleilaxUi = SM_GetInstance(TleilaxState);
  tleilaxUi->starSystem = NULL;
  printf("Running Intro state.\n");
}

STATE_DEFINE(GalaxyView, NoEventData) {
  TleilaxState *tleilaxUi = SM_GetInstance(TleilaxState);
  if (tleilaxUi->starSystem) {
    Galaxy.DestroyStarSystem(tleilaxUi->starSystem);
  }
  printf("Running GalaxyView state.\n");
}

STATE_DEFINE(StarSystemView, NoEventData) {
  ASSERT_TRUE(pEventData);

  printf("Running StarSystemView state.\n");
}

GUARD_DEFINE(StarSystemView, NoEventData) {
  printf("GUARD StarSystemView\n");
  return Tleilax.selectedCoordinates != NULL && Tleilax.selectedStar != NULL;
}

ENTRY_DEFINE(StarSystemView, StarSystemData) {
  printf("ENTRY StarSystemView\n");
  TleilaxState *tleilaxUi = SM_GetInstance(TleilaxState);
  tleilaxUi->starSystem = pEventData->starSystem;
}

EXIT_DEFINE(StarSystemView) {
  printf("EXIT StarSystemView\n");
  TleilaxState *tleilaxUi = SM_GetInstance(TleilaxState);
  assert(tleilaxUi->starSystem);
  Galaxy.DestroyStarSystem(tleilaxUi->starSystem);
  tleilaxUi->starSystem = NULL;
  Tleilax.selectedStar = NULL;
}