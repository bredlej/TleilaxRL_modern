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
static Entity *spaceStation;

void test_Entities_Should_Be_In_Order(void);
void createWorldAndEntities(void);
void addComponents(void);

void setUp(void) {
  createWorldAndEntities();
  addComponents();
}

void createWorldAndEntities() {
  starSystem = CreateWorld();
  gravityCenterOfStar = AddEntity(starSystem);
  star = AddEntity(starSystem);
  planet1 = AddEntity(starSystem);
  gravityCenterOfPlanet2 = AddEntity(starSystem);
  planet2 = AddEntity(starSystem);
  moon = AddEntity(starSystem);
  spaceStation = AddEntity(starSystem);
}



void addComponents(void) {

  /* - Gravity center #1*/
  AddGravityCenter(gravityCenterOfStar);
  /* | */
  /* - Star */
  AddStar(star, "Tleilax", 999);
  AddOrbit(star, gravityCenterOfStar->id, 0, 0.0f);
  /* | */
  /* - Planet #1 */
  AddPlanet(planet1, "Unnax");
  AddOrbit(planet1, gravityCenterOfStar->id, 1, 10.0f);
  /* | */
  /* - Gravity center #2 */
  AddGravityCenter(gravityCenterOfPlanet2);
  AddOrbit(gravityCenterOfPlanet2, gravityCenterOfStar->id, 5, 10.0f);
  /*        | */
  /*        - Planet #2 */
  AddPlanet(planet2, "Oiko");
  AddOrbit(planet2, gravityCenterOfPlanet2->id, 0, 0.0f);
  /*        | */
  /*        - Moon */
  AddMoon(moon, "Yiqian");
  AddOrbit(moon, gravityCenterOfPlanet2->id, 1, 30.0f);
  /*        | */
  /*        - Space Station */
  AddSpaceStation(spaceStation, "Quan");
  AddOrbit(spaceStation, gravityCenterOfPlanet2->id, 2, 120.0f);
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


void tearDown(void) {
  DestroyWorld(starSystem);
}

void test_Entities_Should_Be_In_Order(void) {
  traverse(starSystem);
  TEST_ASSERT_NOT_NULL(starSystem)
  TEST_ASSERT_NOT_NULL(starSystem->entities)
  TEST_ASSERT_EQUAL(gravityCenterOfStar, starSystem->entities);
  TEST_ASSERT_EQUAL(star, gravityCenterOfStar->next);
  TEST_ASSERT_EQUAL(planet1, star->next);
  TEST_ASSERT_EQUAL(gravityCenterOfPlanet2, planet1->next);
  TEST_ASSERT_EQUAL(planet2, gravityCenterOfPlanet2->next);
  TEST_ASSERT_EQUAL(moon, planet2->next);
  TEST_ASSERT_EQUAL(spaceStation, moon->next);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_Entities_Should_Be_In_Order);
  return UNITY_END();
}
