#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pearson.h"
#include "structures.h"
#include "clean_pearson.h"


struct unseen* setThreshold(struct unseen *head)
{
    struct unseen *curr = head, *prev = NULL;

    while (curr != NULL)
    {
        if (curr->neighborCount < 3)
        {
            struct unseen *toDelete = curr;

            if (prev == NULL)  
            {
                head = curr->next;
                curr = head;
            } else {
                prev->next = curr->next; 
                curr = curr->next;
            }
            free(toDelete); 
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    return head;  
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