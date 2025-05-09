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

    while (1)
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

        // Flag that closes the loop 
        if (jumpTo == 0) 
        {
            break;
        }

        while(jumpTo < 1 || jumpTo > pages)
        {
            printf("Error: Out of Bound insert a valid range 1 <= x <= pages ");
            scanf(" %d", &jumpTo);
        }

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

    int exits = 1;
 
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
 

    // Print the sorted copy
    temp = copy;
    printf("\nMovies You Might Like\n");
    while(temp != NULL) 
    { 
        printf("\t%-55s You might give it:%.1f\n", movies[temp->movieId].title, temp->predictRate);
        temp = temp->next; 
    }
    
    printf("\n0. Go back to menu: ");
    scanf("%d", &exits);

    
    // Free the memory allocated for the copy list
    while (copy != NULL) 
    {
        struct unseen *toFree = copy;
        copy = copy->next;
        free(toFree);
    }

    if (exits == 0 ) return;
}

void runLogistic(float w[], struct unseen * notWatched, struct User * targetuser)
{
    
    predictMovie(w, notWatched, targetuser);
    int exits = 1; 

    printf("\n0. Go back to menu: ");
    scanf("%d", &exits);

    if (exits == 0 ) return;

}


void modifyUserData(struct User * targetuser, struct dataSet *movies)
{
    #ifdef _WIN32   // IF WINDOWS  
        system("cls"); 
    #else 
        system("clear"); 
    #endif 
    int counter = 0;
    struct MovieRating * temp = targetuser->ratings;

    printf("\n\nHere are the movies Youve watched\n\n");
    while (temp != NULL)
    {
        printf("\t%6d) %-55s Movie Id:%-6d Rating: %d\n ", counter + 1,  movies[temp->movieId].title, temp->movieId, temp->rating);
        temp = temp -> next; 
        counter++;
    }

    char choice; 
    printf("\n\nDo you want to modify data? y(yes)/ any key (no): ");
    scanf(" %c", &choice);

    if (tolower(choice) == 'y')
    {

        int delIn = -1;

        while (delIn != 0)
        {
            #ifdef _WIN32   // IF WINDOWS  
                system("cls"); 
            #else 
                system("clear"); 
            #endif 

            int index = 0;

            struct MovieRating *display = targetuser->ratings;
            while (display != NULL)
            {
                printf("\t%6d) %-55s Movie Id:%-6d Rating: %d\n", index + 1, movies[display->movieId].title, display->movieId, display->rating);
                display = display->next;
                index++;
            }

            int counter = 0; 
            printf("1. Insert data\n");
            printf("2. Delete data at any position\n");
            printf("3. change data rating / movie id\n");
            printf("0. back to menu\n");

            printf("Choice? ");
            scanf("%d", &delIn);

            while (delIn < 0 || delIn > 3)
            {
                printf("Error: Invalid index, please chose at the apt bound 1,2: ");
                scanf("%d", &delIn);
            }


            // -----    insert data at head -----
            if ( delIn == 1 )
            {
                int tempId, tempRate; 
                printf("Enter Movie Id and Rate (movieid rate): ");

                // validate if movie id and rate is at valid index
                scanf("%d %d", &tempId, &tempRate);

                struct MovieRating * newN = (struct MovieRating *)malloc(sizeof(struct MovieRating));
    
                newN->movieId = tempId;
                newN->rating = tempRate;
                newN->next = targetuser->ratings;

                targetuser->ratings = newN;

                targetuser->countRate++;
                targetuser->sumOfRate += tempRate;
            } 

            //----- delete data -----
            else if ( delIn == 2)
            {
                printf("Delete what position\n");

                int indexDel;
                // validate 
                scanf("%d", &indexDel);

                while (indexDel < 1 || indexDel > targetuser->countRate)
                {
                    printf("Error: Invalid position must be between 0 < x < %d", targetuser->countRate);
                    scanf("%d", &indexDel);
                }

                struct MovieRating *current = targetuser->ratings;
                struct MovieRating *prev = NULL;

                for (int i = 1; i < indexDel; ++i)
                {
                    prev = current;
                    current = current->next;
                }

                if (prev == NULL)
                    targetuser->ratings = current->next;
                else
                    prev->next = current->next;

                targetuser->sumOfRate -= current->rating;
                free(current);
                targetuser->countRate--;


                int index = 0;

                struct MovieRating *display = targetuser->ratings;
                while (display != NULL)
                {
                    printf("\t%6d) %-55s Movie Id:%-6d Rating: %d\n", index + 1, movies[display->movieId].title, display->movieId, display->rating);
                    display = display->next;
                    index++;
                }
            }
            else if (delIn == 3)
            {
                printf("Enter the movie position to modify (starting from 1): ");
                int pos;
                scanf("%d", &pos);

                if (pos < 1 || pos > targetuser->countRate)
                {
                    printf("Invalid position.\n");
                    continue;
                }

                struct MovieRating *current = targetuser->ratings;
                for (int i = 1; i < pos; i++)
                {
                    current = current->next;
                }

                targetuser->sumOfRate -= current->rating;

                printf("Enter new Movie Id and Rating: ");
                scanf("%d %d", &current->movieId, &current->rating);

                targetuser->sumOfRate += current->rating;

            }
        }


    } 
    else return;

    printf("\n\nUpdated Movie Ratings:\n\n");

    FILE * overWriteData = fopen("user-data\\u.txt", "w+");

    if (overWriteData == NULL) {
        perror("Error opening file for overwrite");
        return;
    }

    struct MovieRating *writingHead = targetuser->ratings;
    while (writingHead != NULL)
    {
        fprintf(overWriteData, "%d %d\n", writingHead->movieId, writingHead->rating);
        writingHead = writingHead -> next;
    }

    fclose(overWriteData);
    
    printf("\nPress Enter to return to the main menu...");
    getchar(); // consume leftover newline
    getchar(); // wait for actual Enter key
}




