#include <stdio.h>
#include <stdlib.h>

#include "regression.h"
#include "prediction.h"
#include "structures.h"


/**
 * copy's the data and sort them for printing 
 */
void predictMovie(float w[], struct unseen * notWatched, struct User * targetuser) 
{
    struct unseen *temp = notWatched;
    float averageByTargetU = targetuser->sumOfRate / targetuser->countRate;
    
    // Create a sorted copy of the list in descending order
    struct unseen *sortedList = NULL;
    struct unseen *current = notWatched;
    
    // Iterate through the original list
    while (current != NULL) {
        // Create a new node to add to our sorted list
        struct unseen *newNode = (struct unseen *)malloc(sizeof(struct unseen));
        if (newNode == NULL) {
            printf("Memory allocation failed\n");
            return;
        }
        
        // Copy all data from the current node
        *newNode = *current;
        newNode->next = NULL;  // Reset the next pointer
        
        // Insert into sorted list (descending order by predictRate)
        if (sortedList == NULL || current->predictRate > sortedList->predictRate) {
            // Insert at beginning
            newNode->next = sortedList;
            sortedList = newNode;
        } else {
            // Find the position to insert
            struct unseen *temp = sortedList;
            while (temp->next != NULL && temp->next->predictRate >= current->predictRate) {
                temp = temp->next;
            }
            // Insert after temp
            newNode->next = temp->next;
            temp->next = newNode;
        }
        current = current->next;
    }
    
    // Process and display the sorted results
    temp = sortedList;
    while (temp != NULL) 
    {
        float f[5] = {
            1.0,                                        // Correspond to the bias term 
            (temp->similaritySum / temp->neighborCount),
            temp->neighborCount,
            temp->predictRate,                          // weighted average
            averageByTargetU,
        };
        
        float prob = probability(w, f);
        if (prob > .5)
            printf("You'd love this movie %d (Rating: %.2f) probability = %.2f\n", 
                   temp->movieId, temp->predictRate, prob);
        
        temp = temp->next;
    }
    
    // Free the sorted list
    while (sortedList != NULL) {
        struct unseen *temp = sortedList;
        sortedList = sortedList->next;
        free(temp);
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