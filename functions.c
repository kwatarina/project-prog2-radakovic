#define _CRT_SECURE_NO_WARNINGS
#include "functions.h"


const char* genreNames[] = { "Action", "Drama", "Comedy", "Horror", "SF" };


static int operationCount = 0;

/* ---------- CRUID ---------- */

void createMovie(MOVIE movies[], int* count) {
    if (!count || *count >= MAX_MOVIES) {
        printf("Nema mjesta za novi film.\n");
        return;
    }

    MOVIE* m = &movies[*count];

    printf("Naziv: ");
    clearBuffer();
    fgets(m->title, MAX_TITLE, stdin);
    m->title[strcspn(m->title, "\n")] = '\0';

    printf("Godina: ");
    scanf("%d", &m->releaseYear);

    printf("Ocjena (0-10): ");
    scanf("%f", &m->rating);
    if (!IS_VALID_RATING(m->rating)) m->rating = 0.0f;

    clearBuffer();
    printf("Datum gledanja: ");
    fgets(m->dateWatched, MAX_DATE, stdin);
    m->dateWatched[strcspn(m->dateWatched, "\n")] = '\0';

    printf("Zanr (0=Action 1=Drama 2=Comedy 3=Horror 4=SF): ");
    int g;
    scanf("%d", &g);
    m->movieGenre = (g >= 0 && g < 5) ? (Genre)g : ACTION;

    (*count)++;
    operationCount++;
    printf("Film dodan! (Ukupno operacija: %d)\n", operationCount);
}

void readMovies(const MOVIE movies[], int count) {
    if (!movies || count == 0) { printf("Nema filmova.\n"); return; }

    printf("\n%-3s %-30s %-6s %-6s %-12s %s\n",
        "Br", "Naziv", "God.", "Ocj.", "Datum", "Zanr");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-3d %-30s %-6d %-6.1f %-12s %s\n",
            i + 1,
            movies[i].title,
            movies[i].releaseYear,
            movies[i].rating,
            movies[i].dateWatched,
            genreNames[movies[i].movieGenre]);
    }
}

void updateMovie(MOVIE movies[], int count) {
    if (!movies || count == 0) { printf("Nema filmova.\n"); return; }

    readMovies(movies, count);
    printf("Broj filma za uredivanje: ");
    int idx;
    scanf("%d", &idx);
    if (idx < 1 || idx > count) { printf("Neispravan odabir.\n"); return; }
    idx--;

    clearBuffer();
    printf("Novi naziv [%s]: ", movies[idx].title);
    char buf[MAX_TITLE];
    fgets(buf, MAX_TITLE, stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (strlen(buf) > 0) strncpy(movies[idx].title, buf, MAX_TITLE - 1);

    operationCount++;
    printf("Film azuriran.\n");
}

void insertMovie(MOVIE movies[], int* count, int position) {
    if (!count || *count >= MAX_MOVIES) { printf("Nema mjesta.\n"); return; }
    if (position < 0 || position > *count) position = *count;

 
    for (int i = *count; i > position; i--)
        movies[i] = movies[i - 1];

    memset(&movies[position], 0, sizeof(MOVIE));
    (*count)++;

    /* privremeno postavi count-1 da createMovie upiše na position */
    int tmp = position;
    MOVIE* m = &movies[position];

    printf("Naziv: ");
    clearBuffer();
    fgets(m->title, MAX_TITLE, stdin);
    m->title[strcspn(m->title, "\n")] = '\0';

    printf("Godina: ");
    scanf("%d", &m->releaseYear);
    printf("Ocjena: ");
    scanf("%f", &m->rating);
    clearBuffer();
    printf("Datum: ");
    fgets(m->dateWatched, MAX_DATE, stdin);
    m->dateWatched[strcspn(m->dateWatched, "\n")] = '\0';
    printf("Zanr (0-4): ");
    int g; scanf("%d", &g);
    m->movieGenre = (Genre)(g >= 0 && g < 5 ? g : 0);

    (void)tmp;
    operationCount++;
    printf("Film umetnut na poziciju %d.\n", position + 1);
}

void deleteMovie(MOVIE movies[], int* count) {
    if (!movies || !count || *count == 0) { printf("Nema filmova.\n"); return; }

    readMovies(movies, *count);
    printf("Broj filma za brisanje: ");
    int idx;
    scanf("%d", &idx);
    if (idx < 1 || idx > *count) { printf("Neispravan odabir.\n"); return; }
    idx--;

    printf("Brisem: %s\n", movies[idx].title);
    memset(&movies[idx], 0, sizeof(MOVIE));

    for (int i = idx; i < *count - 1; i++)
        movies[i] = movies[i + 1];

    memset(&movies[*count - 1], 0, sizeof(MOVIE));
    (*count)--;
    operationCount++;
    printf("Film obrisan.\n");
}



void saveMovies(const MOVIE movies[], int count) {
    if (!movies) return;

    FILE* fp = fopen(FILENAME, "wb");
    if (!fp) { perror("fopen"); return; }

    fwrite(&count, sizeof(int), 1, fp);
    fwrite(movies, sizeof(MOVIE), (size_t)count, fp);

    long size = ftell(fp);
    printf("Spremljeno %d filmova (%ld bajtova).\n", count, size);

    if (ferror(fp)) fprintf(stderr, "Greska: %s\n", strerror(errno));
    fclose(fp);
}

void loadMovies(MOVIE movies[], int* count) {
    if (!movies || !count) return;

    FILE* fp = fopen(FILENAME, "rb");
    if (!fp) { perror("fopen"); return; }

    rewind(fp);

    int saved = 0;
    if (fread(&saved, sizeof(int), 1, fp) != 1 || saved < 0 || saved > MAX_MOVIES) {
        if (feof(fp)) printf("Datoteka je prazna.\n");
        else perror("fread");
        fclose(fp);
        return;
    }

    fseek(fp, sizeof(int), SEEK_SET);
    *count = (int)fread(movies, sizeof(MOVIE), (size_t)saved, fp);
    printf("Ucitano %d filmova.\n", *count);
    fclose(fp);
}



static int compareByTitle(const void* a, const void* b) {
    return strcmp(((const MOVIE*)a)->title, ((const MOVIE*)b)->title);
}

void sortMovies(MOVIE movies[], int count) {
    if (!movies || count < 2) return;
    qsort(movies, (size_t)count, sizeof(MOVIE), compareByTitle);
    printf("Sortirano po naslovu.\n");
}



static void swapMovies(MOVIE* a, MOVIE* b) {
    MOVIE tmp = *a; *a = *b; *b = tmp;
}

static int partition(MOVIE arr[], int low, int high) {
    MOVIE* pivot = &arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (strcmp(arr[j].title, pivot->title) <= 0)
            swapMovies(&arr[++i], &arr[j]);
    }
    swapMovies(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(MOVIE arr[], int low, int high) {
    if (!arr || low >= high) return;
    int pi = partition(arr, low, high);
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
}



int searchMovie(MOVIE movies[], int count, const char* title) {
    if (!movies || !title || count == 0) return -1;

    MOVIE key;
    memset(&key, 0, sizeof(key));
    strncpy(key.title, title, MAX_TITLE - 1);

    MOVIE* res = (MOVIE*)bsearch(&key, movies, (size_t)count, sizeof(MOVIE), compareByTitle);
    if (!res) return -1;
    return (int)(res - movies);
}
