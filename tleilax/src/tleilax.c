#include <tleilax/tleilax.h>

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

struct Star* StarAt(uint32_t x, uint32_t y, uint32_t z)
{
	Random.RandomizeSeedXYZ(x, y, z);
	uint32_t starOccurenceChance = Random.RndIntRange(0, 2000);
	if (starOccurenceChance == 1) {
		enum STAR_TYPE starType;
		uint32_t randomType = Random.RndIntRange(0, 100);
		if (randomType < 10)
			starType = STAR_YELLOW;
		else if (randomType >= 10 && randomType < 50)
			starType = STAR_RED;
		else if (randomType >= 50 && randomType < 90) {
            starType = STAR_BLUE;
        }
		else {
		    starType = STAR_BROWN;
		}
		char *starName;
		if (Random.RndIntRange(0,10) < 5) {
            starName = Tleilax.starNames[GREEK]->names[Random.RndIntRange(0, Tleilax.starNames[GREEK]->size)];
		}
		else {
            starName = Tleilax.starNames[INDIAN]->names[Random.RndIntRange(0, Tleilax.starNames[INDIAN]->size)];
		}
		return Galaxy.CreateStar(starName, (float)Random.RndDoubleRange(0.1f, 0.9f), starType);
	}
	else
		return NULL;
}

void init()
{
    struct TleilaxConfig tleilaxConfig = {
            .zoomSpeed = 2.0f,
            .scrollSpeed = 1.0f
    };
    Tleilax.config = tleilaxConfig;
    Tleilax.starNames = malloc(sizeof(struct StarNames *) *  2);
    if (Tleilax.starNames) {
        Tleilax.starNames[GREEK] = LoadNames("scripts/lua/old/names/greek");
        Tleilax.starNames[INDIAN] = LoadNames("scripts/lua/old/names/indian");
    }
}

void destroy()
{
    free(Tleilax.starNames[GREEK]);
    free(Tleilax.starNames[INDIAN]);
    free(Tleilax.starNames);
}

struct Tleilax Tleilax = {
     .Initialize = init,
     .Destroy = destroy
};