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

struct Graphics {
    void (*Initialize)(const int width, const int height);
    void (*Update)(void);
    void (*Render)(void);
    void (*Destroy)(void);
};

extern struct Graphics Graphics;
#endif //TLEILAX_GRAPHICS_H
