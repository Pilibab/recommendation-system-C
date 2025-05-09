#include "utilities.h"
#include "vect_ops.h"
#include "vectorizer.h"
#include "structures.h"


#include <stdio.h>


void printSample(struct dataSet * movies, int items, int jumpTo, int genreCount, char **genre)
{
    for (int i = (jumpTo * 20) - 20, counter = 0; i <  items && counter < 20; i++, counter++)
    {
        printf("\tid: %-4d title: %-55s genre: ",i, movies[i].title);
        printAptGEnre(&movies[i], genreCount,genre);
        printf("\n");
    }
}

void printAptGEnre(struct dataSet *movies, int genreCount, char **genre)
{   
    for (int i = 0; i < genreCount; i++)
        if (movies->genreFeature[i] == 1)
            printf("%s, ", genre[i]);
}

void printSampleLinked(struct User * user)
{
    for (int i = 0; i < 10 ; i ++)
    {
        double avg = 0;
        if ( user[i].countRate != 0 )
            avg = user[i].sumOfRate / user[i].countRate;

        printf("user: %d no of rating: %d total rating: %.2lf average: %.2lf\n", 
            i, user[i].countRate, user[i].sumOfRate, avg);
     
        struct MovieRating * curr = user[i].ratings;
        while (curr != NULL)
        {
            printf("%d ", curr->movieId);
            curr = curr->next; 
        }
        printf("\n");
    }
}
