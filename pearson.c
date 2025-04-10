#include <stdio.h>

#include "pearson.h"
#include "structures.h"
#define NEIGHBOR 10


// float pearsonCorrelation(struct User *user)
// {

// }

void topNeighboor(struct User indexUser, struct User toCompare[], int tN[NEIGHBOR][2], int user)
{
    // [[user id][total similar movie], ...,]


    for (int i = 1; i < user; i++)
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
        if (countSimilar >= tN[NEIGHBOR - 1][1]) 
            insertPos(tN, i, countSimilar);
    } 
}

/**
 * modify topNeigbor array (sort and insertion), insert then shift
 * check if the array size is 10 before inserting (over write the last element)
 */
void insertPos(int arr[][2], int UserID, int similarCount) 
{
    int checkArr = 0;

    if (arr[0][0] == 0){         //case: first time inserting
        arr[0][0] = UserID;
        arr[0][1] = similarCount;
    } else {

        int flagShift = 0, 
            i = 0;

        while (i < NEIGHBOR && arr[i][1] >= similarCount)
            {
                i++;
                flagShift = 1;
            }

        if (flagShift)                          //shift array index
        {
            int tempUser = arr[i][0], tempSim  = arr[i][1] ;

            for (int j = NEIGHBOR - 2; j >= i; j--) {
                arr[j + 1][0] = arr[j][0];
                arr[j + 1][1] = arr[j][1];
            }
            
            arr[i][1] = similarCount;
            arr[i][0] = UserID;
        }
    }
}
