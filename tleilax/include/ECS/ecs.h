//
// Created by geoco on 07.11.2020.
//

#ifndef TANKS_ECS_H
#define TANKS_ECS_H

typedef enum COMPONENT_TYPE {
    POSITION,
    ROTATION,
    MAX_COMPONENT_TYPES
} ComponentType;

typedef struct component {
    ComponentType type;
    void *data;
    struct component *next;
} Component;

typedef struct entity {
    int id;
    Component *components;
    struct entity *next;
} Entity;

typedef struct world {
    Entity *entities;
    int lastId;
} World;

World *CreateWorld();
void DestroyWorld(World *);
Entity *AddEntity(World *);
Component *AddComponent(Entity *entity, const ComponentType type, void *data);


#endif //TANKS_ECS_H
