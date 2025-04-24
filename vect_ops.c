#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

//include header file for declaration 
#include "vect_ops.h"
#include "structures.h"
#include "structures.h"


/**
 * returns single int -> summation of element wise vector multiplication A[i] * B[j]
 */
float dotProduct(struct dataSet movie, struct dataSet toCompare)
{
    float productDot = 0.0;

    for (int i = 0; i < GENRESIZE; i ++)
        productDot += movie.weightedFeature[i] * toCompare.weightedFeature[i];
    
    return productDot; 
}


float magnitude(struct dataSet movie)
{
    float sum = 0.0; 
    for (int i = 0; i < GENRESIZE; i ++)
        sum += movie.weightedFeature[i] * movie.weightedFeature[i];

    return sqrt(sum);
}

/**
 * A.B / (||A||.||B||)
 * 
 * how similar vectors are 0 -> no similarities 1-> similar 0<x<1 -> somewhat similar
 */
float cosineSimilarity(struct dataSet movie, struct dataSet toCompare)
{
    float productDot, magA, magB; 
    productDot = dotProduct(movie, toCompare);
    magA = magnitude(movie);
    magB = magnitude(toCompare);

    if (magA <= 0.0 || magB <= 0.0)
        return 0.0;

    return productDot / magA /  magB;
}
