#include "utilities.h"
#include "vect_ops.h"
#include "vectorizer.h"
#include "structures.h"


#include <stdio.h>


void printSample(struct dataSet * movies)
{
    // print sample data
    for (int i = 0 ; i < 10; i++)
    {
        printf("id: %d\ttitle: %-20s genre arr: ",i, movies[i].title);

        for(int j = 0; j < GENRESIZE; j++)
            printf("%d ",movies[i].genreFeature[j]);

        printf("\t similarity to id 1: %.2f", cosineSimilarity(movies[1],movies[i]));
        printf("\n");
    }
}