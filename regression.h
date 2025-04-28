#ifndef REGRESSION_C
#define REGRESSION_C

    #include "structures.h"

    struct unseen watched(struct topSimiliarUser * pears, 
        struct unseen ** watchedHead, 
        struct  User toCompare[]);
    
    void epoch( struct unseen ** unseenHead, struct User * targetuser);    
    float z(float w[], float f[]);
#endif