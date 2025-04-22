#include <stdio.h>
#include <stdlib.h>

#include "pearson.h"
#include "structures.h"
#define NEIGHBOR 10


float pearsonCorrelation(struct User *user)
{

}

void topNeighboor(struct User indexUser, struct User toCompare[], SimilarUser *similarUsers, int user)
{
    for (int i = 1; i < user + 1; i++)
    {
        int countSimilar = 0;
        struct MovieRating * tempA = toCompare[i].ratings;                  //arr struct
        struct MovieRating * tempB = indexUser.ratings;                    //user to comp

        struct User * UserA = &indexUser;
        struct User * UserB = &toCompare[i];

        while (tempA != NULL && tempB != NULL)
        {
            if (tempA->movieId == tempB->movieId)
            {
                countSimilar++;
                tempA = tempA->next;
                tempB = tempB->next;
            }
            else if(tempA->movieId < tempB->movieId)
                tempA = tempA->next;
            else
                tempB = tempB->next;
        }

        //arr is initially 0.. check if similarity > min similarity in arr
        //im quite the genuis ngnl
        
        // printf("%d: %d\n", i, countSimilar);
        if (countSimilar >= similarUsers[9].similarCount)  
            insertPos(similarUsers, i, countSimilar);
    } 
}

/**
 * modify topNeigbor array (sort and insertion), insert then shift
 * check if the array size is 10 before inserting (over write the last element)
 */
void insertPos(SimilarUser *similarUsers, int UserID, int countSimilar) 
{
    int checkArr = 0, 
        flagShift = 0, 
        i = 0;;

    if (similarUsers[0].similarCount == 0){         //case: first time inserting
        similarUsers[0].userId = UserID;
        similarUsers[0].similarCount = countSimilar;        
    } else {
        while (i < NEIGHBOR && similarUsers[i].similarCount >= countSimilar)
            {
                i++;
                flagShift = 1;
            }

        if (flagShift)                          //shift array index
        {
            int tempUser = similarUsers->userId, tempSim  = similarUsers->similarCount ;

            for (int j = NEIGHBOR - 1; j >= i; j--)         //wr tf did 2 comefrom???
                similarUsers[j + 1] = similarUsers[j];
 
            similarUsers[i].similarCount = countSimilar;
            similarUsers[i].userId = UserID;
        }
    }
}

void getRateOfMovie(struct ratingsTopN *arr, struct User indexUser, struct User toCompare[], SimilarUser *topK)
{
    for (int i = 0; i < NEIGHBOR; i++)
    {
        int index = topK[i].userId;
        struct MovieRating *tempA = indexUser.ratings;
        struct MovieRating *tempB = toCompare[index].ratings;
        
        // The head node is arr[i], and we'll link nodes from its "next" pointer
        struct ratingsTopN *current = &arr[i];
        current->next = NULL;  
        
        while (tempA != NULL && tempB != NULL)
        {
            if (tempA->movieId == tempB->movieId)
            {
                // Create a new ratingsTopN node
                struct ratingsTopN *newNode = (struct ratingsTopN *)malloc(sizeof(struct ratingsTopN));
                
                // put data in node
                newNode->movieId = tempA->movieId;
                newNode->rating[0] = tempA->rating;
                newNode->rating[1] = tempB->rating;
                newNode->pearsonScore = topK[i].similarCount;
                newNode->next = NULL;
                
                // Link new node
                current->next = newNode;
                current = newNode;
                
                tempA = tempA->next;
                tempB = tempB->next;
            }
            else if(tempA->movieId < tempB->movieId)
                tempA = tempA->next;
            else
                tempB = tempB->next;
        }
    }
}