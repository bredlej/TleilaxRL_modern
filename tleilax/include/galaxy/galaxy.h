#ifndef _GALAXY_H_
#define _GALAXY_H_
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
enum STAR_TYPE { STAR_RED, STAR_YELLOW, STAR_BROWN, STAR_BLUE };
enum PLANET_TYPE { EARTHLIKE, GIANT, OCEAN, VOLCANIC, DWARF };

struct StarNames {
  char **names;
  size_t size;
};

typedef struct Coordinates {
  int x;
  int y;
  int z;
} Coordinates;

struct StarNames *LoadNames(char *fileName);

typedef struct Star {
  char *name;
  float size;
  enum STAR_TYPE type;
} Star;

typedef struct Planet {
  char *name;
  enum PLANET_TYPE type;
  float distanceFromStar;
  struct Planet *next;
} Planet;

typedef struct StarSystem {
  Coordinates *coordinates;
  Star *star;
  Planet *planets;
} StarSystem;

struct Galaxy {
  struct offset {
    float x;
    float y;
    float z;
  } offset;
  Star *(*CreateStar)(const char *, const float, const enum STAR_TYPE);
  StarSystem *(*CreateStarSystem)(const Star *star, const Coordinates *coordinates);
  Planet *(*CreatePlanet)(const char *name, const enum PLANET_TYPE type, const float distanceFromStar);

  void (*DestroyStarSystem)(StarSystem *);
  void (*DestroyPlanet)(Planet *);
};

extern struct Galaxy Galaxy;
extern struct Star;
#endif // !_GALAXY_H_
