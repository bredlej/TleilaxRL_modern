#ifndef _GALAXY_H_
#define _GALAXY_H_
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum STAR_TYPE { STAR_RED, STAR_YELLOW, STAR_BROWN, STAR_BLUE };

struct StarNames {
    char **names;
    size_t size;
};

struct StarNames *LoadNames(char *fileName);

struct Star {
	char *name;
	float size;
	enum STAR_TYPE type;
};

struct Galaxy
{
	struct offset {
		float x;
		float y;
		float z;
	} offset;
	struct Star* (*CreateStar)(const char*, const float, const enum STAR_TYPE);
};

extern struct Galaxy Galaxy;
#endif // !_GALAXY_H_
