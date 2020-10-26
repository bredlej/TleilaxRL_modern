#ifndef MYPROJECT_H_
#define MYPROJECT_H_
#define VERSION 0.1
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <galaxy/galaxy.h>
#include <random/random.h>

#define AMOUNT_SECTORS_X 50
#define AMOUNT_SECTORS_Y 50
#define AMOUNT_SECTORS_Z 50

void print_version(void);
int my_library_function(void);

enum STAR_NAME_CLASS {GREEK = 0 , INDIAN = 1};
struct Star* StarAt(uint32_t x, uint32_t y, uint32_t z);

struct TleilaxConfig {
    float zoomSpeed;
    float scrollSpeed;
};

extern struct TleilaxConfig TleilaxConfig;

struct Tleilax {
    void (*Initialize)(void);
    void (*Destroy)(void);
    struct TleilaxConfig config;
    struct StarNames **starNames;
};
extern struct Tleilax Tleilax;
#endif