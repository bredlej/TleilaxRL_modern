//
// Created by geoco on 27.11.2020.
//
#include <galaxy/starsystem.h>

Component *AddGravityCenter(Entity *entity) {
  if (entity) {
    return AddComponent(entity, GRAVITY_CENTER, NULL);
  }
}

Component *AddOrbit(Entity *entity, const unsigned long aroundEntityId,
                    const unsigned int distance, const float degrees) {
  if (entity) {
    struct orbitData *pOrbitData = malloc(sizeof(*pOrbitData));
    if (pOrbitData) {
      pOrbitData->orbitOfEntityId = aroundEntityId;
      pOrbitData->distance = distance;
      pOrbitData->degrees = degrees;
      return AddComponent(entity, ORBIT, pOrbitData);
    }
  }
  return NULL;
}

Component *AddStar(Entity *entity, const char *name, const int type) {
  if (entity) {
    struct starData {
      char *name;
      int type;
    } *pStarData = malloc(sizeof(*pStarData));

    if (pStarData) {
      pStarData->name = name;
      pStarData->type = type;
    }
    AddComponent(entity, STAR, pStarData);
  }
}

Component *AddPlanet(Entity *entity, const char *name) {
  if (entity) {
    struct planetData {
      char *name;
    } *pPlanetData = malloc(sizeof(*pPlanetData));
    if (pPlanetData) {
      pPlanetData->name = name;
      return AddComponent(entity, PLANET, pPlanetData);
    }
  }
  return NULL;
}

Component *AddMoon(Entity *entity, const char *name) {
  if (entity) {
    struct moonData {
      char *name;
    } *pMoonData = malloc(sizeof(*pMoonData));
    if (pMoonData) {
      pMoonData->name = name;
      return AddComponent(entity, MOON, pMoonData);
    }
  }
  return NULL;
}

Component *AddSpaceStation(Entity *entity, const char *name) {
  if (entity) {
    struct spaceStationData {
      char *name
    } *pSpaceStationData = malloc(sizeof(*pSpaceStationData));
    if (pSpaceStationData) {
      pSpaceStationData->name = name;
      return AddComponent(entity, SPACE_STATION, pSpaceStationData);
    }
  }
  return NULL;
}

