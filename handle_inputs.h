#ifndef HANDLE_INPUTS_H
#define HANDLE_INPUTS_H
    #include "structures.h"

    struct User getTargetUserMovies(FILE * file, struct dataSet movies[] );
    int askToInsertData();
    struct MovieRating * insertCorrectPos(struct MovieRating * head, int movieId, int rating);
    int shiftId(int movieId, struct dataSet movies[]);
#endif

