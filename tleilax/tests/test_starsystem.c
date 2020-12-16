//
// Created by geoco on 25.11.2020.
//

#include <ECS/ecs.h>
#include <galaxy/starsystem.h>
#include <unity.h>

static World *starSystem;
static Entity *star;
static Entity *planet1;
static Entity *planet2;
static Entity *moon;
static Entity *spaceStation1;
static Entity *spaceStation2;
static Entity *planet3;

void test_Entities_Should_Be_In_Order(void);
void createWorldAndEntities(void);
void addComponents(void);

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

void tearDown(void) { DestroyWorld(starSystem); }

void test_Entities_Should_Be_In_Order(void) {
  TEST_ASSERT_NOT_NULL(starSystem)
  TEST_ASSERT_NOT_NULL(starSystem->entities)
  TEST_ASSERT_EQUAL(star, starSystem->entities);
  TEST_ASSERT_EQUAL(planet1, star->next);
  TEST_ASSERT_EQUAL(planet2, planet1->next);
  TEST_ASSERT_EQUAL(moon, planet2->next);
  TEST_ASSERT_EQUAL(spaceStation1, moon->next);
}

void testEntityRenderAsNodes(void) {
  setvbuf(stdout, NULL, _IONBF, 0);
  GravityCenterTree *gravityCenterTree = CreateEntityTree(starSystem);
  renderNodeTree(gravityCenterTree->gravityCenters[0], gravityCenterTree);
  DestroyGravityCenterTree(gravityCenterTree);

  TEST_PASS();
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(testEntityRenderAsNodes);
  return UNITY_END();
}
