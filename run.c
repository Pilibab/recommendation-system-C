#include "vect_ops.h"
#include "vectorizer.h"
#include "utilities.h"
#include "colaborative_filtering.h"
#include "structures.h"



#include <stdio.h>
#include <stdlib.h>

int main()
{
    
    FILE *infoFile = fopen("ml-100k\\u.info", "r");
    FILE *itemFile = fopen("ml-100k\\u.item", "r");
    FILE *dataFile = fopen("ml-100k\\u.data", "r");


    int users, items, ratings;

    //check if file ptr exists
    if (infoFile == NULL)
    {
        printf("Error: <u.info> file could not be opened.\n");
        return 0;
    }
    if (itemFile == NULL)
    {
        printf("Error: <u.item> file could not be opened.\n");
        return 0;
    }    
    if (dataFile == NULL)
    {
        printf("Error: <u.data> file could not be opened.\n");
        return 0;
    }

    //scan info file for num of user items and ratings
    fscanf(infoFile, "%d users", &users);
    fscanf(infoFile, "%d items", &items);
    fscanf(infoFile, "%d ratings", &ratings);

    //create arr struct to store movie data set
    struct dataSet movies[items]; 

    //initialize arr and counter related to vectorization
    double genreFreqCounter[GENRESIZE] = {0}; 
    float weights[GENRESIZE];    
    int validMovieCount = 0;  

    // Pass parameters to loadData()
    printf("loading data...\n");
    loadData(movies, items, itemFile, &validMovieCount, genreFreqCounter);

    // Now pass movies to addWeight()
    printf("adding weights...\n");
    addWeight(movies, validMovieCount, weights, genreFreqCounter);
    // printSample(movies);

    printf("creating movie matrix...\n");
    createUsermovieMatrix(dataFile, users, items, ratings, movies);


    
    fclose(infoFile);
    fclose(itemFile);
    fclose(dataFile);
    return 0;
}
