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
 * A is target user 
 * B is from data base
 */
void pearsonCorrelation(struct ratingsTopN * listSameMovies , 
    struct topSimiliarUser * pears,  
    struct User * UserA,
    struct User * UserB,
    int UserB_ID)
{

    int no_of_rate_UA= UserA->countRate,  no_of_rate_UB = UserB->countRate;

    double count_UB = UserB->sumOfRate, count_UA = UserA->countRate;

    float average_UB = (count_UB / no_of_rate_UB);
    float average_UA = (count_UA / no_of_rate_UA); 


    float dotProduct_deMean = 0;
    float preMagDemean_UA = 0, preMagDemean_UB = 0;                     //preMag - before getting the mag niture 

    struct ratingsTopN * head = listSameMovies; 
    struct ratingsTopN * curr = listSameMovies; 
    while (curr != NULL)
    {
        float demean_UA = (curr->rating[0] - average_UA);
        float demean_UB = (curr->rating[1] - average_UB);

        dotProduct_deMean += demean_UA * demean_UB; 
        preMagDemean_UA += sqr(demean_UA); 
        preMagDemean_UB += sqr(demean_UB); 

        curr = curr -> next;
    }   
    float pScore = dotProduct_deMean / (sqrt(preMagDemean_UA) * sqrt(preMagDemean_UB));
    insertPos(pears, UserB_ID, pScore, listSameMovies);
}
/**
 * compiles the top 10 most similar user (interms of no of similar watched movie)
 */
void topNeighboor(struct User indexUser, 
    struct User toCompare[], 
    int user, struct topSimiliarUser * pears)
{
    for (int i = 1; i < user + 1; i++)
    {
        struct MovieRating * tempA = toCompare[i].ratings;                  //arr struct
        struct MovieRating * tempB = indexUser.ratings;                     //user to comp

        struct User * UserA = &indexUser;
        struct User * UserB = &toCompare[i];

        struct ratingsTopN *head = NULL;
        struct ratingsTopN *current = NULL;

        int countSimilar = 0;
        while (tempA != NULL && tempB != NULL)
        {
            if (tempA->movieId == tempB->movieId)
            {
                // printf("flag\n");
                struct ratingsTopN *newNode = getRateOfMovie(tempB, tempA);

                countSimilar++;
                tempA = tempA->next;
                tempB = tempB->next;

                // printf("flag\n");

                if (head == NULL)
                {
                    head = newNode;
                    current = newNode;
                } else
                {
                    current->next = newNode;
                    current = newNode;
                }
            }
            else if(tempA->movieId < tempB->movieId)
                tempA = tempA->next;
            else
                tempB = tempB->next;
        }
        if (countSimilar > 5)
        {      
            pearsonCorrelation(head, pears, UserA, UserB, i);
        } else
            freeLinkedList(head);                                   //free space
    } 
}

/**
 * modify topNeigbor array (sort and insertion), insert then shift
 * check if the array size is 10 before inserting (over write the last element)
 */
void insertPos(struct topSimiliarUser * pears, int UserID, float pScore, struct ratingsTopN * listSameMovies ) 
{
    int insertIndex = -1;
    for (int i = 0; i < NEIGHBOR; i++)
    {
        if (pears[i].pearsonScore == 0.0f) { // Empty slot
            insertIndex = i;
            break;
        } 
        else if (pScore > pears[i].pearsonScore) {
            insertIndex = i;
            break;
        }
    }
    if (insertIndex == -1) return;

    for (int i = NEIGHBOR - 1; i > insertIndex; i--) pears[i] = pears[i - 1];
    pears[insertIndex].userId = UserID;
    pears[insertIndex].pearsonScore = pScore;
    pears[insertIndex].seenMovies = listSameMovies;
    
}

struct ratingsTopN * getRateOfMovie(struct MovieRating *a, struct MovieRating *b)
{
    struct ratingsTopN *newNode = (struct ratingsTopN *)malloc(sizeof(struct ratingsTopN));
    newNode->movieId = a->movieId;
    newNode->rating[0] = a->rating;
    newNode->rating[1] = b->rating;
    newNode->next = NULL;
    return newNode;
}

void freeLinkedList(struct ratingsTopN * head )
{
    struct ratingsTopN *temp = NULL;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}