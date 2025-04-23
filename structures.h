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
    int rating[2];                              //0->A, 1-> B
    struct ratingsTopN * next; 
};

struct topSimiliarUser                          //arr of top pearson
{
    float pearsonScore; 
    int userId;
    struct ratingsTopN * seenMovies; 
    struct unseen * unseenMovies;
};

struct unseen { 
    int movieId;
    float weightedSum;    // ∑(similarity × rating)
    float similaritySum;  // ∑|similarity|
    int neighborCount;    // how many neighbors rated this movie
    struct unseen *next;  // optional: for linked list chaining
};

typedef struct {
    int userId;
    int similarCount;
    struct ratingsTopN * theirMovies; 
} SimilarUser;


#endif 
