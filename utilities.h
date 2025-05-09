#ifndef UTILITIES_H
#define UTILITIES_H
#include "structures.h"

    #include <stdio.h>


    void loadData();
    void printSampleLinked(struct User *user);
    void printSample(struct dataSet * movies, int items, int jumpTo, int genreCount, char **genre);
    void printAptGEnre(struct dataSet * movies, int genreCount, char **genre);

    
    int checkFile (FILE * infoFile, 
        FILE * itemFile, 
        FILE * dataFile, 
        FILE * genreFile,
        FILE * userCookie
    );

    void initializeFile(struct dataSet *movies, int items);
    void checkDups(struct dataSet *movies, int items);
    void displayAllMovies(struct dataSet * movies, int items, int genreCount, char **genres);
    char **loadGenres(FILE * genreFile, int *genreCount); 




#endif