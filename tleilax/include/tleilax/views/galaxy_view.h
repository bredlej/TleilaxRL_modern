//
// Created by geoco on 16.11.2020.
//

#ifndef TLEILAX_GALAXY_VIEW_H
#define TLEILAX_GALAXY_VIEW_H

#include <tleilax/tleilax.h>
#include <galaxy/galaxy.h>
#include <math.h>
#include <raylib.h>
#include <stdint.h>

struct GalaxyView {
  void (*Init)(void);
  void (*Update)(void);
  void (*Render)(void);
  void (*HandleInput)(void);

  SM_StateMachine *TleilaxStateMachineObj;
  Camera camera;
  float cameraDistance;
  float verticalAngle;
  float horizontalAngle;
  float horizontalDistance;
  Vector3 cameraInitialPosition;

  Model starModel;
  Texture2D starTexture;
  Shader shader;
};

extern struct GalaxyView GalaxyView;

#endif // TLEILAX_GALAXY_VIEW_H
