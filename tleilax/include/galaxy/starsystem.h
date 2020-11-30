//
// Created by geoco on 27.11.2020.
//
#ifndef TLEILAX_STARSYSTEM_H
#define TLEILAX_STARSYSTEM_H
#include <ECS/ecs.h>
#include <stdlib.h>

struct orbitData {
  unsigned long orbitOfEntityId;
  unsigned int distance;
  float degrees;
};

typedef struct starData {
  char *name;
  int type;
} StarData;

typedef struct planetData {
  char *name;
} PlanetData;

typedef struct moonData {
  char *name;
} MoonData;

typedef struct spaceStationData {
  char *name;
} SpaceStationData;

Component *AddGravityCenter(Entity *entity);
Component *AddOrbit(Entity *entity, const unsigned long aroundEntityId,
                    const unsigned int distance, const float degrees);
Component *AddStar(Entity *entity, const char *name, const int type);
Component *AddPlanet(Entity *entity, const char *name);
Component *AddMoon(Entity *entity, const char *name);
Component *AddSpaceStation(Entity *entity, const char *name);


#endif // TLEILAX_STARSYSTEM_H
