#include "menu_options.h"
#include "structures.h"
#include "utilities.h"
#include "vect_ops.h"
#include "prediction.h"

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
    printf("\nMovies similar to \"%s\":\n\n", movies[movieId].title);
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

void runCollaborative(struct unseen *listofUnwatched, struct dataSet *movies) 
{ 
    #ifdef _WIN32   // IF WINDOWS  
        system("cls"); 
    #else 
        system("clear"); 
    #endif 
 
    struct unseen *temp = listofUnwatched;
    struct unseen *copy = NULL;  // Initialize copy as NULL
    
    // Create a copy of the list
    while (temp != NULL) {
        if (temp->predictRate > 3) {
            // Create new node
            struct unseen *newNode = (struct unseen *)malloc(sizeof(struct unseen));
            if (newNode == NULL) {
                printf("Memory allocation failed\n");
                return;
            }
            
            // Copy data
            newNode->movieId = temp->movieId;
            newNode->predictRate = temp->predictRate;
            
            // Insert into copy list in descending order
            if (copy == NULL || copy->predictRate < temp->predictRate) {
                // Insert at beginning
                newNode->next = copy;
                copy = newNode;
            } else {
                // Find position to insert
                struct unseen *current = copy;
                while (current->next != NULL && current->next->predictRate >= temp->predictRate) {
                    current = current->next;
                }
                newNode->next = current->next;
                current->next = newNode;
            }
        }
        temp = temp->next;
    }
 
    int TRUTHY = 1; 
    while (TRUTHY) 
    { 
        // Print the sorted copy
        temp = copy;
        printf("\nMovies You Might Like\n");
        while(temp != NULL) 
        { 
            printf("\t%-55s You might give it:%.1f\n", movies[temp->movieId].title, temp->predictRate);
            temp = temp->next; 
        }
        
        printf("\n0. Go back to menu: ");
        scanf("%d", &TRUTHY);
    }
    
    // Free the memory allocated for the copy list
    while (copy != NULL) {
        struct unseen *toFree = copy;
        copy = copy->next;
        free(toFree);
    }
}

void runLogistic(float w[], struct unseen * notWatched, struct User * targetuser)
{
    
    predictMovie(w, notWatched, targetuser);
    int TRUTHY = 1; 
    while (TRUTHY) 
    { 
        printf("\n0. Go back to menu: ");
        scanf("%d", &TRUTHY);
    }

}





