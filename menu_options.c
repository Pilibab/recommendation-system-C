#include "menu_options.h"
#include "structures.h"
#include "utilities.h"
#include "vect_ops.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void displayAllMovies(struct dataSet *movies, int items, int genreCount, char **genres)
{

    int jumpTo = 1;
    int pages = items / 20 + ((items % 20 != 0)? 1: 0);

    int TRUTHY = 1;
    while (TRUTHY)
    {
        #ifdef _WIN32   // IF WINDOWS 
            system("cls");
        #else
            system("clear");
        #endif

        // Add spaces before the Title of the table
        for (int header = 0 ; header < 2; header++)
            printf("\n");

        printf("\t\t\t\t\tMovies From data base\n\n");
        
        printSample(movies, items, jumpTo, genreCount, genres);
        printf("\nTotal pages: %d\t\tCurrent page: %d\n", pages, jumpTo);
        printf("Enter page number (1 to %d), or 0 to go back to menu: ", pages);
        scanf(" %d", &jumpTo);

        while(jumpTo < 1 || jumpTo > pages)
        {
            printf("Error: Out of Bound insert a valid range 1 <= x <= pages ");
            scanf(" %d", &jumpTo);
        }
        // Flag that closes the loop 
        if (jumpTo == 0) TRUTHY = 0;
    }
}

void showCosineSimilar(struct dataSet * movies, int items)
{
    #ifdef _WIN32   // IF WINDOWS 
        system("cls");
    #else
        system("clear");
    #endif
    char charChoice;
    int movieId;
    struct MovieSimilaritCosine * head = NULL, * temp; 

    printf("Enter movie id u want to compare ");
    scanf("%d", &movieId);

    for (int i = 0; i < items ; i++)
    {
        if (movieId == i || movies[i].pointsToFirst != NULL) continue;

        float cosScore = cosineSimilarity(movies[movieId], movies[i]);

        if (cosScore > .75) 
        {
            struct MovieSimilaritCosine * newN = (struct MovieSimilaritCosine*) malloc(sizeof(struct MovieSimilaritCosine)); 
            newN->movieId = i;
            newN->similarityScore = cosScore;
            newN->next = NULL;

            if (head == NULL || cosScore  > head->similarityScore)
            {
                newN->next = head;
                head = newN;
            } else
            {
                temp = head; 
                while (temp->next != NULL && temp->next->similarityScore > cosScore )
                    temp = temp -> next;

                newN->next = temp -> next;
                temp->next = newN;
            }
        } else continue;
    }

        // Display similar movies
    printf("\nMovies similar to \"%s\":\n", movies[movieId].title);
    temp = head;
    while (temp)
    {
        printf("id: %-6d Title: %-55s Similarity: %.2f\n",
               temp->movieId, movies[temp->movieId].title, temp->similarityScore);
        temp = temp->next;
    }

    printf("\n\nEnter movie id you want to compare Y (yes)/ any key(No) ");
    scanf(" %c", &charChoice);

    if (tolower(charChoice) == 'y')
        showCosineSimilar(movies, items);
    else return;
}
