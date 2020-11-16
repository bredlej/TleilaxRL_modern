#ifndef RANDOM_H_
#define RANDOM_H_

#include <stdint.h>

uint32_t rnd(void);

double rndDouble(const double min, const double max);

uint32_t rndInt(const uint32_t min, const uint32_t max);

uint32_t randomize_seed_xy(const uint32_t x, const uint32_t y);

uint32_t randomize_seed_xyz(const uint32_t x, const uint32_t y, const uint32_t z);

struct random {
    uint32_t (*Rnd)(void);
    double (*RndDoubleRange)(const double, const double);
    uint32_t (*RndIntRange)(const uint32_t, const uint32_t);
    uint32_t (*RandomizeSeedXY)(const uint32_t, const uint32_t);
    uint32_t (*RandomizeSeedXYZ)(const uint32_t, const uint32_t, const uint32_t);
};

extern struct random Random;
#endif // !RANDOM_H_
