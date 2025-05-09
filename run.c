#include "vect_ops.h"
#include "vectorizer.h"
#include "utilities.h"
#include "colaborative_filtering.h"
#include "structures.h"
#include "pearson.h"
#include "clean_pearson.h"
#include "regression.h"
#include "prediction.h"
#include "handle_inputs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main()
{
    // File ptr from Ml-100k
    FILE *infoFile = fopen("ml-100k\\u.info", "r");
    FILE *itemFile = fopen("ml-100k\\u.item", "r");
    FILE *dataFile = fopen("ml-100k\\u.data", "r");
    FILE *genreFile = fopen("ml-100k\\u.genre", "r");

    // User cookies (store previous inputs)
    FILE *userCookie = fopen("user-data\\u.txt", "a+");

    int usersCount, items, ratings;

    // Check if file ptr exists
    int fileCount = checkFile(infoFile, itemFile, dataFile, genreFile, userCookie);

    if (fileCount == 0)
        printf("Essential Files are present\n");
    else 
    {
        printf("%d missing Files cannot proceed");
        return 0; 
    }

    // Scan info file for num of user items and ratings
    fscanf(infoFile, "%d users", &usersCount);
    fscanf(infoFile, "%d items", &items);
    fscanf(infoFile, "%d ratings", &ratings);

    
    // Create arr struct to store movie data set
    struct dataSet movies[items]; 
    struct unseen *listofUnwatched = NULL;                                  //head of linked list (movies that the target user hasnt watched)    
    struct unseen *listOfWatched = NULL;   
    struct User users[usersCount];
    struct topSimiliarUser topPearsed[usersCount];    

    // Get user data (u)
    struct User targetUser = getTargetUserMovies(userCookie, movies);

    // Initialize arr and counter related to vectorization
    double genreFreqCounter[GENRESIZE] = {0}; 
    float weights[GENRESIZE];    
    int validMovieCount = 0;  

    // Get apt genre-index
    int genreCount = 0;
    char **genre = loadGenres(genreFile, &genreCount);

    // Initialize pointer of each movie to NULL
    initializeFile(movies, items);


    // Pass parameters to loadData()
    // printf("loading data...\n");
    loadData(movies, items, itemFile, &validMovieCount, genreFreqCounter, genreCount);


    // Check for duplicates 
    checkDups(movies, items);

    // Now pass movies to addWeight()
    // printf("\nadding weights...\n");
    addWeight(movies, validMovieCount, weights, genreFreqCounter);

    // printf("creating movie matrix...\n");
    createUsermovieMatrix(dataFile, usersCount, items, ratings, movies, users, topPearsed);

    
    // printf("printing sample\n");


    int choice;

    while (1) {
        printf("\nChoose an option:\n");
        printf("1. View all movies\n");
        printf("2. Similar movies (cosine)\n");

        if (targetUser.countRate > 4)
            printf("3. Collaborative filtering (KNN)\n");

        if (targetUser.countRate >= 10)
            printf("4. Logistic regression prediction\n");

        printf("0. Exit\n");
        printf("Enter choice: ");

        scanf(" %d", &choice);

        switch (choice) {
            case 1:
                printf("printing Movies From data base\n");
                displayAllMovies(movies, items, genreCount, genre );
                break;
            case 2:
                // showCosineSimilar(targetUser, movies);
                break;
            case 3:
                // runCollaborative(targetUser, users, topPearsed);
                break;
            case 4:
                if (targetUser.countRate < 10)
                    printf("Not enough ratings to run logistic regression. Rate at least 10 movies.\n");
                else
                    // runLogistic(targetUser, users, topPearsed);
                break;
            case 0:
                printf("Goodbye.\n");
                exit(0);
            default:
                printf("Invalid option.\n");
        }
    }



    // skip this part first 
    // Create movie and User matrix 
    topNeighboor(targetUser, users, usersCount, topPearsed);
    getUnseenMovies(topPearsed, &listofUnwatched, users);

    // Update listOfUnwatched
    listofUnwatched = setThreshold(listofUnwatched);

    predictRate(listofUnwatched);


    // printf("getting watched\n");
    watched(topPearsed, &listOfWatched, users);
    predictRate(listOfWatched);

    // Initial weights
    float w[5] = {1,0,0,0,0};  

    if (targetUser.countRate > 20)                                          // If no of rated met the minimum reqment
    {
        epoch(&listOfWatched, &targetUser, 400, w);
    }


    // Making prediction 
    // predictMovie(w, listofUnwatched, &targetUser);

    // printSampleLinked(users);
    






    // Included in paper close everything 
    fclose(infoFile);
    fclose(itemFile);
    fclose(dataFile);
    return 0;
}

char **loadGenres(FILE *genreFile, int *genreCount) {
    int capacity = 10;
    *genreCount = 0;

    char **genreList = malloc(capacity * sizeof(char *));
    if (!genreList) {
        printf( "Memory allocation failed for genre list.\n");
        return NULL;
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), genreFile)) 
    {
        // Remove newline
        buffer[strcspn(buffer, "\n")] = 0;

        // Tokenize by '|'
        char *name = strtok(buffer, "|");
        char *indexStr = strtok(NULL, "|"); // Not used directly here

        if (!name) continue;

        // Resize array if needed
        if (*genreCount >= capacity) {
            capacity *= 2;
            genreList = realloc(genreList, capacity * sizeof(char *));
            if (!genreList) {
                printf("Memory reallocation failed for genre list.\n");
                return NULL;
            }
        }

        // Store genre name
        genreList[*genreCount] = malloc(strlen(name) + 1);
        if (!genreList[*genreCount]) {
            printf("Memory allocation failed for genre name.\n");
            return NULL;
        }
        strcpy(genreList[*genreCount], name);
        (*genreCount)++;
    }

    return genreList;
}


int checkFile (FILE * infoFile, 
    FILE * itemFile, 
    FILE * dataFile, 
    FILE * genreFile,
    FILE * userCookie
)
{
    int Flag = 0 ;
    if (infoFile == NULL)
    {
        printf("Error: <u.info> file could not be opened.\n");
        Flag++;
    }
    if (itemFile == NULL)
    {
        printf("Error: <u.item> file could not be opened.\n");
        Flag++;

    }    
    if (dataFile == NULL)
    {
        printf("Error: <u.data> file could not be opened.\n");
        Flag++;
    }
    if (userCookie == NULL) 
    {
        perror("File creation failed");
        Flag++;
    }
    return Flag;
}

void initializeFile(struct dataSet *movies, int items)
{
    for (int i = 0; i < items ; i++)
    {
        movies[i].pointsToFirst = NULL;
    }
}

void checkDups(struct dataSet *movies, int items)
{
    for (int i = 0, count = 1; i < items; i++, count++)
    {
        for (int j = 0; j < items; j++)
        {
            if (i == j) continue;

            if (strcmp(movies[i].title, movies[j].title) == 0 && i < j)
            {
                movies[j].pointsToFirst = &movies[i];
            }
        }
    }    
}

void displayAllMovies(struct dataSet *movies, int items, int genreCount, char **genres)
{
    int jumpTo = 1;
    printSample(movies, items, jumpTo, genreCount, genres);
}
