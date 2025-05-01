#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "handle_inputs.h"
#include "structures.h"
struct User getTargetUserMovies(FILE * file, struct dataSet movies[] )
{
    struct User user = {0, 0.0, NULL};
    
    int movieId, rating;
    while (fscanf(file, "%d %d", &movieId, &rating) == 2) {

        if (movies[movieId].pointsToFirst == NULL)
        {
            struct dataSet *canonical = &movies[movieId];
    
            // Note that: movies is 0 -> n - 0 to return int
            movieId = canonical - movies ;                                                           // returns the distance / how far cannonical is interms of int           
        }

        user.ratings = insertCorrectPos(user.ratings, movieId, rating);
        user.countRate++;
        user.sumOfRate += rating;
    }

    if(askToInsertData())
    {
        while (1) 
        {
            int id = -1, rate =-1;
            printf("Enter movie id and rating (or -1 -1 to stop): ");
            scanf("%d %d", &id, &rate);

            // If case exits program
            if (id == -1 && rate == -1) break;


            int err = 0;                                                        // Flag if error is made
            if (id < 0 || id > 1682) {                                          // Out of range movie id
                printf("Error: Invalid id 0 <= id <= 1682\n");
                err = 1;
            }
            
            if (rate < 1 || rate > 5) {                                         // Out of range rating
                printf("Error: Invalid rate 1 <= rate <= 5\n");
                err = 1;
            }

            if (err) continue;                                                  // ask again 

            if (movies[movieId].pointsToFirst == NULL)
            {
                struct dataSet *canonical = &movies[movieId];
        
                // Note that: movies is 0 -> n - 0 to return int
                movieId = canonical - movies ;                                                           // returns the distance / how far cannonical is interms of int           
            }
            
            user.ratings = insertCorrectPos(user.ratings, id, rate);
            user.countRate++;
            user.sumOfRate += rate;

            // Append to file
            fprintf(file, "%d %d\n", id, rate);
        }
    }
    return user;
}

/**
 * ask user if they want to rate movie and check the input of user
 *      y: add rating (to file and create struct User for target user)
 *      n: rely on pre-existing data 
 */
int askToInsertData()
{
    char ch;
    printf("Would you like to add new movies? Y/N: ");
    scanf(" %c", &ch);

    switch (tolower(ch))
    {
        case 'y':
            return 1;
        case 'n':
            return 0;
        default:
            printf("Error: Input invalid please try again ");
            askToInsertData();
            break;
    }
}

struct MovieRating * insertCorrectPos(struct MovieRating * head, int movieId, int rating)
{
    struct MovieRating *newNode = malloc(sizeof(struct MovieRating));
    newNode->movieId = movieId;
    newNode->rating = rating;
    newNode->next = NULL;

    if (head == NULL || movieId < head->movieId) {
        newNode->next = head;
        return newNode;
    }

    struct MovieRating *curr = head;
    while (curr->next != NULL && curr->next->movieId < movieId) {
        curr = curr->next;
    }

    newNode->next = curr->next;
    curr->next = newNode;

    return head;
}