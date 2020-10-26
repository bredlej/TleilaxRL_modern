#include <lua_utils/lua_utils.h>
#include <math.h>
#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>
#include <tleilax/tleilax.h>

void DescribeStar(Camera *camera, Vector2 *starPositionInScreenCoords, Vector3 *starPositionInWorldCoords,
                  const char *starDescription);

void RenderStar(const struct Star *star, Vector3 *starPositionInWorldCoords);

void AdjustCameraView(float *distance, float *horizontalDistance,
                      const float *verticalAngle, const float *horizontalAngle,
                      Camera3D *camera, float amount);

Camera ChangeCameraOnUserInput(Camera *camera, float *distance, float *verticalAngle, float *horizontalAngle,
                               float *horizontalDistance);

void PrepareStarDescription(float xOffset, float yOffset, float zOffset, uint32_t sectorZ, uint32_t sectorY,
                            uint32_t sectorX, const struct Star *star, const char *starDescription);

Camera OnStarClicked(Camera *camera, Vector3 *starPositionInWorldCoords);

void InitCamera(Vector3 *cameraInitialPosition, Camera *camera, float *distance, float *verticalAngle,
                float *horizontalAngle, float *horizontalDistance);

int main(int argc, char *argv[]) {
    print_version();

    Tleilax.Initialize();

    const int screenWidth = 1800;
    const int screenHeight = 1450;

    InitWindow(screenWidth, screenHeight,"TleilaxRL");

    Camera camera;
    Vector3 cameraInitialPosition;
    float distance;
    float verticalAngle;
    float horizontalAngle;
    float horizontalDistance;
    InitCamera(&cameraInitialPosition, &camera, &distance, &verticalAngle, &horizontalAngle, &horizontalDistance);

    Ray ray = {0};
    bool mouseHoverOnStar = false;

    Vector3 cubePosition = {0.0f, 0.0f, 20.0f};
    Vector2 starPositionInScreenCoords = {0.0f, 0.0f};

    Galaxy.offset.x = 0.0f;
    Galaxy.offset.y = 0.0f;
    Galaxy.offset.z = 0.0f;
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        camera = ChangeCameraOnUserInput(&camera, &distance, &verticalAngle, &horizontalAngle, &horizontalDistance);

        UpdateCamera(&camera); // Update camera
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);

        float xOffset = (float) AMOUNT_SECTORS_X / 2;
        float yOffset = (float) AMOUNT_SECTORS_Y / 2;
        float zOffset = (float) AMOUNT_SECTORS_Z / 2;

        for (uint32_t sectorZ = 0; sectorZ < AMOUNT_SECTORS_Z; sectorZ++) {
            for (uint32_t sectorY = 0; sectorY < AMOUNT_SECTORS_Y; sectorY++) {
                for (uint32_t sectorX = 0; sectorX < AMOUNT_SECTORS_X; sectorX++) {
                    struct Star *star = StarAt(sectorX + floorf(Galaxy.offset.x),
                                               sectorY + floorf(Galaxy.offset.y),
                                               sectorZ + floorf(Galaxy.offset.z));
                    if (star) {

                        Vector3 starPositionInWorldCoords = {(float) sectorX - xOffset,
                                                             (float) sectorY - yOffset,
                                                             (float) sectorZ - zOffset};
                        Vector3 starOutlineSize = {star->size, star->size, star->size};
                        ray = GetMouseRay(GetMousePosition(), camera);

                        mouseHoverOnStar = CheckCollisionRayBox(
                                ray,
                                (BoundingBox) {
                                        (Vector3) {
                                                starPositionInWorldCoords.x - starOutlineSize.x / 2,
                                                starPositionInWorldCoords.y - starOutlineSize.y / 2,
                                                starPositionInWorldCoords.z - starOutlineSize.z / 2},
                                        (Vector3) {
                                                starPositionInWorldCoords.x + starOutlineSize.x / 2,
                                                starPositionInWorldCoords.y + starOutlineSize.y / 2,
                                                starPositionInWorldCoords.z + starOutlineSize.z / 2}});

                        if (mouseHoverOnStar) {

                            DrawSphere(starPositionInWorldCoords, star->size, RAYWHITE);
                            DrawCubeWires(starPositionInWorldCoords, 1.0f, 1.0f, 1.0f, GREEN);

                            EndMode3D();
                            char starDescription[20];
                            PrepareStarDescription(xOffset, yOffset, zOffset, sectorZ, sectorY, sectorX, star,
                                                   starDescription);
                            DescribeStar(&camera, &starPositionInScreenCoords, &starPositionInWorldCoords,
                                         starDescription);
                            BeginMode3D(camera);

                            camera = OnStarClicked(&camera, &starPositionInWorldCoords);
                        } else {
                            RenderStar(star, &starPositionInWorldCoords);
                        }
                        free(star);
                    }
                }
            }
        }
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            camera.target = cameraInitialPosition;
            distance = 100.0f;
        }

        DrawGrid(50, 1.0f);
        EndMode3D();
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
    Tleilax.Destroy();
    return 0;
}

void InitCamera(Vector3 *cameraInitialPosition, Camera *camera, float *distance, float *verticalAngle,
                float *horizontalAngle, float *horizontalDistance) {
    (*cameraInitialPosition) = {0.0f, 0.0f, 0.0f};
    (*camera) = {0};
    (*distance) = 100.0f;
    (*verticalAngle) = 45.0f;
    (*horizontalAngle) = 90.0f;
    (*horizontalDistance) = distance *
                            cosf(verticalAngle * PI / 180.0f);// Define the camera to look into our 3d world
    (*camera).position = (Vector3) {50.0f, 50.0f, 50.0f};
    (*camera).target = (*cameraInitialPosition);
    (*camera).up = (Vector3) {0.0f, 1.0f, 0.0f};
    (*camera).fovy = 45.0f;
    (*camera).type = CAMERA_PERSPECTIVE;// Set the distance from camera.target to camera.position
// The Y/XZ angle
// The X/Z angle, y is considered as 0
// Horizontal distance, the magnitude

    (*camera).position.x = (*horizontalDistance) *
                           cosf((*horizontalAngle) * PI /
                                180.0f); // Calculate the position of camera.position
// x based on distance etc..
    (*camera).position.z = (*horizontalDistance) * sinf((*horizontalAngle) * PI / 180.0f);
    (*camera).position.y = (*distance) * sinf((*verticalAngle) * PI / 180.0f);
}

Camera OnStarClicked(Camera *camera, Vector3 *starPositionInWorldCoords) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        (*camera).target = (*starPositionInWorldCoords);
    }
    return (*camera);
}

void PrepareStarDescription(float xOffset, float yOffset, float zOffset, uint32_t sectorZ, uint32_t sectorY,
                            uint32_t sectorX, const struct Star *star, const char *starDescription) {
    sprintf(starDescription, "%s [%d, %d, %d]",
star->name,
            sectorX - (uint32_t) xOffset +
            (uint32_t) floorf(Galaxy.offset.x),
            sectorY - (uint32_t) yOffset +
            (uint32_t) floorf(Galaxy.offset.y),
            sectorZ - (uint32_t) zOffset +
            (uint32_t) floorf(Galaxy.offset.z));
}

Camera ChangeCameraOnUserInput(Camera *camera, float *distance, float *verticalAngle, float *horizontalAngle,
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

void DescribeStar(Camera *camera, Vector2 *starPositionInScreenCoords, Vector3 *starPositionInWorldCoords,
                  const char *starDescription) {
    (*starPositionInScreenCoords) =
            GetWorldToScreen((Vector3) {(*starPositionInWorldCoords).x,
                                        (*starPositionInWorldCoords).y,
                                        (*starPositionInWorldCoords).z},
                             (*camera));
    DrawText(starDescription, (int) (*starPositionInScreenCoords).x - 70,
             (int) (*starPositionInScreenCoords).y - 70,
             30, RAYWHITE);
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
