#ifndef MYPROJECT_H_
#define MYPROJECT_H_
#define VERSION 0.1
#include <stdint.h>

void print_version(void);
int my_library_function(void);

uint32_t rnd(void);
uint32_t rndInt(const uint32_t min, const uint32_t max);
uint32_t randomize_seed_xy(const uint32_t x, const uint32_t y);
#endif
