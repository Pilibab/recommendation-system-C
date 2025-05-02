#ifndef HANDLE_INPUTS_C
#define HANDLE_INPUTS_C
    #include "structures.h"

    struct User getTargetUserMovies(FILE * file, struct dataSet movies[] );
    int askToInsertData();
    struct MovieRating * insertCorrectPos(struct MovieRating * head, int movieId, int rating);
    int shiftId(int movieId, struct dataSet movies[]);
#endif

