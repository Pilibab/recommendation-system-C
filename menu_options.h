#ifndef OPTIONS_H
#define OPTIONS_H
    #include "structures.h"

    void displayAllMovies(struct dataSet * movies, int items, int genreCount, char **genres);
    void runCollaborative(struct unseen *listofUnwatched, struct dataSet * movies);
    void showCosineSimilar(struct dataSet * movies, int items);

#endif 