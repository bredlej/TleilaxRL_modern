#include <random/random.h>
#include "pcg_basic.h"

static pcg32_random_t rng;

uint32_t rnd(void) { return pcg32_random_r(&rng); }

double rndDouble(const double min, const double max) {
    return (rnd() / (double)(0xFFFFFFFF)) * (max - min) + min;
}

uint32_t rndInt(const uint32_t min, const uint32_t max) {
    return (rnd() % (max - min)) + min;
}

uint32_t randomize_seed_xy(const uint32_t x, const uint32_t y) {
    pcg32_srandom_r(&rng, ((x + y) >> 1) * (x + y + 1) + y, ((x + y) >> 1));
    return rnd();
}

uint32_t randomize_seed_xyz(const uint32_t x, const uint32_t y, const uint32_t z) {
    pcg32_srandom_r(&rng, ((x + y) >> 1) * (x + y + 1) + y, ((x + z) >> 1) * (x + z + 1) + z);
    return rnd();
}

struct random Random = {
    .Rnd = rnd,
    .RndDoubleRange = rndDouble,
    .RndIntRange = rndInt,
    .RandomizeSeedXY = randomize_seed_xy,
    .RandomizeSeedXYZ = randomize_seed_xyz
};

