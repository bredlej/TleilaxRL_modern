//
// Created by geoco on 16.12.2020.
//

#include "raylib.h"
#include "rlgl.h"
//------------------------------------------------------------------------------------
// Module Functions Declaration
//------------------------------------------------------------------------------------
void DrawSphereBasic(
    Color color); // Draw sphere without any matrix transformation

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight,
             "raylib [models] example - rlgl module usage with push/pop matrix "
             "transformations");

  // Define the camera to look into our 3d world
  Camera camera = {0};
  camera.position = (Vector3){16.0f, 16.0f, 16.0f};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.type = CAMERA_PERSPECTIVE;

  SetCameraMode(camera, CAMERA_FREE);

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    UpdateCamera(&camera);

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);
      Vector3 sphereWorldPos = {0.0f, 0.0f, 0.0f};
      Vector2 firstSphereScreenPos = GetWorldToScreen(sphereWorldPos, camera);
      DrawSphere(sphereWorldPos, 1.0, RED);
    EndMode3D();

    BeginMode3D(camera);
      rlPushMatrix();
        rlTranslatef(0.0f, 5.0f, 0.0f);
        Vector2 secondSphereScreenPos = GetWorldToScreen(sphereWorldPos, camera);
        DrawSphere(sphereWorldPos, 1.0, BLUE);
      rlPopMatrix();
    EndMode3D();

    DrawText("1st", firstSphereScreenPos.x, firstSphereScreenPos.y, 30, BLACK);
    DrawText("    2nd", secondSphereScreenPos.x, secondSphereScreenPos.y, 30, BLACK);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

//--------------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//--------------------------------------------------------------------------------------------
