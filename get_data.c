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
        if (fgets(line, sizeof(line), itemFile) == NULL) {
            break;
        }
        if (i > 265 && i < 270)

        // Remove newline
        line[strcspn(line, "\n")] = 0;
        int j = 0;
        int valid = 1;


        char *token = strtok(line, "|");
        if (!token) continue; // ID (ignore)

        token = strtok(NULL, "|"); // Title

        if (!token) continue;
        strncpy(movies[*validMovieCount].title, token, sizeof(movies[*validMovieCount].title) - 1);
        movies[*validMovieCount].title[sizeof(movies[*validMovieCount].title) - 1] = '\0';

        token = strtok(NULL, "|"); // Release Date


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
        token = strtok(NULL, "|"); // Next field: either IMDB Link or Genre

        // Now check: if token is "0" or "1" (first genre number!)
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

        for (j = 0; j < 19; j++) {    // 19 genres
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
    }
}















