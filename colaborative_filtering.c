#include "colaborative_filtering.h"
#include "structures.h"
#include "pearson.h"
#include "utilities.h"
#include "clean_pearson.h"
#include "regression.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NEIGHBOR 10

void createUsermovieMatrix(FILE *usersRateMovie, 
    int userCount, 
    int numMovies, 
    int numRates,  
    struct dataSet movies[])
{
    struct User users[userCount];
    struct topSimiliarUser topPearsed[NEIGHBOR];                    //pun peers
    struct unseen *listofUnwatched = NULL;                                   //head of linked list (movies that the target user hasnt watched)    

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

    int tempMovieId, tempUserId, tempRate; 
    for (int i = 1; i < numRates; i++)
    {
        fscanf(usersRateMovie, "%d %d %d %*d", &tempUserId, &tempMovieId, &tempRate);
        addRating(&users[tempUserId],  tempMovieId - 1, tempRate);
    }




    // move all of this to run c
    int target = 1;

    topNeighboor(users[target], users, userCount, topPearsed);
    getUnseenMovies(topPearsed, &listofUnwatched, users);

    listofUnwatched = setThreshold(listofUnwatched);

    predictRate(listofUnwatched);

    struct unseen * rankedUnseen = NULL; 

    // struct unseen * temp = listofUnwatched;
    // while (temp != NULL)
    // {
    //     int index = temp->movieId;
    //     printf("we recomend %-30s \n\tpredicted rating is: %f\n", movies[index].title, temp->predictRate);
    //     // printf("\tmovie: %d, N: %d, [%.2f, %.2f]\n", temp->movieId, temp->neighborCount, temp->similaritySum, temp->weightedSum);
    //     temp = temp->next;
    // }

    struct unseen *listOfWatched = NULL;   

    printf("getting watched\n");
    watched(topPearsed, &listOfWatched, users);
    predictRate(listOfWatched);

        struct unseen * temp = listOfWatched;
    // while (temp != NULL)
    // {
    //     int index = temp->movieId;
    //     printf("%d: %f\n", temp->movieId, temp->predictRate);
    //     // printf("\tmovie: %d, N: %d, [%.2f, %.2f]\n", temp->movieId, temp->neighborCount, temp->similaritySum, temp->weightedSum);
    //     temp = temp->next;
    // }

    // Initial weights
    float w[5] = {1,0,0,0,0};  

    if (users->countRate < 10)
    {
        epoch(&listOfWatched, &users[target], 1000, w);
    }

    printf("\nnew weights: \n");
    float prevW[5]= {1,0,0,0,0};

    for (int i = 0 ; i<5 ;i++)
    {
        printf("\t%f: delta_w[%d] = %.6f\n", w[i], i, w[i] - prevW[i]);
    }
}

void addRating(struct User *user, int id, int rating) 
{
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

void printSampleLinked(struct User * user)
{
    for (int i = 1; i < 10 ; i ++)
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
