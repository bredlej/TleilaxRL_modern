//
// Created by geoco on 07.11.2020.
//

#include <stdlib.h>
#include <stdio.h>
#include <ECS/ecs.h>

Entity *AddEntity(World *world)
{
    Entity *nextEntity = malloc(sizeof(*nextEntity));
    if (!nextEntity)
        return NULL;
    nextEntity->id = ++(world->lastId);
    nextEntity->next = NULL;
    nextEntity->components = NULL;

    if (world->entities == NULL) {
        world->entities = nextEntity;
    }
    else {
        Entity *current;
        current = world->entities;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = nextEntity;
    }
    printf("Added entity=[%i]\n", nextEntity->id);
    return nextEntity;
}

World *CreateWorld()
{
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
    while(component) {
        next = component->next;
        printf("Destroying component=[%i]\n", component->type);
        free(component);
        component = next;
    }
}
void DestroyEntities(Entity *entity) {
    Entity *next;
    while (entity) {
        next = entity->next;
        printf("Destroying entity=[%i]\n", entity->id);
        DestroyComponents(entity->components);
        free(entity);
        entity = next;
    }
}
void DestroyWorld(World *world)
{
    Entity *entity, *next;
    entity = world->entities;
    DestroyEntities(world->entities);
    printf("Destroying world");
    free(world);
}

Component *AddComponent(Entity *entity, const ComponentType type, void *data)
{
    Component *component = malloc(sizeof(*component));
    if (!component || !entity)
        return NULL;

    component->next = NULL;
    component->data = data;
    component->type = type;

    if (entity->components == NULL) {
        entity->components = component;
    }
    else {
        Component *current = entity->components;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = component;
    }

    printf("Added component=[%i] to entity=[%i]\n", type, entity->id);
    return component;
}


