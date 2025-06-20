#include "colaborative_filtering.h"
#include "structures.h"
#include "pearson.h"
#include "utilities.h"
#include "clean_pearson.h"
#include "regression.h"
#include "prediction.h"
#include "handle_inputs.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NEIGHBOR 10

void createUsermovieMatrix(FILE *usersRateMovie, 
    int userCount, 
    int numMovies, 
    int numRates,  
    struct dataSet movies[],     
    struct User *users,
    struct topSimiliarUser *topPearsed  )
{

    // Initialize members to NULL and zero to avoid unexpected behavior 
    for (int i = 0; i < userCount + 1; i++)                                  
    {
        users[i].ratings = NULL;
        users[i].countRate = 0;
        users[i].sumOfRate = 0;
        if (i < NEIGHBOR)
        {
            topPearsed[i].pearsonScore = 0.0f;
            topPearsed[i].userId = 0;
            topPearsed[i].seenMovies = NULL;
        }
    }

    int tempMovieId, tempUserId, tempRate;                                                  // Temp place holder for validation 

    // Scan file add movie, rate, count and sum the rate 
    for (int i = 0; i < numRates; i++)
    {
        fscanf(usersRateMovie, "%d %d %d %*d", &tempUserId, &tempMovieId, &tempRate);
        addRating(&users[tempUserId],  tempMovieId - 1, tempRate, movies);
    }
}

void addRating(struct User *user, int id, int rating, struct dataSet movies[]) 
{

    // use pointer arithmetic to shift to correct index
    id = shiftId(id, movies);
    
    struct MovieRating* temp, *head = user->ratings;
    struct MovieRating* newN = (struct MovieRating *)malloc(sizeof(struct MovieRating));

    newN->movieId = id;
    newN->rating = rating;
    newN->next = NULL;

    if (head == NULL || id < head->movieId)                             //insert newN at the beging if null or smallest
    {
        newN->next = head;
        head = newN;
    }
    else
    {
        temp = head; 
        while (temp->next != NULL && temp->next->movieId < id)
            temp = temp -> next;

        newN->next = temp -> next;
        temp -> next = newN;
    }

    user->ratings = head;
    user->sumOfRate += rating;
    user->countRate++;
}
