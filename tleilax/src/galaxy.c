#include <galaxy/galaxy.h>

static struct StarNames *LoadNames(char *fileName) {
  FILE *file = fopen(fileName, "r");
  int error;

  uint32_t amountNames = 100;
  struct StarNames *starNames = malloc(sizeof *starNames);
  if (!file) {
    fprintf(stderr, "Cannot open file %s: %s\n", fileName, strerror(error));
  } else {

    char **names = (char **)malloc(sizeof(char *) * amountNames);
    if (!names) {
      fprintf(stderr, "Out of memory.\n");
      exit(3);
    }
    uint32_t i;
    for (i = 0; 1; i++) {
      uint32_t j;

      if (i >= amountNames) {
        uint32_t new_size;

        new_size = amountNames * 2;
        names = (char **)realloc(names, sizeof(char *) * new_size);
        if (names == NULL) {
          fprintf(stderr, "Out of memory.\n");
          exit(3);
        }
        amountNames = new_size;
      }

      names[i] = malloc(100);
      if (names[i] == NULL) {
        fprintf(stderr, "Out of memory (3).\n");
        exit(4);
      }
      if (fgets(names[i], amountNames - 1, file) == NULL)
        break;

      for (j = strlen(names[i]) - 1;
           j >= 0 && (names[i][j] == '\n' || names[i][j] == '\r'); j--)
        ;

      names[i][j + 1] = '\0';
      starNames->size = i;
    }
    starNames->names = names;
    fclose(file);
  }

  return starNames;
}

static Star *CreateStar(const char *name, const float size,
                 const enum STAR_TYPE type) {
  Star *newStar = malloc(sizeof *newStar);
  newStar->name = name;
  newStar->size = size;
  newStar->type = type;

  return newStar;
}

static Planet *CreatePlanet(const char *name, const enum PLANET_TYPE type,
                     const float distanceFromStar) {
  Planet *newPlanet = malloc(sizeof *newPlanet);
  newPlanet->name = name;
  newPlanet->type = type;
  newPlanet->distanceFromStar = distanceFromStar;
  newPlanet->next = NULL;

  return newPlanet;
}

static StarSystem *CreateStarSystem(const Star *star, const Coordinates *coordinates) {
  StarSystem *newStarSystem = malloc(sizeof(*newStarSystem));
  newStarSystem->star = star;
  newStarSystem->coordinates = coordinates;
  newStarSystem->planets = NULL;

  return newStarSystem;
}

static void DestroyPlanet(Planet *planet) {
  printf("Destroying planet=[%s]\n", planet->name);
  free(planet);
}

static void DestroyStarSystem(StarSystem *starSystem) {
  Planet *planet = starSystem->planets;
  while (planet) {
    Planet *toDelete = planet;
    planet = planet->next;
    DestroyPlanet(toDelete);
  }
  assert(starSystem->star);
  printf("Destroying Star=[%s]\n", starSystem->star->name);
  free(starSystem->star);
  starSystem->star = NULL;
  printf("Destroying StarSystem\n");
  free(starSystem);
}

struct Galaxy Galaxy = {.CreateStar = CreateStar,
                        .CreatePlanet = CreatePlanet,
                        .CreateStarSystem = CreateStarSystem,
                        .DestroyPlanet = DestroyPlanet,
                        .DestroyStarSystem = DestroyStarSystem};
