#include <math.h>
#include <stdio.h>


#include "regression.h"
#include "pearson.h"

/**
 * using structure for unseen but im really getrting the watched movies here
 */
struct unseen watched(struct topSimiliarUser * pears, 
    struct unseen ** watchedHead, 
    struct  User toCompare[])
{

    for (int i = 0; i < NEIGHBOR; i++)
    {
        int index = pears[i].userId;

        struct ratingsTopN * curr = pears[i].seenMovies;                        //watched movies by user and index Neigh
        struct  MovieRating * allMoviesDb = toCompare[index].ratings;           //all movies Data base User watched -> get unwatched 
        while (allMoviesDb != NULL)
        {

            if (curr == NULL) 
            {
                insertUnwatched(watchedHead, allMoviesDb->movieId, allMoviesDb->rating ,pears[i].pearsonScore);
                allMoviesDb = allMoviesDb->next;
            } else 
            {
                if ((curr->movieId == allMoviesDb->movieId)) {
                    insertUnwatched(watchedHead, allMoviesDb->movieId, allMoviesDb->rating ,pears[i].pearsonScore);
                    allMoviesDb = allMoviesDb->next;
                    curr = curr->next;
                } 
                else if ((curr->movieId > allMoviesDb->movieId )){
                    //movie not seen by user 
                    allMoviesDb = allMoviesDb->next;
                }   else 
                    // This shouldn't happen if commonMovies is a subset of allMoviesDb (meaning allMoviesDb is from curr)
                    curr = curr->next;  
            }
        }
    }
}

/**
 * f[0]	Average similarity with K neighbors who rated the movie.
 * f[1]	Number of neighbors who rated the movie (maybe normalized).
 * f[2]	Average rating from those neighbors.
 * f[3]	User’s own average rating across all movies.
 * f[4]	Always 1.0 (bias trick).
 * 
 * computes sigmoid probability 
 */
float probability (float w[], float f[])
{
    return 1 / (1 + exp(-z(w,f)));
}

float z(float w[], float f[])
{
    return( w[0] * f[0] +  
        w[1] * f[1] + w[2] * f[2] + 
        w[3] * f[3] + w[4] * f[4] );
}

/**
 * Log loss=−(ylog(y^)+(1−y)log(1− y^))
 *      where:  y^ || y hat == sigmoid probability 
 */
float loss(struct MovieRating * movieInfo,  float y_hat, int y)
{
    return -(y * log10(y_hat) + (1 - y) * log10(1 - y_hat));
}

void updateWeights(float w[], float f[], float alpha, float y_hat, float y)
{
    float error =  y_hat - y;
    w[0] = w[0] - alpha * error * f[0];
    w[1] = w[1] - alpha * error * f[1];
    w[2] = w[2] - alpha * error * f[2];
    w[3] = w[3] - alpha * error * f[3];
    w[4] = w[4] - alpha * error * f[4];
}

void epoch( struct unseen ** watchedHead, struct User * targetuser, int epochCount, float w[])
{
    const float alpha = 0.01;                                                               //learning rate
    float averageByTargetU = targetuser->sumOfRate / targetuser->countRate;

    printf("starting epoch\n");
    
    for (int i = 0 ; i <= epochCount; i++)
    {
        struct unseen * tempWatched = *watchedHead;
        struct MovieRating * tempMovieInfo = targetuser->ratings;
    
        // For tracking total loss
        float total_loss = 0.0;
    
        int test = 0;                                                                       // Use first 10 movies only for prediction                           
        while (tempWatched != NULL && test <= 10)
        {
            if (i == 0)
            {
                printf("movie id: %d\n", tempWatched->movieId);
            }
            // Initialize feature 
            float f[5] = {
                1.0,                                        // Correspond to the bias term 
                (tempWatched->similaritySum / tempWatched->neighborCount), 
                tempWatched->neighborCount, 
                tempWatched->predictRate,                   //weighted average
                averageByTargetU,
                };


            // Match tempMovie info (to get rating) to movie id of watched by Neighbor
            if (tempMovieInfo->movieId != tempWatched->movieId)
            {
                while (tempMovieInfo->movieId < tempWatched->movieId)
                {
                    tempMovieInfo = tempMovieInfo -> next;
                }
            }

            // Calculate predictions
            float y_hat = probability(w, f);

            // assert if user liked movie (determine true label)
            float y =  (tempMovieInfo->rating >= 4)? 1.0: 0.0;                                      // Consider rate higher than 4 liked

            // cal and track loss
            float currentLoss = loss(tempMovieInfo, y_hat, y);
            total_loss += currentLoss;
            
            // Update the weights thru gradient step
            updateWeights(w, f, alpha, y_hat, y);

            tempMovieInfo = tempMovieInfo -> next;
            tempWatched = tempWatched -> next;
            test++;
        }
        // Note: This is only for debugging to see if the sigmoid function is regressing (learning, more accurate to say -> fitting)
        if (i % 10 == 0)
        {
            printf("\tepoch: %d", i);
            printf("\tAverage log loss for epoch: %.5lf\n", total_loss / test);
        }
    }

    printf("\nTesting model on unseen data (not used in training)...\n\n");
    struct unseen *tempTest = *watchedHead;
    struct MovieRating *temptestMovieInfo = targetuser->ratings;
    int testIndex = 0;
    int correct = 0;
    int totalTested = 0;
    float totalTestLoss = 0.0;

    // skip the first 10 training examples
    while (tempTest != NULL && testIndex < 11) {
        tempTest = tempTest->next;
        testIndex++;
    }

    while (tempTest != NULL) {
        float f[5] = {
            1.0,
            (tempTest->similaritySum / tempTest->neighborCount), 
            tempTest->neighborCount, 
            tempTest->predictRate, 
            averageByTargetU,
        };

        // align to movieId (same logic as before)
        while (temptestMovieInfo != NULL && temptestMovieInfo->movieId < tempTest->movieId)
            temptestMovieInfo = temptestMovieInfo->next;

        if (temptestMovieInfo == NULL) break;

        float y_hat = probability(w, f);
        float y = (temptestMovieInfo->rating >= 4) ? 1.0 : 0.0;
        float lossVal = loss(temptestMovieInfo, y_hat, y);
        totalTestLoss += lossVal;

        // Count as correct if predicted label matches
        int predicted = (y_hat >= 0.5) ? 1 : 0;
        if (predicted == y) correct++;

            printf("\ttest log loss: %.5f\tMovie id: %6d\n",lossVal, tempTest->movieId);
        totalTested++;
        temptestMovieInfo = temptestMovieInfo->next;
        tempTest = tempTest->next;
    }

    printf("\nTest accuracy: %.2f%% (%d/%d correct)\n", (100.0 * correct) / totalTested, correct, totalTested);
    printf("Average test log loss: %.5f\n", totalTestLoss / totalTested);
}