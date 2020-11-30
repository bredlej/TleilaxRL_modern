#ifndef GALAXY_H_
#define GALAXY_H_
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
enum STAR_TYPE { STAR_RED, STAR_YELLOW, STAR_BROWN, STAR_BLUE };
enum PLANET_TYPE { EARTHLIKE, GIANT, OCEAN, VOLCANIC, DWARF };

struct StarNames {
  char **names;
  uint32_t size;
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

struct Galaxy {
  struct offset {
    float x;
    float y;
    float z;
  } offset;
  Star *(*CreateStar)(const char *, const float, const enum STAR_TYPE);
};

extern struct Galaxy Galaxy;
#endif // !GALAXY_H_
