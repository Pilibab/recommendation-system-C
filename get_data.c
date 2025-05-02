#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "vect_ops.h"
#include "vectorizer.h"
#include "structures.h"
#include "utilities.h"

void loadData(struct dataSet movies[], int items, FILE *itemFile, int *validMovieCount, double genreFreqCounter[]) {
    char line[1000];

    for (int i = 0; i < items; i++) {
        if (fgets(line, sizeof(line), itemFile) == NULL)  break;

        // Remove newline
        line[strcspn(line, "\n")] = 0;

        // Initialize variable needed for validity check (complete data)
        int j = 0;
        int valid = 1;


        char *token = strtok(line, "|");
        if (!token) continue;

        token = strtok(NULL, "|");                                      // Title

        if (!token) continue;

        // Add title to struct->title
        strncpy(movies[*validMovieCount].title, token, sizeof(movies[*validMovieCount].title) - 1);
        movies[*validMovieCount].title[sizeof(movies[*validMovieCount].title) - 1] = '\0';


        token = strtok(NULL, "|");                                      // Release Date

        // Check if token is already at genre field -> such case were eminent when the release date and imbd link were missing
        // Note: maybe put this logic in 1 function for cleaner and more modular approach
        if (strcmp(token, "0") == 0 || strcmp(token, "1") == 0) {

            movies[*validMovieCount].genreFeature[0] = atoi(token);
            for (j = 1; j < 19; j++) {    // 19 genres
                token = strtok(NULL, "|");
                if (token == NULL) {
                    valid = 0;
                    break;
                }
                movies[*validMovieCount].genreFeature[j] = atoi(token);
            }
    
            if (valid && j == 19) {
                freqGenreCounter(movies[*validMovieCount], genreFreqCounter);
                (*validMovieCount)++;
            } else {
                printf("Skipped movie at line %d (only %d genres)\n", i+1, j);
            }
            continue;
        }

        // Check if the next token looks like genre field
        token = strtok(NULL, "|");                                      // IMDB Link 

        // Check: if token is "0" or "1" assert if true then its (first genre number!)
        if (strcmp(token, "0") == 0 || strcmp(token, "1") == 0) {
            movies[*validMovieCount].genreFeature[0] = atoi(token);
            for (j = 1; j < 19; j++) {    // 19 genres
                token = strtok(NULL, "|");
                if (token == NULL) {
                    valid = 0;
                    break;
                }
                movies[*validMovieCount].genreFeature[j] = atoi(token);
            }
    
            if (valid && j == 19) {
                freqGenreCounter(movies[*validMovieCount], genreFreqCounter);
                (*validMovieCount)++;
            } else {
                printf("Skipped movie at line %d (only %d genres)\n", i+1, j);
            }
            continue;
        }

        // Append  19 genre to struct->genreFeature
        for (j = 0; j < 19; j++) 
        {    
            token = strtok(NULL, "|");
            if (token == NULL) {
                valid = 0;
                break;
            }
            movies[*validMovieCount].genreFeature[j] = atoi(token);
        }

        // Check: for missing genre if complete we count for TF-IDF
        if (valid && j == 19) {
            freqGenreCounter(movies[*validMovieCount], genreFreqCounter);
            (*validMovieCount)++;
        } else {
            printf("Skipped movie at line %d (only %d genres)\n", i+1, j);
        }
    }
}













