#ifndef PEARSON_H
#define PEARSON_H

#include "structures.h"

#define NEIGHBOR 10

    // float pearsonCorrelation(struct User *user);
    void topNeighboor(struct User indexUser, struct User toCompare[], int tN[][2], int user);
    void insertPos(int arr[][2], int UserID, int similarCount);


#endif