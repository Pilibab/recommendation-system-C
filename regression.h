#ifndef REGRESSION_C
#define REGRESSION_C

    #include "structures.h"

    struct unseen watched(struct topSimiliarUser * pears, 
        struct unseen ** watchedHead, 
        struct  User toCompare[]);
    
    void epoch( struct unseen ** unseenHead, struct User * targetuser, int epochCount, float w[]);    
    float z(float w[], float f[]);
    void updateWeights(float w[], float f[], float alpha, float y_hat, float y);

#endif