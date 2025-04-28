#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pearson.h"
#include "structures.h"
#define NEIGHBOR 10
#define MAXTHRESHOLD 4

#define sqr(x) (x * x)



void getUnseenMovies(struct topSimiliarUser * pears, struct unseen ** unseenHead, struct  User toCompare[])
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
                insertUnwatched(unseenHead, allMoviesDb->movieId, allMoviesDb->rating ,pears[i].pearsonScore);
                allMoviesDb = allMoviesDb->next;
            } else 
            {
                if ((curr->movieId == allMoviesDb->movieId))
                {
                    allMoviesDb = allMoviesDb->next;
                    curr = curr->next;
                } 
                else if ((curr->movieId > allMoviesDb->movieId )){
                    //movie not seen by user 
                    insertUnwatched(unseenHead, allMoviesDb->movieId, allMoviesDb->rating ,pears[i].pearsonScore);
                    allMoviesDb = allMoviesDb->next;
                }   else 
                    // This shouldn't happen if commonMovies is a subset of allMoviesDb (meaning allMoviesDb is from curr)
                    curr = curr->next;  
            }
        }
    }
}

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

void insertUnwatched(struct unseen **head, 
    int movieId, 
    float rating, 
    float similarity)
{

    // if (movieId == 0) {
    //     printf("Adding or updating movie ID 0: similarity=%.2f, rating=%.2f, current count=%d\n", 
    //            similarity, rating, (*head)->neighborCount);
    // }

    // Check if list is empty
    if (*head == NULL) {
        struct unseen *newNode = (struct unseen *)malloc(sizeof(struct unseen));
        newNode->movieId = movieId;
        newNode->similaritySum = similarity;
        newNode->weightedSum = similarity * rating;
        newNode->neighborCount = 1;
        newNode->next = NULL;
        *head = newNode;
        return;
    }
    
    // Check if movie should be inserted at head (smaller ID)
    if (movieId < (*head)->movieId) {
        struct unseen *newNode = (struct unseen *)malloc(sizeof(struct unseen));
        newNode->movieId = movieId;
        newNode->similaritySum = similarity;
        newNode->weightedSum = similarity * rating;
        newNode->neighborCount = 1;
        newNode->next = *head;
        *head = newNode;
        return;
    }
    
    // Check if movie already exists at head
    if ((*head)->movieId == movieId) {
        (*head)->similaritySum += similarity;
        (*head)->weightedSum += similarity * rating;
        (*head)->neighborCount++;
        return;
    }
    
    // Find position in sorted list
    struct unseen *curr = *head;
    while (curr->next != NULL && curr->next->movieId < movieId) {
        curr = curr->next;
    }
    
    // Check if movie already exists
    if (curr->next != NULL && curr->next->movieId == movieId) {
        curr->next->similaritySum += similarity;
        curr->next->weightedSum += similarity * rating;
        curr->next->neighborCount++;
    } else {
        // Insert new node
        struct unseen *newNode = (struct unseen *)malloc(sizeof(struct unseen));
        newNode->movieId = movieId;
        newNode->similaritySum = similarity;
        newNode->weightedSum = similarity * rating;
        newNode->neighborCount = 1;
        newNode->next = curr->next;
        curr->next = newNode;
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

    double count_UB = UserB->sumOfRate, count_UA = UserA->sumOfRate;

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
        if (i == 1) continue;
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
        if (countSimilar > MAXTHRESHOLD)
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