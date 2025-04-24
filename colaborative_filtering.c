#include "colaborative_filtering.h"
#include "structures.h"
#include "pearson.h"
#include "utilities.h"

#include <stdio.h>
#include <stdlib.h>
#define NEIGHBOR 10

void createUsermovieMatrix(FILE *usersRateMovie, int userCount, int numMovies, int numRates)
{
    struct User users[userCount];

    for (int i = 0; i < userCount + 1; i++)                                  //holly fucking shit 6hrs... this is the solution 
    {
        users[i].ratings = NULL;
        users[i].countRate = 0;
        users[i].sumOfRate = 0;
    }

    int tempMovieId, tempUserId, tempRate; 
    for (int i = 1; i < numRates; i++)
    {
        fscanf(usersRateMovie, "%d %d %d %*d", &tempUserId, &tempMovieId, &tempRate);
        addRating(&users[tempUserId], tempMovieId, tempRate);
    }

    struct topSimiliarUser topPearsed[NEIGHBOR];                    //pun peers
    struct unseen listofUnwatched;                                  //head of linked list (movies that the target user hasnt watched)
    listofUnwatched.next = NULL;                                  


    for (int i = 0; i < NEIGHBOR; i++) 
    {
        topPearsed[i].pearsonScore = 0.0f;
        topPearsed[i].userId = 0;
        topPearsed[i].seenMovies = NULL;
    }

    //compare user
    printf("getting top N\n");

    int target = 1;

    topNeighboor(users[target], users, userCount, topPearsed);
    printf("getting rate\n");

    printf("Top similar users (Pearson):\n");

    // for (int i = 0 ; i < NEIGHBOR; i++)
    // {
    //     struct ratingsTopN * temp = topPearsed[i].seenMovies;
    //     printf("%d => user: %d similarity: %.4lf\n",i, topPearsed[i].userId, topPearsed[i].pearsonScore);

    //     while(temp != NULL)
    //     {
    //         printf("\tMovieID: %d, Ratings: [%d, %d]\n", temp->movieId, temp->rating[0], temp->rating[1]);
    //         temp = temp->next;
    //     }
    // }

    printf("getting unseen\n");
    getUnseenMovies(topPearsed, &listofUnwatched, users);
    printf("done\n");

    struct unseen * temp = &listofUnwatched;

    while (temp != NULL)
    {
        printf("\tmovie: %d, N: %d, [%.2f, %.2f]\n", temp->movieId, temp->neighborCount, temp->similaritySum, temp->weightedSum);
        temp = temp->next;
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
