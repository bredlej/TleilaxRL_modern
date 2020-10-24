#include <tleilax/tleilax.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pcg_basic.h>

pcg32_random_t rng;

uint32_t rnd(void) { return pcg32_random_r(&rng); }

uint32_t rndInt(const uint32_t min, const uint32_t max)
{
	return (rnd() % (max - min)) + min;
}

uint32_t randomize_seed_xy(const uint32_t x, const uint32_t y)
{
	pcg32_srandom_r(&rng, ((x + y) >> 1) * (x + y + 1) + y, ((x + y) >> 1));
	
	return rnd();
}

void print_version(void)
{
	printf("Myproject (version %0.2f)\n", VERSION);
}

/*
	Function used for initial unit test of this library.
	See ../tests/test_mylibrary.c"
*/
int my_library_function(void)
{
	return 1;
}
