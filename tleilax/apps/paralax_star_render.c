//
// Created by Patryk Szczypień on 27/12/2020.
//

#include <random/random.h>
#include <raylib.h>
#include <rlgl.h>

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_RICONS
#include <raygui.h>

#undef RAYGUI_IMPLEMENTATION
#include <stdlib.h>
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#define AMOUNT_SPHERES 256
#define GLSL_VERSION 330

typedef struct {
  Camera camera;
  struct {
    float x;
    float y;
  } offset;
  Matrix *starsForeground;
  Matrix *starsBackgroundPlane1;
  Matrix *starsBackgroundPlane2;
  int amountStarsForeground;
  int amountStarsBackgroundPlane1;
  int amountStarsBackgroundPlane2;
} View;

View drawGUI(View *view);
View initialize() {
  View view;

  Vector3 cameraPosition = {0.0f, 17.0f, 0.1f};
  Vector3 cameraLookAt = {0.0f, 0.0f, 0.0f};
  Camera camera = {0};
  camera.type = CAMERA_PERSPECTIVE;
  camera.position = cameraPosition;
  camera.target = cameraLookAt;
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 60;

  view.camera = camera;

  return view;
}

int populateStars(View *view) {
  view->starsForeground = RL_CALLOC(AMOUNT_SPHERES, sizeof(Matrix));
  view->starsBackgroundPlane1 = RL_CALLOC(AMOUNT_SPHERES, sizeof(Matrix));
  view->starsBackgroundPlane2 = RL_CALLOC(AMOUNT_SPHERES, sizeof(Matrix));

  if (view->starsForeground && view->starsBackgroundPlane1 &&
      view->starsBackgroundPlane2) {
    view->amountStarsForeground = 0;
    view->amountStarsBackgroundPlane1 = 0;
    view->amountStarsBackgroundPlane2 = 0;

    int sectorY = 0;
    int sectorX = 0;

    Matrix rotationsForeground[AMOUNT_SPHERES];
    Matrix rotationsIncForeground[AMOUNT_SPHERES];
    Matrix translationsForeground[AMOUNT_SPHERES];
    Matrix rotationsBackgroundPlane1[AMOUNT_SPHERES];
    Matrix rotationsIncBackgroundPlane1[AMOUNT_SPHERES];
    Matrix translationsBackgroundPlane1[AMOUNT_SPHERES];
    Matrix rotationsBackgroundPlane2[AMOUNT_SPHERES];
    Matrix rotationsIncBackgroundPlane2[AMOUNT_SPHERES];
    Matrix translationsBackgroundPlane2[AMOUNT_SPHERES];

    int indexForeground = 0;
    int indexBackgroundPlane1 = 0;
    int indexBackgroundPlane2 = 0;


    /* Divide screen into 32x32 sectors */
    for (sectorY = 0; sectorY < 32; sectorY++) {
      for (sectorX = 0; sectorX < 32; sectorX++) {

        /* Calculate probability of foreground star occurence */
        Random.RandomizeSeedXY(sectorX + (int32_t)view->offset.x,
                               sectorY + (int32_t)view->offset.y);
        bool isStarAtXY = Random.RndIntRange(0, 256) == 1;

        if (isStarAtXY) {
          translationsForeground[indexForeground] =
              MatrixTranslate(sectorX - 16.0f, 1.0f, sectorY - 16.0f);
          float x = GetRandomValue(0, 360);
          float y = GetRandomValue(0, 360);
          float z = GetRandomValue(0, 360);
          Vector3 axis = Vector3Normalize((Vector3){x, y, z});
          float angle = (float)GetRandomValue(0, 10) * DEG2RAD;

          rotationsIncForeground[indexForeground] = MatrixRotate(axis, angle);
          rotationsForeground[indexForeground] = MatrixIdentity();
          indexForeground = indexForeground + 1;
          view->amountStarsForeground += 1;
        }

        Random.RandomizeSeedXY(sectorX + (int32_t)(view->offset.x*0.5f),
                              sectorY + (int32_t)(view->offset.y*0.5f));
        bool isStarAtXYBackgroundPlane1 = Random.RndIntRange(0, 128) == 1;
        if (isStarAtXYBackgroundPlane1) {
          translationsBackgroundPlane1[indexBackgroundPlane1] =
              MatrixTranslate(sectorX - 16.0f, -10.0f, sectorY - 16.0f);
          float x = GetRandomValue(0, 360);
          float y = GetRandomValue(0, 360);
          float z = GetRandomValue(0, 360);
          Vector3 axis = Vector3Normalize((Vector3){x, y, z});
          float angle = (float)GetRandomValue(0, 10) * DEG2RAD;

          rotationsIncBackgroundPlane1[indexBackgroundPlane1] = MatrixRotate(axis, angle);
          rotationsBackgroundPlane1[indexBackgroundPlane1] = MatrixIdentity();
          indexBackgroundPlane1 += 1;
          view->amountStarsBackgroundPlane1 += 1;
        }

        Random.RandomizeSeedXY(sectorX + (int32_t)(view->offset.x*0.1f),
                               sectorY + (int32_t)(view->offset.y*0.1f));
        bool isStarAtXYBackgroundPlane2 = Random.RndIntRange(0, 64) == 1;
        if (isStarAtXYBackgroundPlane2) {
          translationsBackgroundPlane2[indexBackgroundPlane2] =
              MatrixTranslate(sectorX - 16.0f, -20.0f, sectorY - 16.0f);
          float x = GetRandomValue(0, 360);
          float y = GetRandomValue(0, 360);
          float z = GetRandomValue(0, 360);
          Vector3 axis = Vector3Normalize((Vector3){x, y, z});
          float angle = (float)GetRandomValue(0, 10) * DEG2RAD;

          rotationsIncBackgroundPlane2[indexBackgroundPlane2] = MatrixRotate(axis, angle);
          rotationsBackgroundPlane2[indexBackgroundPlane2] = MatrixIdentity();
          indexBackgroundPlane2 += 1;
          view->amountStarsBackgroundPlane2 += 1;
        }
      }
    }

    for (int i = 0; i < view->amountStarsForeground; i++) {
      rotationsForeground[i] = MatrixMultiply(rotationsForeground[i], rotationsIncForeground[i]);
      view->starsForeground[i] = MatrixMultiply(rotationsForeground[i], translationsForeground[i]);
    }

    for (int i = 0; i < view->amountStarsBackgroundPlane1; i++) {
      rotationsBackgroundPlane1[i] = MatrixMultiply(rotationsBackgroundPlane1[i], rotationsIncBackgroundPlane1[i]);
      view->starsBackgroundPlane1[i] = MatrixMultiply(rotationsBackgroundPlane1[i], translationsBackgroundPlane1[i]);
      view->starsBackgroundPlane1[i] = MatrixMultiply(view->starsBackgroundPlane1[i], MatrixScale(0.9f, 0.9f, 0.9f));
    }
    for (int i = 0; i < view->amountStarsBackgroundPlane2; i++) {
      rotationsBackgroundPlane2[i] = MatrixMultiply(rotationsBackgroundPlane2[i], rotationsIncBackgroundPlane2[i]);
      view->starsBackgroundPlane2[i] = MatrixMultiply(rotationsBackgroundPlane2[i], translationsBackgroundPlane2[i]);
      view->starsBackgroundPlane2[i] = MatrixMultiply(view->starsBackgroundPlane2[i], MatrixScale(0.8f, 0.8f, 0.8f));
    }
  }

  return 0;
}

int main() {

  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 1600;
  const int screenHeight = 1200;

  InitWindow(screenWidth, screenHeight, "Paralax stars");

  Color palette[4] = {(Color){59, 39, 86, 255}, (Color){255, 148, 114, 255},
                      (Color){19, 202, 145, 255}, (Color){232, 71, 174, 255}};

  View view = initialize();
  populateStars(&view);

  Shader shader = LoadShader(
      FormatText("assets/shaders/base_lighting_instanced.vs", GLSL_VERSION),
      FormatText("assets/shaders/lighting.fs", GLSL_VERSION));

  // Get some shader loactions
  shader.locs[LOC_MATRIX_MVP] = GetShaderLocation(shader, "mvp");
  shader.locs[LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
  shader.locs[LOC_MATRIX_MODEL] = GetShaderLocationAttrib(shader, "instance");

  // Ambient light level
  int ambientLoc = GetShaderLocation(shader, "ambient");
  SetShaderValue(shader, ambientLoc, (float[4]){0.2f, 0.2f, 0.2f, 1.0f},
                 UNIFORM_VEC4);
  CreateLight(LIGHT_DIRECTIONAL, (Vector3){50, 50, 0}, Vector3Zero(), WHITE,
              shader);

  Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
  Material yellowStarMaterial = LoadMaterialDefault();
  yellowStarMaterial.shader = shader;
  yellowStarMaterial.maps[MAP_DIFFUSE].color = palette[1];

  Material roseStarMaterial = LoadMaterialDefault();
  roseStarMaterial.shader = shader;
  roseStarMaterial.maps[MAP_DIFFUSE].color = palette[3];

  Material violetStarMaterial = LoadMaterialDefault();
  violetStarMaterial.shader = shader;
  violetStarMaterial.maps[MAP_DIFFUSE].color = palette[0];

  SetTargetFPS(60);
  RenderTexture2D framebuffer = LoadRenderTexture(screenWidth, screenHeight);

  while (!WindowShouldClose()) {
    UpdateCamera(&view.camera);
    float cameraPos[3] = {view.camera.position.x, view.camera.position.y,
                          view.camera.position.z};
    SetShaderValue(shader, shader.locs[LOC_VECTOR_VIEW], cameraPos,
                   UNIFORM_VEC3);

    BeginDrawing();
    ClearBackground(BLACK);
    BeginTextureMode(framebuffer);
    BeginMode3D(view.camera);
    ClearBackground(BLACK);

    rlDrawMeshInstanced(cubeMesh, yellowStarMaterial, view.starsForeground,
                        view.amountStarsForeground);
    rlDrawMeshInstanced(cubeMesh, roseStarMaterial, view.starsBackgroundPlane1,
                        view.amountStarsBackgroundPlane1);
    rlDrawMeshInstanced(cubeMesh, violetStarMaterial, view.starsBackgroundPlane2,
                        view.amountStarsBackgroundPlane2);

    EndMode3D();
    EndTextureMode();

    DrawTextureRec(framebuffer.texture,
                   (Rectangle){0, 0, framebuffer.texture.width,
                               -framebuffer.texture.height},
                   (Vector2){0, 0}, WHITE);

    view = drawGUI(&view);

    EndDrawing();
  }
  CloseWindow();

  RL_FREE(view.starsForeground);
  RL_FREE(view.starsBackgroundPlane1);
  RL_FREE(view.starsBackgroundPlane2);

  return 0;
}

View drawGUI(View *view) {
  (*view).camera.position.x =
      GuiSlider((Rectangle){50, 50, 165, 20}, "Camera.x",
                TextFormat("%2.2f", (*view).camera.position.x),
                (*view).camera.position.x, -50, 100);
  (*view).camera.position.y =
      GuiSlider((Rectangle){50, 80, 165, 20}, "Camera.y",
                TextFormat("%2.2f", (*view).camera.position.y),
                (*view).camera.position.y, -50, 100);
  (*view).camera.position.z =
      GuiSlider((Rectangle){50, 110, 165, 20}, "Camera.z",
                TextFormat("%2.2f", (*view).camera.position.z),
                (*view).camera.position.z, -50, 100);

  if (GuiButton((Rectangle){300, 50, 30, 30}, "UP")) {
    (*view).offset.y -= 1;
    populateStars(view);
  }
  if (GuiButton((Rectangle){250, 80, 30, 30}, "LEFT")) {
    (*view).offset.x -= 1;
    populateStars(view);
  }
  if (GuiButton((Rectangle){350, 80, 30, 30}, "RIGHT")) {
    (*view).offset.x += 1;
    populateStars(view);
  }
  if (GuiButton((Rectangle){300, 110, 30, 30}, "DOWN")) {
    (*view).offset.y += 1;
    populateStars(view);
  }
  DrawFPS(20, 20);
  return (*view);
}
