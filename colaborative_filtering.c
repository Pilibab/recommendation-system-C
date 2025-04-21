#include "colaborative_filtering.h"
#include "structures.h"
#include "pearson.h"

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

    printf("adding rating\n"); 
    int tempMovieId, tempUserId, tempRate; 
    for (int i = 1; i < numRates; i++)
    {
        fscanf(usersRateMovie, "%d %d %d %*d", &tempUserId, &tempMovieId, &tempRate);
        addRating(&users[tempUserId], tempMovieId, tempRate);
    }

    printf("comparing user\n");
    
    SimilarUser similarUsers[NEIGHBOR];

    for (int i = 0; i < NEIGHBOR; i++) {
        similarUsers[i].userId = 0;
        similarUsers[i].similarCount = 0;
        similarUsers[i].pearsonScore = 0.0f;
        similarUsers[i].similarMovie_UA = NULL;
        similarUsers[i].similarMovie_UB = NULL;
    }


    //compare user
    topNeighboor(users[1], users, similarUsers, userCount);

    printf("printing top neighbor\n");

    for (int i = 0; i < NEIGHBOR; i++)
        printf("%d: %d \n", similarUsers[i].userId , similarUsers[i].similarCount);

    // printSampleLinked(users);
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

void freeRatings(struct User *user) 
{
    struct MovieRating *current = user->ratings;
    while (current != NULL) {
        struct MovieRating *temp = current;
        current = current->next;
        free(temp);
    }
    user->ratings = NULL;  // Avoid dangling pointer
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
     
        // struct MovieRating * curr = user[i].ratings;
        // while (curr != NULL)
        // {
        //     printf("%d ", curr->movieId);
        //     curr = curr->next; 
        // }
        printf("\n");
    }
}
