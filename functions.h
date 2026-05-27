#pragma once
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#define MAX_MOVIES 100
#define MAX_TITLE 100
#define MAX_DATE 20
#define FILENAME "movies.bin"
#define FILENAME_BAK "movies.bak" 
#define FILENAME_TXT "movies.txt"
#define FILENAME_TXT_BAK "movies_old.txt" 

#define MENU_ADD 1
#define MENU_READ 2
#define MENU_UPDATE 3
#define MENU_DELETE 4
#define MENU_INSERT 5
#define MENU_SORT 6
#define MENU_SAVE 7
#define MENU_SEARCH 8
#define MENU_EXIT 0


#define IS_VALID_RATING(r) ((r) >= 0.0f && (r) <= 10.0f)

typedef enum {
	ACTION,
	DRAMA,
	COMEDY,
	HORROR,
	SF
} Genre;

typedef struct {
	char title[MAX_TITLE];
	int releaseYear;
	float rating;
	char dateWatched[MAX_DATE];
	Genre movieGenre;
} MOVIE;

typedef int (*CmpFn)(const void*, const void*);

extern const char* genreNames[];

static inline void clearBuffer(void) {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}


void createMovie(MOVIE movies[], int* count);
void readMovies(const MOVIE movies[], int count);
void updateMovie(MOVIE movies[], int count);
void insertMovie(MOVIE movies[], int* count, int position);
void deleteMovie(MOVIE movies[], int* count);


void saveMovies(const MOVIE movies[], int count);
void loadMovies(MOVIE movies[], int* count);


void sortMovies(MOVIE movies[], int count);
int searchMovie(MOVIE movies[], int count, const char* title);


void quickSort(MOVIE arr[], int low, int high);


void safeFreeMOVIES(MOVIE** ptr);

void exportMoviesTXT(const MOVIE movies[], int count);
void importMoviesTXT(MOVIE movies[], int* count);
void deleteExportFile(void);
void backupMovies(void);

void sortMoviesBy(MOVIE movies[], int count, CmpFn cmp);
void sortMoviesByRating(MOVIE movies[], int count);

int searchMovieByYear(MOVIE movies[], int count, int year);

#endif
