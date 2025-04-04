#include <stdio.h>
#include <string.h>
#include <math.h>


#include "vect_ops.h"
#include "vectorizer.h"
#include "structures.h"
#include "utilities.h"


void loadData(struct dataSet movies[], int items, FILE *itemFile, int *validMovieCount, double genreFreqCounter[])
{
    char bufferRD[50];
    char bufferLink[250];

    for (int i = 0; i < items; i++)
    {
        int result = fscanf(itemFile, "%*d|%[^|]|%[^|]||%[^|]"
            "|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d",
            movies[*validMovieCount].title,  bufferRD, bufferLink,
            &(movies[*validMovieCount].genreFeature[0]), &(movies[*validMovieCount].genreFeature[1]), 
            &(movies[*validMovieCount].genreFeature[2]), &(movies[*validMovieCount].genreFeature[3]), 
            &(movies[*validMovieCount].genreFeature[4]), &(movies[*validMovieCount].genreFeature[5]), 
            &(movies[*validMovieCount].genreFeature[6]), &(movies[*validMovieCount].genreFeature[7]), 
            &(movies[*validMovieCount].genreFeature[8]), &(movies[*validMovieCount].genreFeature[9]),
            &(movies[*validMovieCount].genreFeature[10]), &(movies[*validMovieCount].genreFeature[11]), 
            &(movies[*validMovieCount].genreFeature[12]), &(movies[*validMovieCount].genreFeature[13]), 
            &(movies[*validMovieCount].genreFeature[14]), &(movies[*validMovieCount].genreFeature[15]), 
            &(movies[*validMovieCount].genreFeature[16]), &(movies[*validMovieCount].genreFeature[17]), 
            &(movies[*validMovieCount].genreFeature[18]));
        
        if (result == 22)  
        {  
            freqGenreCounter(movies[*validMovieCount], genreFreqCounter);
            (*validMovieCount)++;  // Increment valid count
        } 
        else 
        {
            char discard[1000];  
            if (fgets(discard, sizeof(discard), itemFile) == NULL) 
                break;  

            printf("Skipped a movie entry due to missing val at %d\n", i);
        }    
    }
}











