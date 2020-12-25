//
// Created by geoco on 29.10.2020.
//
#include <tleilax/graphics.h>
#include <tleilax/views/galaxy_view.h>

/*
 * Following includes must be below the include of graphics.h to work.
 * TODO refactor into independent modules
 **/
#include "state_views/starsystem_view.c"
#include "state_views/intro_view.c"

void Initialize(const int width, const int height) {

  Tleilax.Initialize();
  /* Start with render and update callbacks of Intro state */

  InitWindow(width, height, "TleilaxRL");
  GalaxyView.cameraDistance = 100.0f;
  GalaxyView.verticalAngle = 45.0f;
  GalaxyView.horizontalAngle = 90.0f;
  GalaxyView.horizontalDistance = GalaxyView.cameraDistance * cosf(GalaxyView.verticalAngle * PI / 180.0f);
  GalaxyView.TleilaxStateMachineObj = &TleilaxStateMachineObj;
  GalaxyView.Init();
  StarSystemView.Init();

  Graphics.framebuffer = LoadRenderTexture(width, height);
  Graphics.fbPostProcessingShader = LoadShader(0, "assets/shaders/tleilax_postprocessing.frag");
  Graphics.u_timeLoc = GetShaderLocation(Graphics.fbPostProcessingShader, "time");
  SetTargetFPS(60);
}

void Update() {
  switch (TleilaxStateMachineObj.currentState) {
  case ST_INTRO:
    UpdateIntro();
    break;
  case ST_GALAXY_VIEW:
    GalaxyView.HandleInput();
    GalaxyView.Update();
    break;
  case ST_STARSYSTEM_VIEW:
    //UpdateStarSystemView();
    StarSystemView.Update();
    break;
  default:
    break;
  }
}

void Render() {
  Graphics.elapsedTime += GetFrameTime();
  SetShaderValue(Graphics.fbPostProcessingShader, Graphics.u_timeLoc, &Graphics.elapsedTime, UNIFORM_FLOAT);
  BeginTextureMode(Graphics.framebuffer);
  switch (TleilaxStateMachineObj.currentState) {
  case ST_INTRO:
    RenderIntro();
    break;
  case ST_GALAXY_VIEW:
    GalaxyView.Render();
    break;
  case ST_STARSYSTEM_VIEW:
    //RenderStarSystemView();
    StarSystemView.Render();
    break;
  default:
    break;
  }
  EndTextureMode();
  ClearBackground(BLACK);
  BeginShaderMode(Graphics.fbPostProcessingShader);
  DrawTextureRec(Graphics.framebuffer.texture, (Rectangle){ 0, 0, Graphics.framebuffer.texture.width, -Graphics.framebuffer.texture.height }, (Vector2){ 0, 0 }, WHITE);
  EndShaderMode();
}

void Destroy() {
  Tleilax.Destroy();
  UnloadRenderTexture(Graphics.framebuffer);
  UnloadShader(Graphics.fbPostProcessingShader);
  CloseWindow();
}

struct Graphics Graphics = {.Initialize = Initialize,
                            .Update = Update,
                            .Render = Render,
                            .Destroy = Destroy,
                            };
