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
    
    FILE *infoFile = fopen("ml-100k\\u.info", "r");
    FILE *itemFile = fopen("ml-100k\\u.item", "r");
    FILE *dataFile = fopen("ml-100k\\u.data", "r");

    FILE *userCookie = fopen("user-data\\u.txt", "a+");

    int usersCount, items, ratings;

    //check if file ptr exists
    if (infoFile == NULL)
    {
        printf("Error: <u.info> file could not be opened.\n");
        return 0;
    }
    if (itemFile == NULL)
    {
        printf("Error: <u.item> file could not be opened.\n");
        return 0;
    }    
    if (dataFile == NULL)
    {
        printf("Error: <u.data> file could not be opened.\n");
        return 0;
    }
    if (userCookie == NULL) {
        perror("File creation failed");
    }



    //scan info file for num of user items and ratings
    fscanf(infoFile, "%d users", &usersCount);
    fscanf(infoFile, "%d items", &items);
    fscanf(infoFile, "%d ratings", &ratings);





    //create arr struct to store movie data set
    struct dataSet movies[items]; 

    for (int i = 0; i < items ; i++)
    {
        movies[i].pointsToFirst = NULL;
    }

    //initialize arr and counter related to vectorization
    double genreFreqCounter[GENRESIZE] = {0}; 
    float weights[GENRESIZE];    
    int validMovieCount = 0;  

    // Pass parameters to loadData()
    printf("loading data...\n");
    loadData(movies, items, itemFile, &validMovieCount, genreFreqCounter);

    // get user data (u)
    struct User targetUser = getTargetUserMovies(userCookie, movies);


    // Now pass movies to addWeight()
    printf("\nadding weights...\n");
    addWeight(movies, validMovieCount, weights, genreFreqCounter);
    // printSample(movies);



    // Check for duplicates 
    int count = 1; 

    for (int i = 0 ; i < items; i++)
    {
        for (int j = 0; j < items; j++)
        {
            if (i == j) continue;

            if (strcmp(movies[i].title, movies[j].title) == 0 && i < j)
            {
                movies[j].pointsToFirst = &movies[i];
                printf("%d) Duplicate %d = %d: %s\n", count, i, j, movies[i].title);
                count++;
            }
        }
    }    

    
    struct User users[usersCount];
    struct topSimiliarUser topPearsed[usersCount];    

    printf("creating movie matrix...\n");
    createUsermovieMatrix(dataFile, usersCount, items, ratings, movies, users, topPearsed);


    struct unseen *listofUnwatched = NULL;                                  //head of linked list (movies that the target user hasnt watched)    


    // Create movie and User matrix 
    topNeighboor(targetUser, users, usersCount, topPearsed);
    getUnseenMovies(topPearsed, &listofUnwatched, users);

    // Update listOfUnwatched
    listofUnwatched = setThreshold(listofUnwatched);

    predictRate(listofUnwatched);

    struct unseen *listOfWatched = NULL;   

    printf("getting watched\n");
    watched(topPearsed, &listOfWatched, users);
    predictRate(listOfWatched);

    // Initial weights
    float w[5] = {1,0,0,0,0};  

    if (targetUser.countRate > 20)                                          // If no of rated met the minimum reqment
    {
        epoch(&listOfWatched, &targetUser, 400, w);
    }

    printf("\nnew weights: \n");
    float prevW[5]= {1,0,0,0,0};

    for (int i = 0 ; i<5 ;i++)
    {
        printf("\t%f: delta_w[%d] = %.6f\n", w[i], i, w[i] - prevW[i]);
    }

    // Making prediction 
    // predictMovie(w, listofUnwatched, &targetUser);

    // printSampleLinked(users);
    
    fclose(infoFile);
    fclose(itemFile);
    fclose(dataFile);
    return 0;
}


