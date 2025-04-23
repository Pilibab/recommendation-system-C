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

    for (int i = 1; i < userCount + 1; i++)                                  //holly fucking shit 6hrs... this is the solution 
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

    
    SimilarUser similarUsers[NEIGHBOR];
    struct ratingsTopN kth[NEIGHBOR];
    struct topSimiliarUser topPearsed[NEIGHBOR];                    //pun peers

    for (int i = 0; i < NEIGHBOR; i++) {
        similarUsers[i].userId = 0;
        similarUsers[i].similarCount = 0;

        kth[i].movieId = 0;
        kth[i].next = NULL;
        kth->rating[0] = 0;
        kth->rating[1] = 0;

        topPearsed->pearsonScore = 0.0f;
        topPearsed->userId = 0;
        topPearsed->unseenMovies = NULL;
    }

    //compare user
    topNeighboor(users[1], users, similarUsers, userCount);
    printf("getting rate\n");
    getRateOfMovie (kth, users[1], users, similarUsers);

    printf("computing pearson\n");
    pearsonCorrelation(similarUsers, users, users[1], topPearsed);
    printf("computed!!!\n");


    for (int i = 0 ; i < NEIGHBOR; i++)
    {    

        struct ratingsTopN * temp = similarUsers[i].theirMovies;

        printf("user: %d similarity: %.4lf\n", topPearsed[i].userId, topPearsed[i].pearsonScore);

        int j = 0; 
        while(temp != NULL && j < 3)
        {
            printf("\t%d: %d, %d\n", temp->movieId, temp->rating[0], temp->rating[1]);
            temp = temp -> next;
            j++;
        }
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
