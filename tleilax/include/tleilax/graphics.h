//
// Created by geoco on 29.10.2020.
//

#ifndef TLEILAX_GRAPHICS_H
#define TLEILAX_GRAPHICS_H

#include <galaxy/galaxy.h>
#include <math.h>
#include <raylib.h>
#include <stdint.h>
#include <tleilax/tleilax.h>

void UpdateGalaxyView(void);
void RenderGalaxyView(void);
void UpdateStarSystemView(void);
void RenderStarSystemView(void);
void UpdateIntro(void);
void RenderIntro(void);

struct Graphics {
  void (*Initialize)(const int width, const int height);
  void (*Update)(void);
  void (*Render)(void);
  void (*Destroy)(void);
};

extern struct Graphics Graphics;

#endif // TLEILAX_GRAPHICS_H
