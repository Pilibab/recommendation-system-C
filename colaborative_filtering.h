#ifndef COLLABORATIVE_FILTERING_H
#define COLLABORATIVE_FILTERING_H

#include "colaborative_filtering.h"
#include "structures.h"

#include <stdio.h>
#include <stdlib.h>

    void createUsermovieMatrix(
        FILE *usersRateMovie, 
        int user, 
        int numMovies, 
        int numRates);

    void addRating(struct User *user, int movieId, int rating);
    void freeRatings(struct User *user);
    void printSampleLinked(struct User *user);


#endif