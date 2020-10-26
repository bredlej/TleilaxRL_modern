#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <stdint.h>

struct random {
    uint32_t (*Rnd)(void);
    double (*RndDoubleRange)(const double, const double);
    uint32_t (*RndIntRange)(const uint32_t, const uint32_t);
    uint32_t (*RandomizeSeedXY)(const uint32_t, const uint32_t);
    uint32_t (*RandomizeSeedXYZ)(const uint32_t, const uint32_t, const uint32_t);
};

extern struct random Random;
#endif
