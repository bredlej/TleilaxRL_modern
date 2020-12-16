//
// Created by geoco on 12.11.2020.
//
#include <tleilax/views/galaxy_view.h>

void DescribeStar(Camera *camera, Vector3 *starPositionInWorldCoords,
                  char *starDescription);

void RenderStar(const struct Star *star, Vector3 *starPositionInWorldCoords);

void AdjustCameraView(float *distance, float *horizontalDistance,
                      const float *verticalAngle, const float *horizontalAngle,
                      Camera3D *camera, float amount);

Camera AdjustCameraOnUserInput(Camera *camera, float *distance,
                               float *verticalAngle, float *horizontalAngle,
                               float *horizontalDistance);

void PrepareStarDescription(Coordinates coords, const struct Star *star,
                            char *starDescription);

Camera OnStarClicked(Camera *camera, const Vector3 *starPositionInWorldCoords,
                     const Coordinates coords, struct Star *pStar);

void DrawGalaxy(Camera *camera, Vector3 *cameraInitialPosition,
                float *distance);

void RenderWorld(Vector3 *cameraInitialPosition, Camera *camera,
                 float *distance);

Camera InitializeCamera(Vector3 *cameraInitialPosition, float cameraDistance,
                        float horizontalDistance, float horizontalAngle,
                        float verticalAngle);

Camera InitializeCamera(Vector3 *cameraInitialPosition, float cameraDistance,
                        float horizontalDistance, float horizontalAngle,
                        float verticalAngle) {
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

void RenderWorld(Vector3 *cameraInitialPosition, Camera *camera,
                 float *distance) {
  BeginDrawing();

  ClearBackground(BLACK);
  BeginMode3D((*camera));

  DrawGalaxy(camera, cameraInitialPosition, distance);
  DrawGrid(50, 1.0f);

  EndMode3D();

  char galaxy_offset[50];
  sprintf(galaxy_offset, "Galaxy Offset = [%d, %d, %d]", (int)Galaxy.offset.x,
          (int)Galaxy.offset.y, (int)Galaxy.offset.z);
  DrawText(galaxy_offset, 200, 10, 30, RAYWHITE);
  DrawText("W S A D", 30, 530, 30, RAYWHITE);
  DrawText("Arrow keys, + & -", 30, 560, 30, RAYWHITE);
  DrawText("Click on a star and press enter", 30, 590, 30, RAYWHITE);
  DrawFPS(10, 10);

  EndDrawing();
}

void DrawGalaxy(Camera *camera, Vector3 *cameraInitialPosition,
                float *distance) {
  float xOffset = (float)AMOUNT_SECTORS_X / 2;
  float yOffset = (float)AMOUNT_SECTORS_Y / 2;
  float zOffset = (float)AMOUNT_SECTORS_Z / 2;

  for (uint32_t sectorZ = 0; sectorZ < AMOUNT_SECTORS_Z; sectorZ++) {
    for (uint32_t sectorY = 0; sectorY < AMOUNT_SECTORS_Y; sectorY++) {
      for (uint32_t sectorX = 0; sectorX < AMOUNT_SECTORS_X; sectorX++) {
        Coordinates coords = {sectorX + (int32_t)floorf(Galaxy.offset.x),
                              sectorY + (int32_t)floorf(Galaxy.offset.y),
                              sectorZ + (int32_t)floorf(Galaxy.offset.z)};
        struct Star *starInSectorXYZ = StarAt(coords.x, coords.y, coords.z);
        if (starInSectorXYZ) {

          Vector3 starPositionInWorldCoords = {(float)sectorX - xOffset,
                                               (float)sectorY - yOffset,
                                               (float)sectorZ - zOffset};
          Vector3 starOutlineSize = {starInSectorXYZ->size,
                                     starInSectorXYZ->size,
                                     starInSectorXYZ->size};
          Ray ray = GetMouseRay(GetMousePosition(), (*camera));

          bool isMousePointerOnStar = CheckCollisionRayBox(
              ray,
              (BoundingBox){
                  (Vector3){starPositionInWorldCoords.x - starOutlineSize.x / 2,
                            starPositionInWorldCoords.y - starOutlineSize.y / 2,
                            starPositionInWorldCoords.z -
                                starOutlineSize.z / 2},
                  (Vector3){starPositionInWorldCoords.x + starOutlineSize.x / 2,
                            starPositionInWorldCoords.y + starOutlineSize.y / 2,
                            starPositionInWorldCoords.z +
                                starOutlineSize.z / 2}});

          if (isMousePointerOnStar) {

            DrawSphere(starPositionInWorldCoords, starInSectorXYZ->size,
                       RAYWHITE);
            DrawCubeWires(starPositionInWorldCoords, 1.0f, 1.0f, 1.0f, GREEN);

            EndMode3D();
            char textAboveStar[20];
            PrepareStarDescription(coords, starInSectorXYZ, textAboveStar);
            DescribeStar(camera, &starPositionInWorldCoords, textAboveStar);
            BeginMode3D((*camera));

            (*camera) = OnStarClicked(camera, &starPositionInWorldCoords,
                                      coords, starInSectorXYZ);
          } else {
            RenderStar(starInSectorXYZ, &starPositionInWorldCoords);
          }
          if (starInSectorXYZ != Tleilax.selectedStar) {
            free(starInSectorXYZ);
          }
        }
      }
    }
  }
  if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
    if (Tleilax.selectedCoordinates) {
      free(Tleilax.selectedCoordinates);
      Tleilax.selectedCoordinates = NULL;
    }
    (*camera).target = (*cameraInitialPosition);
    (*distance) = 100.0f;
  }
}

Camera OnStarClicked(Camera *camera, const Vector3 *starPositionInWorldCoords,
                     const struct Coordinates coords, struct Star *pStar) {
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

    (*camera).target = (*starPositionInWorldCoords);
    if (Tleilax.selectedCoordinates) {
      free(Tleilax.selectedCoordinates);
    }
    Coordinates *selectedCoordinates = malloc(sizeof(*selectedCoordinates));
    if (selectedCoordinates) {
      selectedCoordinates->x = coords.x;
      selectedCoordinates->y = coords.y;
      selectedCoordinates->z = coords.z;
      Tleilax.selectedCoordinates = selectedCoordinates;
      Tleilax.selectedStar = pStar;
    }
  }
  return (*camera);
}

void PrepareStarDescription(const Coordinates coords, const struct Star *star,
                            char *starDescription) {
  sprintf(starDescription, "%s [%d, %d, %d]", star->name, coords.x, coords.y,
          coords.z);
}

Camera AdjustCameraOnUserInput(Camera *camera, float *distance,
                               float *verticalAngle, float *horizontalAngle,
                               float *horizontalDistance) {
  if (IsKeyDown(KEY_A)) {
    (*camera).position.x =
        (*horizontalDistance) * cosf((*horizontalAngle) * PI / 180.0f);
    (*camera).position.z =
        (*horizontalDistance) * sinf((*horizontalAngle) * PI / 180.0f);
    (*horizontalAngle) -= 1.0f;
  } else if (IsKeyDown(KEY_D)) {
    (*camera).position.x =
        (*horizontalDistance) * cosf((*horizontalAngle) * PI / 180.0f);
    (*camera).position.z =
        (*horizontalDistance) * sinf((*horizontalAngle) * PI / 180.0f);
    (*horizontalAngle) += 1.0f;
  } else if (IsKeyDown(KEY_W) && (*verticalAngle) < 88.0f) {
    (*horizontalDistance) = (*distance) * cosf((*verticalAngle) * PI / 180.0f);
    (*camera).position.x =
        (*horizontalDistance) * cosf((*horizontalAngle) * PI / 180.0f);
    (*camera).position.z =
        (*horizontalDistance) * sinf((*horizontalAngle) * PI / 180.0f);
    (*camera).position.y = (*distance) * sinf((*verticalAngle) * PI / 180.0f);

    (*verticalAngle) += 1.0f;
  } else if (IsKeyDown(KEY_S) && (*verticalAngle) > 2.0f) {
    (*horizontalDistance) = (*distance) * cosf((*verticalAngle) * PI / 180.0f);
    (*camera).position.x =
        (*horizontalDistance) * cosf((*horizontalAngle) * PI / 180.0f);
    (*camera).position.z =
        (*horizontalDistance) * sinf((*horizontalAngle) * PI / 180.0f);
    (*camera).position.y = (*distance) * sinf((*verticalAngle) * PI / 180.0f);

    (*verticalAngle) -= 1.0f;
  }

  if (IsKeyDown(KEY_RIGHT)) {
    Galaxy.offset.x += Tleilax.config.scrollSpeed;
  } else if (IsKeyDown(KEY_LEFT)) {
    Galaxy.offset.x -= Tleilax.config.scrollSpeed;
  }

  if (IsKeyDown(KEY_UP)) {
    Galaxy.offset.y += Tleilax.config.scrollSpeed;
  } else if (IsKeyDown(KEY_DOWN)) {
    Galaxy.offset.y -= Tleilax.config.scrollSpeed;
  }

  if (IsKeyDown(KEY_KP_ADD)) {
    AdjustCameraView(distance, horizontalDistance, verticalAngle,
                     horizontalAngle, camera, -Tleilax.config.zoomSpeed);
  }
  if (IsKeyDown(KEY_KP_SUBTRACT)) {
    AdjustCameraView(distance, horizontalDistance, verticalAngle,
                     horizontalAngle, camera, Tleilax.config.zoomSpeed);
  }

  if (GetMouseWheelMove() != 0) {
    AdjustCameraView(distance, horizontalDistance, verticalAngle,
                     horizontalAngle, camera,
                     GetMouseWheelMove() * Tleilax.config.zoomSpeed);
  }
  return (*camera);
}

void RenderStar(const struct Star *star, Vector3 *starPositionInWorldCoords) {
  switch (star->type) {
  case STAR_YELLOW:
    DrawSphere((*starPositionInWorldCoords), star->size, YELLOW);
    break;
  case STAR_RED:
    DrawSphere((*starPositionInWorldCoords), star->size, RED);
    break;
  case STAR_BLUE:
    DrawSphere((*starPositionInWorldCoords), star->size, DARKBLUE);
    break;
  case STAR_BROWN:
    DrawSphere((*starPositionInWorldCoords), star->size, BROWN);
    break;
  }
}

void DescribeStar(Camera *camera, Vector3 *starPositionInWorldCoords,
                  char *starDescription) {
  Vector2 starPositionInScreenCoords = GetWorldToScreen(
      (Vector3){(*starPositionInWorldCoords).x, (*starPositionInWorldCoords).y,
                (*starPositionInWorldCoords).z},
      (*camera));
  DrawText(starDescription, (int)starPositionInScreenCoords.x - 70,
           (int)starPositionInScreenCoords.y - 70, 30, RAYWHITE);
}

void AdjustCameraView(float *distance, float *horizontalDistance,
                      const float *verticalAngle, const float *horizontalAngle,
                      Camera3D *camera, float amount) {
  *distance += amount;
  *horizontalDistance = *distance * cosf(*verticalAngle * PI / 180.0f);
  camera->position.x =
      *horizontalDistance * cosf(*horizontalAngle * PI / 180.0f);
  camera->position.z =
      *horizontalDistance * sinf(*horizontalAngle * PI / 180.0f);
  camera->position.y = *distance * sinf(*verticalAngle * PI / 180.0f);
}

void UpdateGalaxyView() {
  GalaxyView.camera = AdjustCameraOnUserInput(
      &GalaxyView.camera, &GalaxyView.cameraDistance, &GalaxyView.verticalAngle,
      &GalaxyView.horizontalAngle, &GalaxyView.horizontalDistance);
}

void RenderGalaxyView() {
  RenderWorld(&GalaxyView.cameraInitialPosition, &GalaxyView.camera,
              &GalaxyView.cameraDistance);
}

void InitGraphics() {
  Vector3 cameraInitialPosition = {0.0f, 0.0f, 0.0f};
  GalaxyView.camera =
      InitializeCamera(&cameraInitialPosition, GalaxyView.cameraDistance,
                       GalaxyView.horizontalDistance,
                       GalaxyView.horizontalAngle, GalaxyView.verticalAngle);
}

void HandleInput(void) {
  if (IsKeyPressed(KEY_TAB)) {
    TLX_ShowIntro(GalaxyView.TleilaxStateMachineObj, NULL);
  }
  if (Tleilax.selectedCoordinates && IsKeyPressed(KEY_ENTER)) {
    Coordinates *coords = SM_XAlloc(sizeof(*coords));
    if (coords) {
      coords->x = Tleilax.selectedCoordinates->x;
      coords->y = Tleilax.selectedCoordinates->y;
      coords->z = Tleilax.selectedCoordinates->z;
    }
    TLX_ShowStarSystem(GalaxyView.TleilaxStateMachineObj, coords);
  }
}

struct GalaxyView GalaxyView = {.Init = InitGraphics,
                                .Render = RenderGalaxyView,
                                .Update = UpdateGalaxyView,
                                .HandleInput = HandleInput};
