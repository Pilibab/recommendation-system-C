#ifndef OPTIONS_H
#define OPTIONS_H
    #include "structures.h"

    void displayAllMovies(struct dataSet * movies, int items, int genreCount, char **genres);
    void runCollaborative(struct unseen *listofUnwatched, struct dataSet * movies);
    void showCosineSimilar(struct dataSet * movies, int items);
    void runLogistic(float w[], struct unseen * notWatched, struct User * targetuser);
    void modifyUserData(struct User * targetuser, struct dataSet *movies);

#endif 