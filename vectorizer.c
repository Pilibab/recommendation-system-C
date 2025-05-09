#include "vectorizer.h"
#include "structures.h"
#include "structures.h"



#include <stdio.h>
#include <math.h>


void addWeight(
    struct dataSet *movies, int validMovieCount, 
    float weights[GENRESIZE], 
    double genreFreqCounter[GENRESIZE] )
{
    //add weighted vector to all valid movies
    for (int i = 0; i < validMovieCount; i++)   
    {
        weightedVector(&movies[i], genreFreqCounter, validMovieCount, weights);
    }    
}

/**
 * formula: log(total movies / no. of movies with genre of)
 * ensures that movies with lesser genres freq are not overshadowed by movies whose genre freq are allot
 */

void tf_idf(double df[GENRESIZE], double totalMovies, float weights[GENRESIZE])
{
    for (int i = 0 ; i < GENRESIZE; i++)
    {
        // Prevent division by zero or log(0)... although unecessary for this data set
        if (df[i] > 0) {
            df[i] = log(totalMovies / df[i]);
            weights[i] = (float) df[i];
        } else {
            df[i] = 0; // If genre never appears, set IDF to 0
        }
    }
}

/**
 * transform one hot encoded vector to weighted vector
 */
void weightedVector(struct dataSet * movies,double df[GENRESIZE], double totalMovies, float weights[GENRESIZE])
{
    double df_copy[GENRESIZE];          

    for (int i = 0; i < GENRESIZE; i++) {
        df_copy[i] = df[i];
    }
    
    tf_idf(df_copy, totalMovies, weights);

    for (int i = 0; i < GENRESIZE; i++) {    
        movies->weightedFeature[i] = (float)(movies->genreFeature[i] * df_copy[i]);
    }
}

/**
 * count the freq of each genre at constant time O(k)
 */
void freqGenreCounter(struct dataSet movies, double df[GENRESIZE], int genreCount)
{
    for (int i = 0 ; i < genreCount; i++)
    {
        if (movies.genreFeature[i] == 1) df[i]++;
    }
}