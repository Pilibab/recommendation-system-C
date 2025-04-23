#ifndef PEARSON_H
#define PEARSON_H

#include "structures.h"

#define NEIGHBOR 10

    void getUnseenMovies(struct topSimiliarUser * pears, 
        struct User targetUser, 
        struct User database[]);

    void pearsonCorrelation(struct ratingsTopN * listSameMovies , 
        struct topSimiliarUser * pears,  
        struct User * UserA,
        struct User * UserB,
        int UserB_ID);

    void topNeighboor(struct User indexUser, 
        struct User toCompare[], 
        int user, struct topSimiliarUser * pears);

    void insertPos(struct topSimiliarUser * pears, 
        int UserID, float pScore, 
        struct ratingsTopN * listSameMovies );
    struct ratingsTopN * getRateOfMovie(struct MovieRating *a, struct MovieRating *b);
    void freeLinkedList(struct ratingsTopN * head );

#endif