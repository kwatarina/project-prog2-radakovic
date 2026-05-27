#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "functions.h"

int main(void) {
	MOVIE* movies = calloc(MAX_MOVIES, sizeof(MOVIE));
	if (!movies) { fprintf(stderr, "calloc failed\n"); return 1; }
	int count = 0;
	int choice;

	loadMovies(movies, &count);

	do {
		printf("\n=== MOVIE MANAGER ===\n");
		printf("%d. Dodaj film\n", MENU_ADD);
		printf("%d. Prikazi filmove\n", MENU_READ);
		printf("%d. Uredi film\n", MENU_UPDATE);
		printf("%d. Obrisi film\n", MENU_DELETE);
		printf("%d. Umetni film\n", MENU_INSERT);
		printf("%d. Sortiraj\n", MENU_SORT);
		printf("%d. Spremi\n", MENU_SAVE);
		printf("%d. Pretrazi\n", MENU_SEARCH);
		printf("%d. Izlaz\n", MENU_EXIT);
		printf("Odabir: ");
		scanf("%d", &choice);

		switch (choice) {
		case MENU_ADD:
			createMovie(movies, &count);
			break;

		case MENU_READ:
			readMovies(movies, count);
			break;

		case MENU_UPDATE:
			updateMovie(movies, count);
			break;

		case MENU_DELETE:
			deleteMovie(movies, &count);
			break;

		case MENU_INSERT: {
			printf("Na koju poziciju umetnut (1-%d): ", count + 1);
			int pos; scanf("%d", &pos);
			insertMovie(movies, &count, pos - 1);
			break;
		}

		case MENU_SORT: {
			printf("\n Sortiraj po:\n");
			printf(" 1. Naslovu\n");
			printf(" 2. Ocjeni\n");
			printf(" Odabir: ");
			int sub; scanf("%d", &sub);
			if (sub == 1) {
				sortMovies(movies, count); 
			}
			else if (sub == 2) {
				sortMoviesByRating(movies, count);
			}
			else {
				printf("Neispravan odabir.\n");
			}
			readMovies(movies, count);
			break;
		}

		case MENU_SAVE:
			backupMovies(); 
			saveMovies(movies, count);
			exportMoviesTXT(movies, count); 
			break;

		case MENU_SEARCH: {
			printf("\n Pretrazi po:\n");
			printf(" 1. Naslovu\n");
			printf(" 2. Godini\n");
			printf(" Odabir: ");
			int sub; scanf("%d", &sub);

			if (sub == 1) {
				clearBuffer();
				char title[MAX_TITLE];
				printf("Naslov: ");
				fgets(title, MAX_TITLE, stdin);
				title[strcspn(title, "\n")] = '\0';
				sortMovies(movies, count); 
				int idx = searchMovie(movies, count, title); 
				if (idx >= 0)
					printf("Pronadeno: %s (%d)\n", movies[idx].title, movies[idx].releaseYear);
				else
					printf("Nije pronadeno.\n");

			}
			else if (sub == 2) {
				printf("Godina: ");
				int yr; scanf("%d", &yr);
				int idx = searchMovieByYear(movies, count, yr);
				if (idx >= 0)
					printf("Pronadeno: %s (%d)\n", movies[idx].title, movies[idx].releaseYear);
				else
					printf("Nije pronadeno.\n");
			}
			else {
				printf("Neispravan odabir.\n");
			}
			break;
		}

		case MENU_EXIT:
			saveMovies(movies, count); 
			exportMoviesTXT(movies, count); 
			deleteExportFile(); 
			printf("Dovidenja!\n");
			break;

		default:
			printf("Neispravan unos.\n");
		}
	} while (choice != MENU_EXIT);

	safeFreeMOVIES(&movies);
	return 0;
}
