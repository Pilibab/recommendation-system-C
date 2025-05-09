#include <stdio.h>

#include "regression.h"
#include "prediction.h"
#include "structures.h"

void predictMovie(float w[], struct unseen * notWatched, struct User * targetuser)
{
    struct unseen * temp = notWatched;
    float averageByTargetU = targetuser->sumOfRate / targetuser->countRate;

    while (temp != NULL)
    {
        float f[5] = {
            1.0,                                        // Correspond to the bias term 
            (temp->similaritySum / temp->neighborCount), 
            temp->neighborCount, 
            temp->predictRate,                   //weighted average
            averageByTargetU,
            };
        
        float prob = probability(w, f);

        if (prob > .5)
            printf("ud love this movie %d probability = %.2f\n", temp->movieId, prob);
        else
            printf("loss\n");

        temp = temp->next;
    }
}

/**
 * Predict likelihood by KNN
 */
void predictRate(struct unseen * unseenHead)
{
    struct unseen * temp = unseenHead;

    while (temp != NULL)
    {
        float predictedRate = temp->weightedSum / temp->similaritySum;
        temp->predictRate = predictedRate;
        temp = temp->next;
    }
}