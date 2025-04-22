#ifndef COLLABORATIVE_FILTERING_H
#define COLLABORATIVE_FILTERING_H

    #include "structures.h"

    #include <stdio.h>
    #include <stdlib.h>

    void createUsermovieMatrix(
        FILE *usersRateMovie, 
        int user, 
        int numMovies, 
        int numRates);

    void addRating(struct User *user, int id, int rating);


#endif