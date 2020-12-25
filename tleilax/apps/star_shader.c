//
// Created by geoco on 18.12.2020.
//

#include "raylib.h"
#include "rlgl.h"
#include <stdio.h>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight,
             "Star shader");

  // Define the camera to look into our 3d world
  Camera camera = {0};
  camera.position = (Vector3){0.0f, 10.0f, 0.0f};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.type = CAMERA_PERSPECTIVE;

  Texture2D starTexture = LoadTexture("assets/textures/sun.png");
  RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
  Model starModel = LoadModelFromMesh(GenMeshSphere(1.0f, 32, 32));
  Shader starShader = LoadShader(0, TextFormat("assets/shaders/starShader.frag"));

  int u_timeLoc = GetShaderLocation(starShader, "u_time");
  int u_resolutionLoc = GetShaderLocation(starShader, "u_resolution");
  Vector2 resolution = {screenWidth, screenHeight};

  SetShaderValue(starShader, u_resolutionLoc, &resolution, UNIFORM_VEC2);
  starModel.materials[0].maps[MAP_DIFFUSE].texture = target.texture;

  SetCameraMode(camera, CAMERA_FREE);

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  double deg = 0.0f;
  Vector3 sphereWorldPos = {0.0f, 0.0f, 0.0f};
  Vector3 sphereWorldPos2 = {0.0f, 0.0f, 5.0f};
  Vector3 rotation = {0.0f, 1.0f, 0.0f};
  float elapsedTime = 0.0f;
  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    elapsedTime += GetFrameTime();

    //----------------------------------------------------------------------------------
    UpdateCamera(&camera);

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(BLACK);

    BeginTextureMode(target);
    ClearBackground(BLACK);
    BeginShaderMode(starShader);
    DrawRectangle(0,0, screenWidth, screenHeight, WHITE);
    EndTextureMode();
    EndShaderMode();


    BeginMode3D(camera);

    //DrawBillboard(camera, target.texture, sphereWorldPos, 1.0f, WHITE);
    DrawModel(starModel, sphereWorldPos, 1.0f, WHITE);
    DrawBillboard(camera, target.texture, sphereWorldPos2, 2.0f, RED);
    EndMode3D();


    //EndBlendMode();


    if (elapsedTime > 3.14f) {
      elapsedTime = 0.0f;
    }
    EndDrawing();
    //----------------------------------------------------------------------------------

    SetShaderValue(starShader, u_timeLoc, &elapsedTime, UNIFORM_FLOAT);
  }

  UnloadRenderTexture(target);
  UnloadTexture(starTexture);
  UnloadModel(starModel);
  UnloadShader(starShader);
  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

//--------------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//--------------------------------------------------------------------------------------------
