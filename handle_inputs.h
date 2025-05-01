#ifndef HANDLE_INPUTS_C
#define HANDLE_INPUTS_C
    #include "structures.h"

    struct User getTargetUserMovies(FILE * file );
    int askToInsertData();
    struct MovieRating * insertCorrectPos(struct MovieRating * head, int movieId, int rating);
#endif

