#ifndef VECTORIZER_H
#define VECTORIZER_H
#include "structures.h"
#include "structures.h"



#define GENRESIZE 19
    void addWeight(
        struct dataSet *movies, 
        int validMovieCount, 
        float weights[GENRESIZE], 
        double genreFreqCounter[GENRESIZE]);

    void weightedVector(
        struct dataSet * movies , 
        double df[GENRESIZE], 
        double totalMovies, 
        float weights[GENRESIZE]);

    void freqGenreCounter(struct dataSet movies, double df[GENRESIZE], int genreCount);
    
    void tf_idf(double df[GENRESIZE], 
        double totalMovies, 
        float weights[GENRESIZE]);

#endif