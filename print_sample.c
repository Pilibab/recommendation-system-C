#include "utilities.h"
#include "vect_ops.h"
#include "vectorizer.h"
#include "structures.h"


#include <stdio.h>


void printSample(struct dataSet * movies)
{
    // print sample data

    int j = 1357;
    for (int i = j ; i <  j + 10; i++)
    {
        printf("id: %d\ttitle: %-50s genre arr: ",i, movies[i].title);

        for(int j = 0; j < GENRESIZE; j++)
            printf("%d ",movies[i].genreFeature[j]);

        printf("\t similarity to id 1: %.2f", cosineSimilarity(movies[1],movies[i]));
        printf("\n");
    }
}