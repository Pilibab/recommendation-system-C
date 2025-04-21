#include <stdio.h>
#include <stdlib.h>

#include "pearson.h"
#include "structures.h"
#define NEIGHBOR 10


// float pearsonCorrelation(struct User *user)
// {

// }

void topNeighboor(struct User indexUser, struct User toCompare[], SimilarUser *similarUsers, int user)
{
    for (int i = 1; i < user + 1; i++)
    {
        int countSimilar = 0;
        struct MovieRating * tempA = toCompare[i].ratings;                  //arr struct
        struct MovieRating * tempB = indexUser.ratings;                    //user to comp

        struct User * UserA = &indexUser;
        struct User * UserB = &toCompare[i];
        
        struct MovieRating * currA = NULL;
        struct MovieRating * currB = NULL;

        struct MovieRating * headA= NULL;
        struct MovieRating * headB = NULL;

        while (tempA != NULL && tempB != NULL)
        {
            if (tempA->movieId == tempB->movieId)
            {
                struct MovieRating * sm_UA = (struct MovieRating *)malloc(sizeof(struct MovieRating));
                struct MovieRating * sm_UB = (struct MovieRating *)malloc(sizeof(struct MovieRating));
                
                //add similar movie from user a
                sm_UA->movieId  = tempA->movieId;
                sm_UA->rating   = tempA->rating;   
                sm_UA -> next = NULL;

                if (headA == NULL) {
                    headA = sm_UA;
                    currA = sm_UA;
                } else {
                    currA -> next =  sm_UA;
                    currA = sm_UA;
                }

                //add sim movie from b 
                sm_UB->movieId  = tempB->movieId;
                sm_UB->rating   = tempB->rating;
                sm_UB -> next = NULL;

                if (headB == NULL) {
                    headB = sm_UB;
                    currB = sm_UB;
                } else {
                    currB -> next =  sm_UB;
                    currB = sm_UA;
                }


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
