//
// Created by geoco on 27.11.2020.
//
#include <galaxy/starsystem.h>

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

uint32_t getIndexOfGravityCenter(const unsigned long entityId,
                                 const int *indexes, const int amountOfEntries) {
  uint32_t index = NULL;
  uint32_t i = 0;
  while (i < amountOfEntries) {
    if (entityId == *(indexes + i)) {
      index = i;
      break;
    }
    i++;
  }
  return index;
}

GravityCenterTree *CreateEntityTree(const World *world) {

  Entity *pEntity = world->entities;
  uint32_t amountOrbits = 0;
  uint32_t sizeOfOrbitTable = 4;
  uint32_t *indexes = malloc(sizeOfOrbitTable * sizeof(uint32_t));
  printf("          -> indexes malloc(%p)\n", indexes);
  GravityCenterNode **gravityCenterNodes =
      malloc(sizeOfOrbitTable * sizeof(GravityCenterNode));

  /*
   * 1st pass - Create EntityNode array out of entities which have a gravity
   * center component.
   */
  int gravityCenterIndex = 0;
  while (pEntity) {
    Component *gravityCenterComponent = GetComponent(pEntity, GRAVITY_CENTER);
    if (gravityCenterComponent) {
      amountOrbits++;
      if (amountOrbits > sizeOfOrbitTable) {
        sizeOfOrbitTable = 2 * sizeOfOrbitTable;
        gravityCenterNodes = realloc(
            gravityCenterNodes, sizeOfOrbitTable * sizeof(GravityCenterNode));
        indexes = realloc(indexes, sizeof(int));
        printf("          -> indexes realloc(%p)\n", indexes);
      }
      GravityCenterNode *gravityCenterNode = malloc(sizeof(*gravityCenterNode));
      printf("          -> gravityCenterNode:59 malloc(%p)\n", gravityCenterNode);
      if (gravityCenterNode) {
        gravityCenterNode->entity = pEntity;
        gravityCenterNode->children = NULL;
        gravityCenterNode->size = 0;
        gravityCenterNode->amountChildren = 0;

        gravityCenterNodes[gravityCenterIndex] = gravityCenterNode;
        indexes[gravityCenterIndex] = pEntity->id;

        gravityCenterIndex = gravityCenterIndex + 1;
      }
    }
    pEntity = pEntity->next;
  }

  /*
   * 2nd pass - Attach nodes to gravity centers
   */
  pEntity = world->entities;
  while (pEntity) {
    Component *orbitComponent = GetComponent(pEntity, ORBIT);
    if (orbitComponent) {
      struct orbitData *orbitData = orbitComponent->data;
      if (orbitData) {
        gravityCenterIndex = getIndexOfGravityCenter(orbitData->orbitOfEntityId,
                                                     indexes, amountOrbits);
        GravityCenterNode *parentNode = gravityCenterNodes[gravityCenterIndex];
        if (parentNode) {
          if (parentNode->children == NULL) {
            parentNode->size = 1;
            parentNode->amountChildren = 1;
            parentNode->children = malloc(sizeof(parentNode->children));
            printf("          -> parentNode->children:92 malloc(%p)\n", parentNode->children);
            GravityCenterNode *childNode = malloc(sizeof(*childNode));
            printf("          -> childNode:94 malloc(%p)\n", childNode);
            if (childNode) {
              childNode->entity = pEntity;
              childNode->children = NULL;
              childNode->amountChildren = 0;
              childNode->size = 0;
            }
            parentNode->children[0] = childNode;
          } else {
            uint32_t amountChildren = parentNode->amountChildren;
            if (amountChildren + 1 > parentNode->size) {
              parentNode->children =
                  realloc(parentNode->children,
                          2 * parentNode->size * sizeof(parentNode->children));
              printf("          -> parentNode->children:107 realloc(%p)\n", parentNode->children);
              if (parentNode->children) {
                parentNode->size = parentNode->size * 2;
              }
            }
            GravityCenterNode *childNode = malloc(sizeof(*childNode));
            printf("          -> childNode:114 malloc(%p)\n", childNode);
            if (childNode) {
              childNode->entity = pEntity;
              childNode->children = NULL;
              childNode->amountChildren = 0;
              childNode->size = 0;

              parentNode->children[amountChildren] = childNode;
              parentNode->amountChildren++;
            }
          }
        }
      }
    }

    pEntity = pEntity->next;
  }

  GravityCenterTree *gravityCenterTree = malloc(sizeof(*gravityCenterTree));

  printf("          -> gravityCenterTree:133 malloc(%p)\n", gravityCenterTree);
  if (gravityCenterTree) {
    gravityCenterTree->gravityCenters = gravityCenterNodes;
    gravityCenterTree->size = sizeOfOrbitTable;
    gravityCenterTree->amountOfGravityCenters = amountOrbits;
    gravityCenterTree->indexes = indexes;

    return gravityCenterTree;
  }

  return NULL;
}

void DestroyGravityCenterTree(GravityCenterTree *gravityCenterTree) {
  if (gravityCenterTree) {
    int i = 0;
    for (; i < gravityCenterTree->amountOfGravityCenters; i++) {
      if (gravityCenterTree->gravityCenters[i]) {
        printf("Deleting index %d\n", *(gravityCenterTree->indexes + i));
        if (gravityCenterTree->gravityCenters[i]->children != NULL) {
            uint32_t childIndex = 0;
          for (;
               childIndex <
               gravityCenterTree->gravityCenters[i]->amountChildren;
               childIndex++) {
            printf("Deleting child [%lu]\n",
                   gravityCenterTree->gravityCenters[i]
                       ->children[childIndex]
                       ->entity->id);
            if (gravityCenterTree->gravityCenters[i]->children[childIndex]) {
              printf("          -> gravityCenterTree->gravityCenters[i]->children[childIndex] free(%p)\n", gravityCenterTree->gravityCenters[i]->children[childIndex]);
              free(gravityCenterTree->gravityCenters[i]->children[childIndex]);
            }
          }
        }
        printf("          -> gravityCenterTree->gravityCenters[i]->children free(%p)\n", gravityCenterTree->gravityCenters[i]->children);
        free(gravityCenterTree->gravityCenters[i]->children);
        printf("          -> gravityCenterTree->gravityCenters[i] free(%p)\n", gravityCenterTree->gravityCenters[i]);
        free(gravityCenterTree->gravityCenters[i]);
      }
    }
    free(gravityCenterTree->gravityCenters);
    printf("          -> gravityCenterTree->indexes free(%p)\n", gravityCenterTree->indexes);
    free(gravityCenterTree->indexes);
    printf("          -> gravityCenterTree free(%p)\n", gravityCenterTree);
    free(gravityCenterTree);
  }
}

GravityCenterNode *renderNodeTree(GravityCenterNode *rootNode,
                                  const GravityCenterTree *gravityCenterTree) {
  printf("Enter [%lu]\n", rootNode->entity->id);
  if (rootNode->children == NULL) {
    printf("  No orbits around [%lu]\n", rootNode->entity->id);
    return NULL;
  }
  uint32_t childIdx = 0;
  for (; childIdx < rootNode->amountChildren; childIdx++) {
    GravityCenterNode *childNode = rootNode->children[childIdx];
    if (childNode) {
      Component *gravityComponent =
          GetComponent(childNode->entity, GRAVITY_CENTER);
      if (gravityComponent) {
        renderNodeTree(
            gravityCenterTree->gravityCenters[getIndexOfGravityCenter(
                childNode->entity->id, gravityCenterTree->indexes, gravityCenterTree->amountOfGravityCenters)],
            gravityCenterTree);
      } else {
        renderNodeTree(childNode, gravityCenterTree);
      }
    }
  }
  printf("Exit [%lu]\n", rootNode->entity->id);
}

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
    StarData *pStarData = malloc(sizeof(*pStarData));

    if (pStarData) {
      pStarData->name = name;
      pStarData->type = type;
    }
    AddComponent(entity, STAR, pStarData);
  }
}

Component *AddPlanet(Entity *entity, const char *name) {
  if (entity) {
    PlanetData *pPlanetData = malloc(sizeof(*pPlanetData));
    if (pPlanetData) {
      pPlanetData->name = name;
      return AddComponent(entity, PLANET, pPlanetData);
    }
  }
  return NULL;
}

Component *AddMoon(Entity *entity, const char *name) {
  if (entity) {
    MoonData *pMoonData = malloc(sizeof(*pMoonData));
    if (pMoonData) {
      pMoonData->name = name;
      return AddComponent(entity, MOON, pMoonData);
    }
  }
  return NULL;
}

Component *AddSpaceStation(Entity *entity, const char *name) {
  if (entity) {
    SpaceStationData *pSpaceStationData = malloc(sizeof(*pSpaceStationData));
    if (pSpaceStationData) {
      pSpaceStationData->name = name;
      return AddComponent(entity, SPACE_STATION, pSpaceStationData);
    }
  }
  return NULL;
}
