#ifndef PREDICTION_H
#define PREDICTION_H

    void predictMovie(float w[], struct unseen * notWatched, struct User * targetuser);
    void predictRate(struct unseen * unseenHead);

#endif