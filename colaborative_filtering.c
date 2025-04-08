#include "colaborative_filtering.h"
#include "structures.h"

#include <stdio.h>
#include <stdlib.h>


void createUsermovieMatrix(FILE *usersRateMovie, int user, int numMovies, int numRates)
{

    struct User users[user];
    for (int i = 0; i < user; i++) {
        users[i].ratings = NULL;
        users[i].countRate = 0;
    }

    int tempMovieId, tempUserId, tempRate; 
    for (int i = 1; i < numRates; i++)
    {
        
        fscanf(usersRateMovie, "%d %d %d %*d", &tempUserId, &tempMovieId, &tempRate);


        {
        addRating(&users[tempUserId], tempMovieId, tempRate);
        }
    }
    printSampleLinked(users);
}

void addRating(struct User *user, int id, int rating) 
{
    struct MovieRating* temp, *head = user->ratings;
    struct MovieRating* newN = (struct MovieRating *)malloc(sizeof(struct MovieRating));

    newN->movieId=id;
    newN->next = NULL;

    if (head == NULL || id < head->movieId)
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
        printf("user: %d no of rating: %d\n", i, user[i].countRate);

        struct MovieRating * curr = user[i].ratings;

        while (curr != NULL)
        {
            printf("%d ", curr->movieId);
            curr = curr->next; 
        }
        printf("\n");
    }
}
