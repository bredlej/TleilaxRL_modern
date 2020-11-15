//
// Created by geoco on 29.10.2020.
//
#include <tleilax/graphics.h>
/*
 * Following includes must be below the include of graphics.h to work.
 * TODO refactor into independent modules
 **/
#include "state_views/starsystem_view.c"
#include "state_views/galaxy_view.c"
#include "state_views/intro_view.c"

void Initialize(const int width, const int height) {

  Tleilax.Initialize();
  /* Start with render and update callbacks of Intro state */

  InitWindow(width, height, "TleilaxRL");
  InitGraphics(100, 100);
  SetTargetFPS(60);
}

void Update() {
  switch (TleilaxUISMObj.currentState) {
  case ST_INTRO:
    UpdateIntro();
    break;
  case ST_GALAXY_VIEW:
    UpdateGalaxyView();
    break;
  case ST_STARSYSTEM_VIEW:
    UpdateStarSystemView();
    break;
  default:
    break;
  }
}

void Render() {
  switch (TleilaxUISMObj.currentState) {
  case ST_INTRO:
    RenderIntro();
    break;
  case ST_GALAXY_VIEW:
    RenderGalaxyView();
    break;
  case ST_STARSYSTEM_VIEW:
    RenderStarSystemView();
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
