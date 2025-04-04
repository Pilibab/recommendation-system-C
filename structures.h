#ifndef STRUCTURES_H
#define STRUCTURES_H

#define GENRESIZE 19



// Structure for storing movie ratings
struct MovieRating {
    int movieId;
    int rating;
    struct MovieRating *next;
};

// Structure for storing user data (including ratings)
struct User {
    struct MovieRating *ratings; // Dynamic array of ratings
};

struct dataSet
{
    char title[100];
    int genreFeature[GENRESIZE];
    float weightedFeature[GENRESIZE]; 
}; 

#endif // STRUCTURES_H
