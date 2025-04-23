#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pearson.h"
#include "structures.h"
#define NEIGHBOR 10

#define sqr(x) (x * x)

void getUnseenMovies(struct topSimiliarUser * pears, struct User targetUser, struct User database[])
{
    for (int i = 0; i < NEIGHBOR; i++)
    {
        int index = pears[i].userId;
        struct MovieRating *tempA = targetUser.ratings;
        struct MovieRating *tempB = database[index].ratings;
        
        struct unseen *current = NULL;
        struct unseen *head = NULL;

        while (tempA != NULL && tempB != NULL)
        {
            if (tempA->movieId == tempB->movieId)
            {
                tempA = tempA->next;
                tempB = tempB->next;
            }
            else if(tempA->movieId < tempB->movieId)
                tempA = tempA->next;
            else
            {
                tempB = tempB->next;

                // Create new node
                struct unseen *newNode = (struct unseen *)malloc(sizeof(struct unseen));

                // put data in node
                newNode->movieId = tempA->movieId;

                newNode->next = NULL;
                
                if (head == NULL)                                   // insert head on first time 
                {
                    head = newNode;
                    current = newNode;
                    newNode->neighborCount = 1; 
                    newNode->similaritySum = pears[i].pearsonScore;
                    newNode->weightedSum = pears[i].pearsonScore * tempB->rating;
                } else
                {
                    newNode->neighborCount ++; 
                    newNode->similaritySum += pears[i].pearsonScore;
                    newNode->weightedSum += (pears[i].pearsonScore * tempB->rating);
                    
                    current -> next = newNode;
                    current = newNode;
                }     
            }
        }
        pears[i].unseenMovies = head;
    }
}
/***
 * calculate pearson for each top k neighbor 
 */
void pearsonCorrelation(SimilarUser * topKthN, struct User getUserInfo[], struct User userA, struct topSimiliarUser * pears)
{
    for (int nearest10 = 0; nearest10 < NEIGHBOR; nearest10++)
    {

        int userId = topKthN[nearest10].userId;
        int no_of_rate = getUserInfo[userId].countRate;
        double count_UB = getUserInfo[userId].sumOfRate, count_UA = userA.sumOfRate;

        float average_UB = (count_UB / no_of_rate);
        float average_UA = (count_UA / no_of_rate); 

        struct ratingsTopN * curr_UA = topKthN[nearest10].theirMovies;

        float dotProduct_deMean = 0;
        float preMagDemean_UA = 0, preMagDemean_UB = 0;                     //preMag - before getting the mag niture 

        while (curr_UA != NULL )
        {
            float demean_UA = (curr_UA->rating[0] - average_UA);
            float demean_UB = (curr_UA->rating[1] - average_UB);

            dotProduct_deMean += demean_UA * demean_UB; 
            preMagDemean_UA += sqr(demean_UA); 
            preMagDemean_UB += sqr(demean_UB); 

            curr_UA = curr_UA -> next;
        }   
        pears[nearest10].userId = userId;
        pears[nearest10].pearsonScore =  dotProduct_deMean / (sqrt(preMagDemean_UA) * sqrt(preMagDemean_UB));
    }
}
/**
 * compiles the top 10 most similar user (interms of no of similar watched movie)
 */
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

void getRateOfMovie(struct ratingsTopN *arr, 
    struct User indexUser, 
    struct User toCompare[], SimilarUser *topK)
{
    for (int i = 0; i < NEIGHBOR; i++)
    {
        int index = topK[i].userId;
        struct MovieRating *tempA = indexUser.ratings;
        struct MovieRating *tempB = toCompare[index].ratings;
        
        
        struct ratingsTopN *current = NULL;
        struct ratingsTopN *head = NULL;

        while (tempA != NULL && tempB != NULL)
        {
            if (tempA->movieId == tempB->movieId)
            {
                // Create new node
                struct ratingsTopN *newNode = (struct ratingsTopN *)malloc(sizeof(struct ratingsTopN));
                
                // put data in node
                newNode->movieId = tempA->movieId;
                newNode->rating[0] = tempA->rating;
                newNode->rating[1] = tempB->rating;
                newNode->next = NULL;
                
                if (head == NULL)                                   // insert head on first time 
                {
                    head = newNode;
                    current = newNode;
                } else
                {
                    current -> next = newNode;
                    current = newNode;
                }

                tempA = tempA->next;
                tempB = tempB->next;
            }
            else if(tempA->movieId < tempB->movieId)
                tempA = tempA->next;
            else
                tempB = tempB->next;
        }
        topK[i].theirMovies = head;
    }
}