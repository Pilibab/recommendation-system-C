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
    int countRate;                              // counts the no. of time the user has rated a movie
    double sumOfRate;                           // adds all the rate that the user made 
    struct MovieRating *ratings; 
};

struct dataSet
{
    char title[100];
    int genreFeature[GENRESIZE];
    float weightedFeature[GENRESIZE]; 
}; 
struct ratingsTopN
{
    int movieId;
    int rating[2];
    float pearsonScore; 
    struct ratingsTopN * next; 
};

typedef struct {
    int userId;
    int similarCount;
    struct ratingsTopN * theirMovies; 
} SimilarUser;


#endif 
