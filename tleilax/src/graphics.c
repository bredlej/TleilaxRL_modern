//
// Created by geoco on 29.10.2020.
//
#include <tleilax/graphics.h>
#include "state_views/galaxy_view.c"
#include "state_views/intro_view.c"

void DestroyGraphics()
{
    CloseWindow();
}

void Initialize(const int width, const int height) {
  /* Start with render and update callbacks of Intro state */
  tleilaxUI.Render = RenderIntro;
  tleilaxUI.Update = UpdateIntro;

  InitWindow(width, height, "TleilaxRL");
  InitGraphics(100, 100);
}

void Render() {
  /* Render current state */
  TleilaxUI *tleilaxUIState = TleilaxUISMObj.pInstance;
  tleilaxUIState->Render();
}

void Update() {
  TleilaxUI *tleilaxUIState = TleilaxUISMObj.pInstance;
  tleilaxUIState->Update();
}

struct Graphics Graphics = {
        .Initialize = Initialize,
        .Update = Update,
        .Render = Render,
        .Destroy = DestroyGraphics
};
