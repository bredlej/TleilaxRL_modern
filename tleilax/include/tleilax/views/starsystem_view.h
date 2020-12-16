//
// Created by geoco on 15.12.2020.
//

#ifndef TLEILAX_STARSYSTEM_VIEW_H
#define TLEILAX_STARSYSTEM_VIEW_H

#include <tleilax/tleilax.h>
#include <galaxy/galaxy.h>
#include <math.h>
#include <raylib.h>
#include <rlgl.h>
#include <stdint.h>

struct StarSystemView {
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
};

extern struct StarSystemView StarSystemView;

#endif // TLEILAX_STARSYSTEM_VIEW_H
