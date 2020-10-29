//
// Created by geoco on 29.10.2020.
//

#ifndef TLEILAX_GRAPHICS_H
#define TLEILAX_GRAPHICS_H

#include <raylib.h>
#include <math.h>
#include <galaxy/galaxy.h>
#include <tleilax/tleilax.h>
#include <stdint.h>

void DescribeStar(Camera *camera, Vector3 *starPositionInWorldCoords,
                  const char *starDescription);

void RenderStar(const struct Star *star, Vector3 *starPositionInWorldCoords);

void AdjustCameraView(float *distance, float *horizontalDistance,
                      const float *verticalAngle, const float *horizontalAngle,
                      Camera3D *camera, float amount);

Camera AdjustCameraOnUserInput(Camera *camera, float *distance, float *verticalAngle, float *horizontalAngle,
                               float *horizontalDistance);

void PrepareStarDescription(float xOffset, float yOffset, float zOffset, uint32_t sectorZ, uint32_t sectorY,
                            uint32_t sectorX, const struct Star *star, const char *starDescription);

Camera OnStarClicked(Camera *camera, Vector3 *starPositionInWorldCoords);

void DrawGalaxy(Camera *camera, Vector3 *cameraInitialPosition, float *distance);

void RenderWorld(Vector3 *cameraInitialPosition, Camera *camera, float *distance);

Camera
InitializeCamera(Vector3 *cameraInitialPosition, float cameraDistance, float horizontalDistance, float horizontalAngle,
                 float verticalAngle);

#endif //TLEILAX_GRAPHICS_H
