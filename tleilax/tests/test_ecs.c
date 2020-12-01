//
// Created by Patryk Szczypień on 09/11/2020.
//

#include <ECS/ecs.h>
#include <unity.h>
#include <stdlib.h>

void setUp(void) {}

void tearDown(void) {}

static void test_Should_Create_World() {
  World *world = CreateWorld();
  TEST_ASSERT_EQUAL(0, world->lastId);
  DestroyWorld(world);
}

static void test_Should_Create_Entities() {
  World *world = CreateWorld();
  TEST_ASSERT_EQUAL(0, world->lastId);
  Entity *entity = AddEntity(world);
  TEST_ASSERT_EQUAL(1, world->lastId);
  TEST_ASSERT_EQUAL(1, entity->id);
  TEST_ASSERT_EQUAL(NULL, entity->components);
  TEST_ASSERT_EQUAL(NULL, entity->next);
  Entity *nextEntity = AddEntity(world);
  TEST_ASSERT_EQUAL(2, world->lastId);
  TEST_ASSERT_EQUAL(1, entity->id);
  TEST_ASSERT_EQUAL(2, nextEntity->id);
  TEST_ASSERT_EQUAL(nextEntity, entity->next);
  DestroyWorld(world);
}

static void test_Should_Remove_First_Entity_Without_Successors() {
  World *world = CreateWorld();
  AddEntity(world);
  RemoveEntity(world, 1);
  TEST_ASSERT_NULL(world->entities);
  DestroyWorld(world);
}

static void test_Should_Remove_First_Entity_With_Successors() {
  World *world = CreateWorld();
  AddEntity(world);
  AddEntity(world);
  RemoveEntity(world, 1);
  TEST_ASSERT_EQUAL(2, world->entities->id);
  TEST_ASSERT_NULL(world->entities->next);
  DestroyWorld(world);
}

static void test_Should_Remove_Second_Entity_Without_Successors() {
  World *world = CreateWorld();
  AddEntity(world);
  AddEntity(world);
  RemoveEntity(world, 2);
  TEST_ASSERT_EQUAL(1, world->entities->id);
  TEST_ASSERT_NULL(world->entities->next);
  DestroyWorld(world);
}

static void test_Should_Remove_Second_Entity_With_Successors() {
  World *world = CreateWorld();
  AddEntity(world);
  AddEntity(world);
  AddEntity(world);
  RemoveEntity(world, 2);
  TEST_ASSERT_EQUAL(1, world->entities->id);
  TEST_ASSERT_EQUAL(3, world->entities->next->id);
  TEST_ASSERT_NULL(world->entities->next->next);
  DestroyWorld(world);
}

static void test_Should_Not_Remove_Entity() {
  World *world = CreateWorld();
  AddEntity(world);
  AddEntity(world);
  TEST_ASSERT_EQUAL(0, RemoveEntity(world, 3));
  TEST_ASSERT_EQUAL(2, world->lastId);
  TEST_ASSERT_EQUAL(1, world->entities->id);
  TEST_ASSERT_EQUAL(2, world->entities->next->id);
  DestroyWorld(world);
}

static void test_Should_Add_Component_To_Entity() {
  World *world = CreateWorld();
  Entity *entity = AddEntity(world);
  struct position {
    int x;
    int y;
  } *position = malloc(sizeof(*position));
  position->x = 1;
  position->y = 2;
  AddComponent(entity, POSITION, position);

  TEST_ASSERT_NOT_NULL(world->entities->components);
  TEST_ASSERT_NULL(world->entities->components->next);
  TEST_ASSERT_EQUAL(POSITION, world->entities->components->type);
  struct position *positionComponentData = world->entities->components->data;
  TEST_ASSERT_EQUAL(1, positionComponentData->x);
  TEST_ASSERT_EQUAL(2, positionComponentData->y);
  DestroyWorld(world);
}

static void test_Should_Add_Only_One_Component_Of_Given_Type() {
  World *world = CreateWorld();
  Entity *entity = AddEntity(world);
  struct position {
    int x;
    int y;
  };
  struct position *position1 = malloc (sizeof(*position1));
  position1->x = 1;
  position1->y = 2;
  struct position *position2 = malloc (sizeof(*position1));
  position2->x = 3;
  position2->y = 4;
  AddComponent(entity, POSITION, position1);
  Component *invalidComponent = AddComponent(entity, POSITION, position2);
  if (!invalidComponent) {
    free(position2);
  }
  TEST_ASSERT_NOT_NULL(world->entities->components);
  TEST_ASSERT_NULL(world->entities->components->next);
  TEST_ASSERT_EQUAL(POSITION, world->entities->components->type);
  struct position *positionComponentData = world->entities->components->data;
  TEST_ASSERT_EQUAL(1, positionComponentData->x);
  TEST_ASSERT_EQUAL(2, positionComponentData->y);
  DestroyWorld(world);
}

static void test_Should_Add_Two_Components_To_Entity() {
  World *world = CreateWorld();
  Entity *entity = AddEntity(world);
  struct position {
    int x;
    int y;
  } *position = malloc(sizeof(*position));

  struct rotation {
    float degrees;
  } *rotation = malloc(sizeof(*rotation));

  position->x = 1;
  position->y = 2;
  rotation->degrees = 30.0f;

  Component *positionComponent = AddComponent(entity, POSITION, position);
  Component *rotationComponent = AddComponent(entity, ROTATION, rotation);

  TEST_ASSERT_EQUAL(positionComponent, world->entities->components);
  TEST_ASSERT_EQUAL(rotationComponent, world->entities->components->next);
  TEST_ASSERT_NULL(world->entities->components->next->next);

  struct position *positionComponentData = world->entities->components->data;
  struct rotation *rotationComponentData =
      world->entities->components->next->data;
  TEST_ASSERT_EQUAL(1, positionComponentData->x);
  TEST_ASSERT_EQUAL(2, positionComponentData->y);
  TEST_ASSERT_EQUAL(30.0f, rotationComponentData->degrees);

  DestroyWorld(world);
}

static void test_Should_Remove_First_Component_Without_Successors() {
  World *world = CreateWorld();
  Entity *entity = AddEntity(world);
  AddComponent(entity, POSITION, NULL);
  TEST_ASSERT_EQUAL(1, RemoveComponent(entity, POSITION));
  TEST_ASSERT_NULL(entity->components);
  DestroyWorld(world);
}

static void test_Should_Remove_First_Component_With_Successors() {
  World *world = CreateWorld();
  Entity *entity = AddEntity(world);
  AddComponent(entity, POSITION, NULL);
  AddComponent(entity, ROTATION, NULL);
  TEST_ASSERT_EQUAL(1, RemoveComponent(entity, POSITION));
  TEST_ASSERT_EQUAL(ROTATION, entity->components->type);
  TEST_ASSERT_NULL(entity->components->next);
  DestroyWorld(world);
}

static void test_Should_Remove_Second_Component_Without_Successors() {
  World *world = CreateWorld();
  Entity *entity = AddEntity(world);
  AddComponent(entity, POSITION, NULL);
  AddComponent(entity, ROTATION, NULL);
  TEST_ASSERT_EQUAL(1, RemoveComponent(entity, ROTATION));
  TEST_ASSERT_EQUAL(POSITION, entity->components->type);
  TEST_ASSERT_NULL(entity->components->next);
  DestroyWorld(world);
}

static void test_Should_Remove_Second_Component_With_Successors() {
  World *world = CreateWorld();
  Entity *entity = AddEntity(world);
  AddComponent(entity, POSITION, NULL);
  AddComponent(entity, ROTATION, NULL);
  AddComponent(entity, SCALE, NULL);
  TEST_ASSERT_EQUAL(1, RemoveComponent(entity, ROTATION));
  TEST_ASSERT_EQUAL(POSITION, entity->components->type);
  TEST_ASSERT_NOT_NULL(entity->components->next);
  TEST_ASSERT_EQUAL(SCALE, entity->components->next->type);

  DestroyWorld(world);
}

static void test_Should_Not_Remove_Component() {
  World *world = CreateWorld();
  Entity *entity = AddEntity(world);
  AddComponent(entity, POSITION, NULL);
  TEST_ASSERT_EQUAL(0, RemoveComponent(entity, SCALE));
  TEST_ASSERT_EQUAL(POSITION, entity->components->type);
  TEST_ASSERT_NULL(entity->components->next);
  DestroyWorld(world);
}

static void test_Should_Change_Component_Value() {
  struct position {
    int x;
    int y;
  } *position = malloc(sizeof(*position));
  position->x = 0;
  position->y = 0;

  World *world = CreateWorld();
  Entity *entity = AddEntity(world);
  AddComponent(entity, POSITION, position);
  struct position *positionComponentData = entity->components->data;
  TEST_ASSERT_EQUAL(0, positionComponentData->x);
  TEST_ASSERT_EQUAL(0, positionComponentData->y);
  positionComponentData->x = 1;
  positionComponentData->y = 1;
  struct position *positionComponentDataVerification = entity->components->data;
  TEST_ASSERT_EQUAL(1, positionComponentDataVerification->x);
  TEST_ASSERT_EQUAL(1, positionComponentDataVerification->y);

  DestroyWorld(world);
}

static void test_Should_Get_Component_Of_Proper_Type() {
  World *world = CreateWorld();
  Entity *entity = AddEntity(world);
  Component *positionComponent = AddComponent(entity, POSITION, NULL);
  AddComponent(entity, ROTATION, NULL);
  TEST_ASSERT_EQUAL(positionComponent, GetComponent(entity, POSITION));
  TEST_ASSERT_NULL(GetComponent(entity, SCALE));

  DestroyWorld(world);
}

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_Should_Create_World);
  RUN_TEST(test_Should_Create_Entities);
  RUN_TEST(test_Should_Remove_First_Entity_Without_Successors);
  RUN_TEST(test_Should_Remove_First_Entity_With_Successors);
  RUN_TEST(test_Should_Remove_Second_Entity_Without_Successors);
  RUN_TEST(test_Should_Remove_Second_Entity_With_Successors);
  RUN_TEST(test_Should_Not_Remove_Entity);
  RUN_TEST(test_Should_Add_Component_To_Entity);
  RUN_TEST(test_Should_Add_Only_One_Component_Of_Given_Type);
  RUN_TEST(test_Should_Add_Two_Components_To_Entity);
  RUN_TEST(test_Should_Remove_First_Component_Without_Successors);
  RUN_TEST(test_Should_Remove_First_Component_With_Successors);
  RUN_TEST(test_Should_Remove_Second_Component_Without_Successors);
  RUN_TEST(test_Should_Remove_Second_Component_With_Successors);
  RUN_TEST(test_Should_Not_Remove_Component);
  RUN_TEST(test_Should_Change_Component_Value);
  RUN_TEST(test_Should_Get_Component_Of_Proper_Type);

  return UNITY_END();
}