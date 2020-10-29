#include <lua_utils/lua_utils.h>
#include <math.h>
#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>
#include <tleilax/tleilax.h>
#include <tleilax/graphics.h>

int main(int argc, char *argv[]) {
    print_version();

    const int screenWidth = 1800;
    const int screenHeight = 1450;
    float cameraDistance = 100.0f;
    float verticalAngle = 45.0f;
    float horizontalAngle = 90.0f;
    float horizontalDistance =
            cameraDistance *
            cosf(verticalAngle * PI / 180.0f);
    Vector3 cameraInitialPosition = {0.0f, 0.0f, 0.0f};
    Camera camera = InitializeCamera(&cameraInitialPosition, cameraDistance, horizontalDistance, horizontalAngle,
                                     verticalAngle);
    Galaxy.offset.x = 0.0f;
    Galaxy.offset.y = 0.0f;
    Galaxy.offset.z = 0.0f;

    Tleilax.Initialize();
    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight,"TleilaxRL");

    while (!WindowShouldClose()) {

        camera = AdjustCameraOnUserInput(&camera, &cameraDistance, &verticalAngle, &horizontalAngle,
                                         &horizontalDistance);
        UpdateCamera(&camera);

        RenderWorld(&cameraInitialPosition, &camera, &cameraDistance);
    }

    CloseWindow(); // Close window and OpenGL context
    Tleilax.Destroy();
    return 0;
}