#include <math.h>
#include <stdio.h>


#include "regression.h"
#include "pearson.h"

/**
 * using structure for unseen but im really getrting the watched movies here
 */
struct unseen watched(struct topSimiliarUser * pears, 
    struct unseen ** watchedHead, 
    struct  User toCompare[])
{

    for (int i = 0; i < NEIGHBOR; i++)
    {
        int index = pears[i].userId;

        struct ratingsTopN * curr = pears[i].seenMovies;                        //watched movies by user and index Neigh
        struct  MovieRating * allMoviesDb = toCompare[index].ratings;           //all movies Data base User watched -> get unwatched 
        while (allMoviesDb != NULL)
        {

            if (curr == NULL) 
            {
                insertUnwatched(watchedHead, allMoviesDb->movieId, allMoviesDb->rating ,pears[i].pearsonScore);

                allMoviesDb = allMoviesDb->next;
            } else 
            {
                if ((curr->movieId == allMoviesDb->movieId)) {
                    insertUnwatched(watchedHead, allMoviesDb->movieId, allMoviesDb->rating ,pears[i].pearsonScore);

                    allMoviesDb = allMoviesDb->next;
                    curr = curr->next;
                } 
                else if ((curr->movieId > allMoviesDb->movieId )){
                    //movie not seen by user 
                    allMoviesDb = allMoviesDb->next;
                }   else 
                    // This shouldn't happen if commonMovies is a subset of allMoviesDb (meaning allMoviesDb is from curr)
                    curr = curr->next;  
            }
        }
    }
    // struct unseen *temp = *watchedHead;   

    // while (temp != NULL)
    // {
    //     printf("movie: %d, N=%d Ssum: %f, wSum: %f\n", temp->movieId, temp->neighborCount, temp->similaritySum, temp->weightedSum);
    //     temp = temp->next;
    // }
}

/**
 * f[0]	Average similarity with K neighbors who rated the movie.
 * f[1]	Number of neighbors who rated the movie (maybe normalized).
 * f[2]	Average rating from those neighbors.
 * f[3]	User’s own average rating across all movies.
 * f[4]	Always 1.0 (bias trick).
 */
float probability (float w[], float f[])
{


    
    return 1 / (1 + exp(z(w,f)));
}

float z(float w[], float f[])
{
    return( w[0] +  w[1] * f[0] + 
        w[2] * f[1] + w[3] * f[2] + 
        w[4] * f[3] + w[5] * f[4]);
}

void epoch( struct unseen ** watchedHead, struct User * targetuser)
{
    float w[6] = {1,0,0,0,0,0};                               //initial weights
    float averageByTargetU = targetuser->sumOfRate / targetuser->countRate;


    struct unseen * temp = *watchedHead;

    while (temp != NULL)
    {
        float f[5] = {
            (temp->similaritySum / temp->neighborCount), 
            temp->neighborCount, 
            temp->predictRate,               //weighted average
            averageByTargetU,
            1.0
            };

        // printf("%f ", probability(w, f));

        temp = temp -> next;
    }
}