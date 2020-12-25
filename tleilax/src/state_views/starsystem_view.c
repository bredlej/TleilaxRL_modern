//
// Created by geoco on 12.11.2020.
//
#include <tleilax/views/starsystem_view.h>

Camera StarSystemInitializeCamera(Vector3 *cameraInitialPosition,
                                  float cameraDistance,
                                  float horizontalDistance,
                                  float horizontalAngle, float verticalAngle);

void pushOrbit(const Entity *entity);
Camera StarSystemInitializeCamera(Vector3 *cameraInitialPosition,
                                  float cameraDistance,
                                  float horizontalDistance,
                                  float horizontalAngle, float verticalAngle) {
  Camera camera = {0};
  camera.target = (*cameraInitialPosition);
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.type = CAMERA_PERSPECTIVE;
  camera.position.x = horizontalDistance * cosf(horizontalAngle * PI / 180.0f);
  camera.position.z = horizontalDistance * sinf(horizontalAngle * PI / 180.0f);
  camera.position.y = cameraDistance * sinf(verticalAngle * PI / 180.0f);
  return camera;
}

void StarSystemInit() {
  Vector3 cameraInitialPosition = {0.0f, 0.0f, 0.0f};
  StarSystemView.camera = StarSystemInitializeCamera(
      &cameraInitialPosition, StarSystemView.cameraDistance,
      StarSystemView.horizontalDistance, StarSystemView.horizontalAngle,
      StarSystemView.verticalAngle);
}

uint32_t getIndexOfGC(const unsigned long entityId, const int *indexes,
                      const int amountOfEntries) {
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

void renderNode(GravityCenterNode *pNode) {
  Entity *entity = pNode->entity;
  if (entity) {

    Component *star = GetComponent(entity, STAR);
    if (star) {
      // Vector3 vec = {3.0, 43.0, 40.0};
      Vector3 vec = {0.0, 0.0, 0.0};
      DrawSphere(vec, 1.0f, YELLOW);
    }

    Component *planet = GetComponent(entity, PLANET);
    if (planet) {
      PlanetData *planetData = planet->data;
      Vector3 vec = {0.0, 0.0, 0.0};
      DrawSphere(vec, 0.5f, RED);
      /*
      Vector2 screenCoords = GetWorldToScreen(vec, StarSystemView.camera);
      Matrix m = GetMatrixProjection();
      EndMode3D();

      BeginMode3D(StarSystemView.camera);
      SetMatrixProjection(m);
      */
    }

    Component *moon = GetComponent(entity, MOON);
    if (moon) {
      pushOrbit(entity);
      MoonData *moonData = moon->data;
      Vector3 vec = {0.0, 0.0, 0.0};
      DrawSphere(vec, 0.3f, GRAY);
    }

    Component *spaceStation = GetComponent(entity, SPACE_STATION);
    if (spaceStation) {
      pushOrbit(entity);
      SpaceStationData *spaceStationData = spaceStation->data;
      Vector3 vec = {0.0, 0.0, 0.0};
      DrawSphere(vec, 0.2f, ORANGE);
    }
  }

}
void pushOrbit(const Entity *entity) {
  Component *orbit = GetComponent(entity, ORBIT);
  if (orbit) {
    struct orbitData *orbitData = orbit->data;
    if (orbitData) {
      Vector3 vec = {0.0, 0.0, 0.0};
      Vector3 rot = {1.0f, 0.0f, 0.0f};
      DrawCircle3D(vec, orbitData->distance * 0.2, rot, 90, RAYWHITE);
      rlRotatef(orbitData->degrees, 0.0f, 1.0f, 0.0f);
      rlTranslatef(orbitData->distance * 0.2, 0.0f, 0.0f);
    }
  }
}
GravityCenterNode *
renderStarSystem(GravityCenterNode *rootNode,
                 const GravityCenterTree *gravityCenterTree) {
  rlPushMatrix();
  Component *orbit = GetComponent(rootNode->entity, ORBIT);
  if (orbit) {
    struct orbitData *orbitData = orbit->data;
    if (orbitData) {
      Vector3 vec = {0.0, 0.0, 0.0};
      Vector3 rot = {1.0f, 0.0f, 0.0f};
      DrawCircle3D(vec, orbitData->distance, rot, 90, RAYWHITE);
      rlRotatef(orbitData->degrees, 0.0f, 1.0f, 0.0f);
      rlTranslatef(orbitData->distance, 0.0f, 0.0f);
    }
  }

  renderNode(rootNode);
  if (rootNode->children == NULL) {
    rlPopMatrix();
    return NULL;
  }

  uint32_t childIdx = 0;
  for (; childIdx < rootNode->amountChildren; childIdx++) {
    GravityCenterNode *childNode = rootNode->children[childIdx];
    if (childNode) {
      Component *gravityComponent =
          GetComponent(childNode->entity, GRAVITY_CENTER);
      if (gravityComponent) {

        rlPushMatrix();

        renderStarSystem(gravityCenterTree->gravityCenters[getIndexOfGC(
                             childNode->entity->id, gravityCenterTree->indexes,
                             gravityCenterTree->amountOfGravityCenters)],
                         gravityCenterTree);
        rlPopMatrix();
      } else {
        renderStarSystem(childNode, gravityCenterTree);
      }
    }
  }
  rlPopMatrix();
}

GravityCenterNode *
renderStarSystemNew(GravityCenterNode *rootNode,
                 const GravityCenterTree *gravityCenterTree) {
  rlPushMatrix();
  Component *orbit = GetComponent(rootNode->entity, ORBIT);
  if (orbit) {
    struct orbitData *orbitData = orbit->data;
    if (orbitData) {
      Vector3 vec = {0.0, 0.0, 0.0};
      Vector3 rot = {1.0f, 0.0f, 0.0f};
      DrawCircle3D(vec, orbitData->distance, rot, 90, RAYWHITE);
      rlRotatef(orbitData->degrees, 0.0f, 1.0f, 0.0f);
      rlTranslatef(orbitData->distance, 0.0f, 0.0f);
    }
  }

  renderNode(rootNode);
  if (rootNode->children == NULL) {
    rlPopMatrix();
    return NULL;
  }

  uint32_t childIdx = 0;
  for (; childIdx < rootNode->amountChildren; childIdx++) {
    GravityCenterNode *childNode = rootNode->children[childIdx];
    if (childNode) {
      Component *gravityComponent =
          GetComponent(childNode->entity, GRAVITY_CENTER);
      if (gravityComponent) {

        rlPushMatrix();

        renderStarSystem(gravityCenterTree->gravityCenters[getIndexOfGC(
            childNode->entity->id, gravityCenterTree->indexes,
            gravityCenterTree->amountOfGravityCenters)],
                         gravityCenterTree);
        rlPopMatrix();
      } else {
        renderStarSystem(childNode, gravityCenterTree);
      }
    }
  }
  rlPopMatrix();
}

void UpdateStarSystemView() {
  Entity *entity = Tleilax.starSystem->entities;
  while (entity) {
    Component *orbit = GetComponent(entity, ORBIT);
    if (orbit) {
      struct orbitData *orbitData = orbit->data;
      orbitData->degrees +=
          1.1f +
          1 / (1.0f / orbitData->distance > 0 ? orbitData->distance : 1.0);
      if (orbitData->degrees > 360.0f) {
        orbitData->degrees = 0.0f;
      }
    }
    entity = entity->next;
  }
  if (IsKeyPressed(KEY_TAB)) {
    SM_Event(TleilaxStateMachine, TLX_ShowGalaxy, NULL);
  }
}

void RenderStarSystemView() {
  Vector3 cameraPos = {0, 15, 30};
  StarSystemView.camera.position = cameraPos;
  UpdateCamera(&StarSystemView.camera);
  BeginDrawing();

  ClearBackground(BLACK);
  char coords[20];
  sprintf(coords, "[%d, %d, %d]", Tleilax.selectedCoordinates->x,
          Tleilax.selectedCoordinates->y, Tleilax.selectedCoordinates->z);
  DrawText(coords, 300, 50, 50, RAYWHITE);
  DrawText("Press <Tab> to go back", 30, 580, 30, RAYWHITE);
  GravityCenterTree *gravityCenterTree = Tleilax.gravityCenterTree;


  BeginMode3D(StarSystemView.camera);
  if (gravityCenterTree) {
    renderStarSystemNew(gravityCenterTree->gravityCenters[0], gravityCenterTree);
  }
  EndMode3D();


  EndDrawing();
}

struct StarSystemView StarSystemView = {.Init = StarSystemInit,
                                        .Render = RenderStarSystemView,
                                        .Update = UpdateStarSystemView,
                                        .HandleInput = NULL};