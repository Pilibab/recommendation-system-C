#include "colaborative_filtering.h"
#include "structures.h"

#include <stdio.h>
#include <stdlib.h>


void createUsermovieMatrix(FILE *usersRateMovie, int user, int numMovies, int numRates)
{

    struct User users[user];

    int tempMovieId, tempUserId, tempRate; 
    for (int i = 0; i < numRates; i++)
    {
        fscanf(usersRateMovie, "%d %d %d %*d", &tempUserId, &tempMovieId, &tempRate);

        //append rate, id in linked list
        addRating(&users[tempUserId], tempMovieId, tempRate);
    }

    printSampleLinked(users);
}

void addRating(struct User *user, int movieId, int rating) 
{
    struct MovieRating *newRating = malloc(sizeof(struct MovieRating));

    newRating->movieId = movieId;
    newRating->rating = rating;
    newRating->next = user->ratings;
    user->ratings = newRating;
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
    for (int i = 0; i < 10 ; i ++)
    {
        printf("user: %d", i);

        struct MovieRating * curr = user[i].ratings;
        while (curr != NULL)
        {
            printf("{movie_id: %d, rating: %d}", curr->movieId, curr->rating);
            curr = curr->next; 
        }
        printf("\n");
    }
}
