#include <galaxy/galaxy.h>

struct StarNames *LoadNames(char *fileName) {
    FILE *file;
    errno_t error;

    uint32_t amountNames = 100;
    struct StarNames *starNames = malloc(sizeof *starNames);
    if (error = fopen_s(&file, fileName, "r") != 0) {
        fprintf(stderr, "Cannot open file %s: %s\n", fileName, strerror(error));
    } else {

        char **names = (char **) malloc(sizeof(char *) * amountNames);
        if (!names) {
            fprintf(stderr, "Out of memory.\n");
            exit(3);
        }
        int i;
        for (i = 0; 1; i++) {
            int j;

            if (i >= amountNames) {
                int new_size;

                new_size = amountNames * 2;
                names = (char **) realloc(names, sizeof(char *) * new_size);
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


            for (j = strlen(names[i]) - 1; j >= 0 && (names[i][j] == '\n' || names[i][j] == '\r'); j--);

            names[i][j + 1] = '\0';
            starNames->size = i;
        }
        starNames->names = names;
        fclose(file);
    }

    return starNames;
}

struct Star *CreateStar(char *name, float size, enum STAR_TYPE type) {
    struct Star *newStar = malloc(sizeof *newStar);
    newStar->name = name;
    newStar->size = size;
    newStar->type = type;

    return newStar;
}

struct Galaxy Galaxy = {
        .CreateStar = CreateStar
};