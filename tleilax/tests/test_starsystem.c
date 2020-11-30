//
// Created by geoco on 25.11.2020.
//

#include <ECS/ecs.h>
#include <galaxy/starsystem.h>
#include <unity.h>

static World *starSystem;
static Entity *gravityCenterOfStar;
static Entity *star;
static Entity *planet1;
static Entity *gravityCenterOfPlanet2;
static Entity *planet2;
static Entity *moon;
static Entity *spaceStation1;
static Entity *spaceStation2;
static Entity *planet3;

void test_Entities_Should_Be_In_Order(void);
void createWorldAndEntities(void);
void addComponents(void);

void renderStar(const Entity *entity, long entityId, const Component *star,
                int i);
void renderPlanet(const Entity *entity, long entityId, const Component *planet,
                  int i);
void renderMoon(const Entity *entity, long entityId, const Component *moon,
                int i);
void renderSpaceStation(const Entity *entity, long entityId,
                        const Component *spaceStation, int i);
void renderComponentsOnOrbit(const Entity *entity, long entityId);
void renderComponentsOnGravityCenter(const Entity *pEntity, unsigned long id);
uint32_t getIndexOfGravityCenter(const unsigned long entityId,
                                 const uint32_t *gravityCenterIndexes,
                                 const uint32_t amountGravityCenters);
void setUp(void) {
  createWorldAndEntities();
  addComponents();
}

void createWorldAndEntities() {
  starSystem = CreateWorld();
  star = AddEntity(starSystem);
  planet1 = AddEntity(starSystem);
  planet2 = AddEntity(starSystem);
  spaceStation1 = AddEntity(starSystem);
  moon = AddEntity(starSystem);
  spaceStation2 = AddEntity(starSystem);
  planet3 = AddEntity(starSystem);
}

char *printEntity(int id) {
  switch (id) {
  case 1:
    return "[Star] Tleilax";
  case 2:
    return "[Planet] Unnax";
  case 3:
    return "[Planet] Giedi Prime";
  case 4:
    return "[Space Station] Quan";
  case 5:
    return "[Moon] Yiqian";
  case 6:
    return "[Space Station] Orso";
  case 7:
    return "[Planet] Hilarios";
  default:
    return "unknown";
  }
}

typedef struct node {
  Entity *entity;
  struct node **children;
  uint32_t amountChildren;
  size_t size;
} EntityNode;

static int level = 0;

EntityNode *renderNode(EntityNode *node) {
  /*
   * 1 Star
   *    2 Planet 1
   *    3 Planet 2
   *        4 Moon
   *    5 Space Station 1
   * 6 Space Station 2
   */
  int offset = 0;
  for (; offset < level; offset++)
    printf(" ");
  printf("-> %s [%lu]\n", printEntity(node->entity->id), node->entity->id);
  level += 2;
  if (node->children == NULL) {
    int offset = 0;
    level -= 2;
    for (; offset < level; offset++)
      printf(" ");
    printf("<- %s\n", printEntity(node->entity->id));
    return NULL;
  }
  int index = 0;
  for (; index < node->amountChildren; index++) {
    (void)renderNode(node->children[index]);
  }
  offset = 0;
  level -= 2;
  for (; offset < level; offset++)
    printf(" ");
  printf("<- %s\n", printEntity(node->entity->id));
}

EntityNode *renderNodeTree(EntityNode *rootNode, EntityNode **gravityNodes,
                           uint32_t *indexes, uint32_t i) {
  printf("Enter [%s]\n", printEntity(rootNode->entity->id));
  if (rootNode->children == NULL) {
    printf("  No orbits around [%s]\n", printEntity(rootNode->entity->id));
    return NULL;
  }
  for (int childIdx = 0; childIdx < rootNode->amountChildren; childIdx++) {
    EntityNode *childNode = rootNode->children[childIdx];
    if (childNode) {
      Component *gravityComponent = GetComponent(childNode->entity, GRAVITY_CENTER);
      if (gravityComponent) {
        renderNodeTree(gravityNodes[getIndexOfGravityCenter(childNode->entity->id, indexes, i)], gravityNodes, indexes, i);
      }
      else {
        renderNodeTree(childNode, gravityNodes, indexes, i);
      }
    }
  }
  printf("Exit [%s]\n", printEntity(rootNode->entity->id));
}

uint32_t getIndexOfGravityCenter(const unsigned long entityId,
                                 const uint32_t *gravityCenterIndexes,
                                 const uint32_t amountGravityCenters) {
  uint32_t index = NULL;
  uint32_t i = 0;
  while (i < amountGravityCenters) {
    if (entityId == *(gravityCenterIndexes + i)) {
      index = i;
      break;
    }
    i++;
  }
  return index;
}

EntityNode *CreateEntityTree(const World *world) {

  Entity *pEntity = world->entities;
  uint32_t amountOrbits = 0;
  uint32_t sizeOfOrbitTable = 4;
  uint32_t *indexes = malloc(sizeOfOrbitTable * sizeof(uint32_t));
  EntityNode **gravityCenterNodes =
      malloc(sizeOfOrbitTable * sizeof(EntityNode));

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
        gravityCenterNodes =
            realloc(gravityCenterNodes, sizeOfOrbitTable * sizeof(EntityNode));
        indexes = realloc(indexes, sizeof(int));
      }
      EntityNode *gravityCenterNode = malloc(sizeof(*gravityCenterNode));
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

  pEntity = world->entities;
  while (pEntity) {
    Component *orbitComponent = GetComponent(pEntity, ORBIT);
    if (orbitComponent) {
      struct orbitData *orbitData = orbitComponent->data;
      if (orbitData) {
        gravityCenterIndex = getIndexOfGravityCenter(orbitData->orbitOfEntityId,
                                                     indexes, amountOrbits);
        EntityNode *parentNode = gravityCenterNodes[gravityCenterIndex];
        if (parentNode) {
          if (parentNode->children == NULL) {
            parentNode->size = 1;
            parentNode->amountChildren = 1;
            parentNode->children = malloc(sizeof(EntityNode));

            EntityNode *childNode = malloc(sizeof(EntityNode));
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
                          2 * parentNode->size * sizeof(EntityNode));
              if (parentNode->children) {
                parentNode->size = parentNode->size * 2;
              }
            }
            EntityNode *childNode = malloc(sizeof(EntityNode));
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

  renderNodeTree(gravityCenterNodes[0], gravityCenterNodes, indexes,
                 amountOrbits);

  int i = 0;
  for (; i < amountOrbits; i++) {
    if (gravityCenterNodes[i]) {
      printf("Deleting index %d\n", *(indexes + i));
      if (gravityCenterNodes[i]->children != NULL) {
        for (uint32_t childIndex = 0;
             childIndex < gravityCenterNodes[i]->amountChildren; childIndex++) {
          printf("Deleting child [%lu]\n",
                 gravityCenterNodes[i]->children[childIndex]->entity->id);
          free(gravityCenterNodes[i]->children[childIndex]);
        }
      }
      free(gravityCenterNodes[i]);
    }
  }
  free(indexes);
  return NULL;
}

void renderEntityTree() {
  EntityNode starNode;
  EntityNode planet1Node;
  EntityNode planet2Node;
  EntityNode spaceStation1Node;
  EntityNode moonNode;
  EntityNode spaceStation2Node;

  starNode.entity = star;
  starNode.children = malloc(2 * sizeof(EntityNode));
  starNode.size = 2;
  starNode.children[0] = &planet1Node;
  starNode.children[1] = &planet2Node;

  planet1Node.entity = planet1;
  planet1Node.children = NULL;
  planet1Node.size = 0;

  planet2Node.entity = planet2;
  planet2Node.children = malloc(2 * sizeof(EntityNode));
  planet2Node.size = 2;
  planet2Node.children[0] = &spaceStation1Node;
  planet2Node.children[1] = &moonNode;

  spaceStation1Node.entity = spaceStation1;
  spaceStation1Node.children = NULL;
  spaceStation1Node.size = 0;

  moonNode.entity = moon;
  moonNode.children = malloc(1 * sizeof(EntityNode));
  moonNode.size = 1;
  moonNode.children[0] = &spaceStation2Node;

  spaceStation2Node.entity = spaceStation2;
  spaceStation2Node.children = NULL;
  spaceStation2Node.size = 0;

  //  renderNode(&starNode);

  free(moonNode.children);
  free(planet2Node.children);
  free(starNode.children);
}

void addComponents(void) {

  AddStar(star, "Tleilax", 999);
  AddGravityCenter(star);

  AddPlanet(planet1, "Unnax");
  AddOrbit(planet1, star->id, 1, 10.0f);

  AddPlanet(planet2, "Giedi Prime");
  AddGravityCenter(planet2);
  AddOrbit(planet2, star->id, 5, 10.0f);

  AddMoon(moon, "Yiqian");
  AddGravityCenter(moon);

  AddSpaceStation(spaceStation2, "Orso");
  AddOrbit(spaceStation2, moon->id, 1, 10.0f);

  AddSpaceStation(spaceStation1, "Quan");
  AddOrbit(spaceStation1, planet2->id, 2, 120.0f);

  AddPlanet(planet3, "Hilarios");
  AddGravityCenter(planet3);
  AddOrbit(planet3, star->id, 10, 88.0f);
  AddOrbit(moon, planet3->id, 1, 30.0f);
}

void traverse(World *starsystem) {
  Entity *entity = starsystem->entities;
  printf("\n");
  printf("--- Traversing starsystem\n");
  while (entity) {
    Component *gravityCenter = GetComponent(entity, GRAVITY_CENTER);
    if (gravityCenter) {
      printf("Entity=[%lu] is a gravity center\n", entity->id);
    }
    Component *orbit = GetComponent(entity, ORBIT);
    if (orbit) {
      struct orbitData *orbitData = orbit->data;
      printf("  - Entity=[%lu] is orbiting entity=[%lu]\n", entity->id,
             orbitData->orbitOfEntityId);
    }
    entity = entity->next;
  }
  printf("--- End of traverse \n");
  printf("\n");
}

void renderStarsystem(const Entity *entity, long entityId) {
  uint8_t hasGravityCenter = 0;
  if (entity) {
    Component *gravityCenter = GetComponent(entity, GRAVITY_CENTER);
    if (gravityCenter) {
      if (entityId) {
        printf("[%lu] popMatrix\n", entityId);
      }
      printf("[%lu] pushMatrix\n", entity->id);
      hasGravityCenter = 1;
      renderComponentsOnGravityCenter(entity, entity->id);
    }

    Component *orbit = GetComponent(entity, ORBIT);
    if (orbit) {
      renderComponentsOnOrbit(entity, entityId);
    }
    renderStarsystem(entity->next, hasGravityCenter ? entity->id : entityId);

  } else {
    printf("[%lu] popMatrix\n", entityId);
  }
}

void renderComponentsOnOrbit(const Entity *entity, long entityId) {
  Component *star = GetComponent(entity, STAR);
  if (star) {
    renderStar(entity, entityId, star, 0);
  }
  Component *planet = GetComponent(entity, PLANET);
  if (planet) {
    renderPlanet(entity, entityId, planet, 0);
  }
  Component *moon = GetComponent(entity, MOON);
  if (moon) {
    renderMoon(entity, entityId, moon, 0);
  }
  Component *spaceStation = GetComponent(entity, SPACE_STATION);
  if (spaceStation) {
    renderSpaceStation(entity, entityId, spaceStation, 0);
  }
}

void renderComponentsOnGravityCenter(const Entity *entity,
                                     unsigned long entityId) {
  Component *star = GetComponent(entity, STAR);
  if (star) {
    renderStar(entity, entityId, star, 1);
  }
  Component *planet = GetComponent(entity, PLANET);
  if (planet) {
    renderPlanet(entity, entityId, planet, 1);
  }
  Component *moon = GetComponent(entity, MOON);
  if (moon) {
    renderMoon(entity, entityId, moon, 1);
  }
  Component *spaceStation = GetComponent(entity, SPACE_STATION);
  if (spaceStation) {
    renderSpaceStation(entity, entityId, spaceStation, 1);
  }
}

void renderSpaceStation(const Entity *entity, long entityId,
                        const Component *spaceStation, int i) {
  SpaceStationData *spaceStationData = spaceStation->data;
  if (spaceStationData) {
    printf("  [%lu] pushMatrix\n", entityId);
    printf("    SpaceStation %s [%lu] orbiting entity=[%lu]\n",
           spaceStationData->name, entity->id, entityId);
    printf("  [%lu] popMatrix\n", entityId);
  }
}
void renderMoon(const Entity *entity, long entityId, const Component *moon,
                int i) {
  MoonData *moonData = moon->data;
  if (moonData) {
    printf("  [%lu] pushMatrix\n", entityId);
    printf("    Moon %s [%lu] orbiting entity=[%lu]\n", moonData->name,
           entity->id, entityId);
    printf("  [%lu] popMatrix\n", entityId);
  }
}
void renderPlanet(const Entity *entity, long entityId, const Component *planet,
                  int i) {
  PlanetData *planetData = planet->data;
  if (planetData) {
    printf("  [%lu] pushMatrix\n", entityId);
    printf("    Planet %s [%lu] orbiting entity=[%lu]\n", planetData->name,
           entity->id, entityId);
    printf("  [%lu] popMatrix\n", entityId);
  }
}

void renderStar(const Entity *entity, long entityId, const Component *star,
                int i) {
  StarData *starData = star->data;
  if (starData) {
    if (i) {
      printf("  Star %s [%lu] is a gravity center!\n", starData->name,
             entity->id, entityId);
    } else {
      printf("  [%lu] pushMatrix\n", entityId);
      printf("    Star %s [%lu] orbiting [%lu]\n", starData->name, entity->id,
             entityId);
      printf("  [%lu] popMatrix\n", entityId);
    }
  }
}

void tearDown(void) { DestroyWorld(starSystem); }

void test_Entities_Should_Be_In_Order(void) {
  // traverse(starSystem);
  renderStarsystem(starSystem->entities, NULL);
  TEST_ASSERT_NOT_NULL(starSystem)
  TEST_ASSERT_NOT_NULL(starSystem->entities)
  TEST_ASSERT_EQUAL(star, starSystem->entities);
  TEST_ASSERT_EQUAL(planet1, star->next);
  TEST_ASSERT_EQUAL(planet2, planet1->next);
  TEST_ASSERT_EQUAL(moon, planet2->next);
  TEST_ASSERT_EQUAL(spaceStation1, moon->next);
}

void testEntityRenderAsNodes(void) {
  renderEntityTree();
  CreateEntityTree(starSystem);
  TEST_PASS();
}

int main(void) {
  UNITY_BEGIN();
  // RUN_TEST(test_Entities_Should_Be_In_Order);
  RUN_TEST(testEntityRenderAsNodes);
  return UNITY_END();
}
