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

struct Galaxy Galaxy = {.CreateStar = CreateStar};
