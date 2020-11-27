//
// Created by geoco on 07.11.2020.
//

#include <ECS/ecs.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

Entity *AddEntity(World *world) {
  assert(world);
  Entity *nextEntity = malloc(sizeof(*nextEntity));
  if (!nextEntity)
    return NULL;
  nextEntity->id = ++(world->lastId);
  nextEntity->next = NULL;
  nextEntity->components = NULL;

  if (world->entities == NULL) {
    world->entities = nextEntity;
  } else {
    Entity *current;
    current = world->entities;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = nextEntity;
  }
  printf("Added entity=[%lu]\n", nextEntity->id);
  return nextEntity;
}

int RemoveEntity(World *world, const unsigned long entityId) {
  assert(world);
  assert(entityId);
  Entity *entity = world->entities;
  if (entity->id == entityId) {
    if (entity->next == NULL) {
      DestroyEntities(entity);
      world->entities = NULL;
      printf("Removed Entity=[%lu] with no successors\n", entityId);
    } else {
      world->entities = entity->next;
      entity->next = NULL;
      DestroyEntities(entity);
      printf("Removed Entity=[%lu]. Successor=[%lu]\n", entityId,
             world->entities->id);
    }
    return 1;
  } else {
    while (entity->next != NULL) {
      Entity *nextEntity = entity->next;
      if (nextEntity->id == entityId) {
        if (nextEntity->next != NULL) {
          entity->next = nextEntity->next;
          nextEntity->next = NULL;
          DestroyEntities(nextEntity);
          printf("Removed Entity=[%lu]. Successor=[%lu]\n", entityId,
                 entity->next->id);
        } else {
          DestroyEntities(entity->next);
          entity->next = NULL;
          printf("Removed Entity=[%lu] with no successors\n", entityId);
        }
        return 1;
      }
      entity = nextEntity;
    }
  }
  return 0;
}

World *CreateWorld(void) {
  World *world = malloc(sizeof(*world));
  if (!world)
    return NULL;
  world->entities = NULL;
  world->lastId = 0;
  printf("Created a new world!\n");
  return world;
}

void DestroyComponents(Component *component) {
  Component *next;
  while (component) {
    next = component->next;
    printf("Destroying component=[%i]\n", component->type);
    if (component->data) {
      printf(" > Destroying component data\n", component->type);
      free(component->data);
    }
    free(component);
    component = next;
  }
}

void DestroyEntities(Entity *entity) {
  Entity *next;
  while (entity) {
    next = entity->next;
    printf("Destroying entity=[%lu]\n", entity->id);
    DestroyComponents(entity->components);
    free(entity);
    entity = next;
  }
}

void DestroyWorld(World *world) {
  assert(world);
  DestroyEntities(world->entities);
  printf("Destroying world\n");
  free(world);
}

Component *AddComponent(Entity *entity, const ComponentType type, void *data) {
  assert(entity);

  Component *component = malloc(sizeof(*component));
  if (!component || !entity)
    return NULL;

  component->next = NULL;
  component->data = data;
  component->type = type;

  if (entity->components == NULL) {
    entity->components = component;
  } else {
    Component *current = entity->components;
    if (current->type == type) {
      return NULL;
    }
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = component;
  }

  printf("Added component=[%i] to entity=[%lu]\n", type, entity->id);
  return component;
}

int RemoveComponent(Entity *entity, const ComponentType type) {
  assert(entity);

  Component *component = entity->components;
  if (component->type == type) {
    if (component->next == NULL) {
      DestroyComponents(component);
      entity->components = NULL;
      printf("Removed Component=[%d] with no successors\n", type);
    } else {
      entity->components = component->next;
      component->next = NULL;
      DestroyComponents(component);
      printf("Removed Component=[%d]. Successor=[%d]\n", type,
             entity->components->type);
    }
    return 1;
  } else {
    while (component->next != NULL) {
      Component *nextComponent = component->next;
      if (nextComponent->type == type) {
        if (nextComponent->next != NULL) {
          component->next = nextComponent->next;
          nextComponent->next = NULL;
          DestroyComponents(nextComponent);
          printf("Removed Component=[%d]. Successor=[%d]\n", type,
                 component->next->type);
        } else {
          DestroyComponents(component->next);
          component->next = NULL;
          printf("Removed Component=[%d] with no successors\n", type);
        }
        return 1;
      }
      component = nextComponent;
    }
  }
  return 0;
}

Component *GetComponent(const Entity *entity, const ComponentType type) {
  assert(entity);
  Component *component = entity->components;
  while(component) {
    if (component->type == type) {
      return component;
    }
    component = component->next;
  }
  return NULL;
}
