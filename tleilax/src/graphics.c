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
}

void Destroy() {
  Tleilax.Destroy();
  CloseWindow();
}

struct Graphics Graphics = {.Initialize = Initialize,
                            .Update = Update,
                            .Render = Render,
                            .Destroy = Destroy,
                            };
