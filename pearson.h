#ifndef PEARSON_H
#define PEARSON_H

#include "structures.h"



    void getUnseenMovies(struct topSimiliarUser *pears, struct unseen ** unseenHead, struct  User toCompare[]);

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

    void insertUnwatched ( struct unseen ** head, 
        int movieId, 
        float rating, 
        float similarity);

#endif