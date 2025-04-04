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
void freqGenreCounter(struct dataSet movies, double df[GENRESIZE])
{
    if (movies.genreFeature[0] == 1) df[0]++;
    if (movies.genreFeature[1] == 1) df[1]++;
    if (movies.genreFeature[2] == 1) df[2]++;
    if (movies.genreFeature[3] == 1) df[3]++;
    if (movies.genreFeature[4] == 1) df[4]++;
    if (movies.genreFeature[5] == 1) df[5]++;
    if (movies.genreFeature[6] == 1) df[6]++;
    if (movies.genreFeature[7] == 1) df[7]++;
    if (movies.genreFeature[8] == 1) df[8]++;
    if (movies.genreFeature[9] == 1) df[9]++;
    if (movies.genreFeature[10] == 1) df[10]++;
    if (movies.genreFeature[11] == 1) df[11]++;
    if (movies.genreFeature[12] == 1) df[12]++;
    if (movies.genreFeature[13] == 1) df[13]++;
    if (movies.genreFeature[14] == 1) df[14]++;
    if (movies.genreFeature[15] == 1) df[15]++;
    if (movies.genreFeature[16] == 1) df[16]++;
    if (movies.genreFeature[17] == 1) df[17]++;
    if (movies.genreFeature[18] == 1) df[18]++; 
}