//
// Created by geoco on 27.11.2020.
//
#ifndef TLEILAX_STARSYSTEM_H
#define TLEILAX_STARSYSTEM_H
#include <ECS/ecs.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

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

typedef struct gravityCenterNode {
  Entity *entity;
  struct gravityCenterNode **children;
  uint32_t amountChildren;
  size_t size;
} GravityCenterNode;

typedef struct gravityCenterTree {
  GravityCenterNode **gravityCenters;
  uint32_t amountOfGravityCenters;
  uint32_t *indexes;
  size_t size;
} GravityCenterTree;

Component *AddGravityCenter(Entity *entity);
Component *AddOrbit(Entity *entity, const unsigned long aroundEntityId,
                    const unsigned int distance, const float degrees);
Component *AddStar(Entity *entity, const char *name, const int type);
Component *AddPlanet(Entity *entity, const char *name);
Component *AddMoon(Entity *entity, const char *name);
Component *AddSpaceStation(Entity *entity, const char *name);

/**
 * Helper function for obtaining the index of a given entity in the array of
 * GravityCenterNodes.
 *
 * @param entityId Id of the entity associated with the node. Should have a
 * GRAVITY_CENTER component attached
 * @param gravityCenterTree reference to a the struct where the array is stored
 * @returns index of node in table or NULL if not present
 */
static uint32_t
getIndexOfGravityCenter(const unsigned long entityId,
                        const int *indexes, int amountOfEntries);
GravityCenterNode *renderNodeTree(GravityCenterNode *rootNode,
                                  const GravityCenterTree *gravityCenterTree);
GravityCenterTree *CreateEntityTree(const World *world);
void DestroyGravityCenterTree(GravityCenterTree *gravityCenterTree);


#endif // TLEILAX_STARSYSTEM_H
