#ifndef PEARSON_H
#define PEARSON_H

#include "structures.h"

#define NEIGHBOR 10

    // float pearsonCorrelation(struct User *user);
    void topNeighboor(struct User indexUser, struct User toCompare[], SimilarUser *similarUsers, int user);
    void insertPos(SimilarUser *similarUsers, int UserID, int countSimilar);
    void getRateOfMovie(struct ratingsTopN *arr, struct User indexUser, struct User toCompare[], SimilarUser *topK);
    void pearsonCorrelation(SimilarUser * topKthN, struct User getUserInfo[], struct User userA );


#endif