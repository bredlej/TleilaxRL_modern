//
// Created by geoco on 17.12.2020.
//

#include "raylib.h"
#include "rlgl.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight,
             "Render to texture playground");

  // Define the camera to look into our 3d world
  Camera camera = {0};
  camera.position = (Vector3){16.0f, 16.0f, 16.0f};
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.type = CAMERA_PERSPECTIVE;

  Texture2D starTexture = LoadTexture("assets/textures/sun.png");
  RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
  Model starModel = LoadModelFromMesh(GenMeshSphere(1.0f, 32, 32));

  Shader bloomShader = LoadShader(0, TextFormat("assets/shaders/bloom.fs"));
  Shader blurShader = LoadShader(0, TextFormat("assets/shaders/blur.fs"));
  Shader scanlineShader = LoadShader(TextFormat("assets/shaders/scan.vert"), TextFormat("assets/shaders/scan.frag"));
  Shader grayscaleShader = LoadShader(0, TextFormat("assets/shaders/grayscale.fs"));

  starModel.materials[0].maps[MAP_DIFFUSE].texture = starTexture;

  SetCameraMode(camera, CAMERA_FREE);

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  double deg = 0.0f;
  Vector3 sphereWorldPos = {0.0f, 0.0f, 0.0f};
  Vector3 rotation = {0.0f, 1.0f, 0.0f};

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

    BeginTextureMode(target);
    ClearBackground(BLACK);
    BeginMode3D(camera);

    rlPushMatrix();
    rlRotatef(deg, rotation.x, rotation.y, rotation.z);
    DrawModel(starModel, sphereWorldPos, 1.0f, WHITE);
    deg+=15.0f;
    if (deg > 360.0f) deg = 0.0f;
    rlPopMatrix();
    EndMode3D();
    EndTextureMode();


    BeginShaderMode(blurShader);

    DrawTextureRec(target.texture, (Rectangle){ 0, 0, target.texture.width, -target.texture.height }, (Vector2){ 0, 0 }, WHITE);
    EndShaderMode();

    //EndBlendMode();


    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  UnloadRenderTexture(target);
  UnloadTexture(starTexture);
  UnloadModel(starModel);
  UnloadShader(bloomShader);
  UnloadShader(scanlineShader);
  UnloadShader(blurShader);
  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

//--------------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//--------------------------------------------------------------------------------------------
