#pragma once
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#define MAX_MOVIES  100
#define MAX_TITLE   100
#define MAX_DATE    20
#define FILENAME    "movies.bin"

#define MENU_ADD    1
#define MENU_READ   2
#define MENU_UPDATE 3
#define MENU_DELETE 4
#define MENU_SEARCH 5
#define MENU_SORT   6
#define MENU_SAVE   7
#define MENU_LOAD   8
#define MENU_EXIT   0


#define IS_VALID_RATING(r) ((r) >= 0.0f && (r) <= 10.0f)

typedef enum {
    ACTION,
    DRAMA,
    COMEDY,
    HORROR,
    SF
} Genre;

typedef struct {
    char  title[MAX_TITLE];
    int   releaseYear;
    float rating;
    char  dateWatched[MAX_DATE];
    Genre movieGenre;
} MOVIE;


extern const char *genreNames[];

/* inline funkcija */
static inline void clearBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void createMovie(MOVIE movies[], int *count);
void readMovies(const MOVIE movies[], int count);
void updateMovie(MOVIE movies[], int count);
void insertMovie(MOVIE movies[], int *count, int position);
void deleteMovie(MOVIE movies[], int *count);


void saveMovies(const MOVIE movies[], int count);
void loadMovies(MOVIE movies[], int *count);


void sortMovies(MOVIE movies[], int count);
int  searchMovie(MOVIE movies[], int count, const char *title);


void quickSort(MOVIE arr[], int low, int high);

#endif
