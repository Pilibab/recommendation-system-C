#ifndef STRUCTURES_H
#define STRUCTURES_H

#define GENRESIZE 19



// Structure for storing movie ratings
struct MovieRating {
    int movieId;
    int rating;                             // no of rating
    struct MovieRating *next;
};

// Structure for storing user data (including ratings)
struct User {
    int countRate; 
    double sumOfRate;                          // add all rate 
    struct MovieRating *ratings; 
};

struct dataSet
{
    char title[100];
    int genreFeature[GENRESIZE];
    float weightedFeature[GENRESIZE]; 
}; 

typedef struct {
    int userId;
    int similarCount;
    float pearsonScore; // Add this now so you can store it directly

    struct MovieRating * similarMovie_UA;
    struct MovieRating * similarMovie_UB;
} SimilarUser;


#endif 
