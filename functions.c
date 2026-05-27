#define _CRT_SECURE_NO_WARNINGS
#include "functions.h"


const char* genreNames[] = { "Action", "Drama", "Comedy", "Horror", "SF" };


static int operationCount = 0;



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
	if (position < 0 || position >* count) position = *count;


	for (int i = *count; i > position; i--)
		movies[i] = movies[i - 1];

	memset(&movies[position], 0, sizeof(MOVIE));
	(*count)++;

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
	if (idx < 1 || idx >* count) { printf("Neispravan odabir.\n"); return; }
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


void safeFreeMOVIES(MOVIE** ptr) {
	if (!ptr || !*ptr) return; 
	memset(*ptr, 0, MAX_MOVIES * sizeof(MOVIE)); 
	free(*ptr); 
	*ptr = NULL; 
}


static int copyFile(const char* src, const char* dst) {
	if (!src || !dst) return -1;

	FILE* in = fopen(src, "rb");
	if (!in) { perror("copyFile fopen src"); return -1; }

	FILE* out = fopen(dst, "wb");
	if (!out) {
		perror("copyFile fopen dst");
		fclose(in);
		return -1;
	}

	char buf[512];
	size_t n;
	while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
		if (fwrite(buf, 1, n, out) != n) {
			fprintf(stderr, "copyFile: greska pri pisanju: %s\n", strerror(errno));
			fclose(in); fclose(out);
			return -1;
		}
	}

	
	if (ferror(in))
		fprintf(stderr, "copyFile: greska pri citanju: %s\n", strerror(errno));

	fclose(in);
	fclose(out);
	return 0;
}

void exportMoviesTXT(const MOVIE movies[], int count) {
	if (!movies) return;

	
	if (rename(FILENAME_TXT, FILENAME_TXT_BAK) == 0)
		printf("Backup kreiran: %s\n", FILENAME_TXT_BAK);

	FILE* fp = fopen(FILENAME_TXT, "w");
	if (!fp) { perror("exportMoviesTXT fopen"); return; }

	
	rewind(fp);

	for (int i = 0; i < count; i++) {
		fprintf(fp, "%s|%d|%.1f|%s|%d\n",
			movies[i].title,
			movies[i].releaseYear,
			movies[i].rating,
			movies[i].dateWatched,
			(int)movies[i].movieGenre);
	}

	
	if (ferror(fp))
		fprintf(stderr, "exportMoviesTXT: greska: %s\n", strerror(errno));

	long size = ftell(fp);
	printf("Eksportirano %d filmova u '%s' (%ld bajtova).\n", count, FILENAME_TXT, size);

	fclose(fp);
}


void importMoviesTXT(MOVIE movies[], int* count) {
	if (!movies || !count) return;

	FILE* fp = fopen(FILENAME_TXT, "r");
	if (!fp) { perror("importMoviesTXT fopen"); return; }

	
	rewind(fp);
	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	rewind(fp);

	if (fileSize == 0) {
		printf("Tekstualna datoteka je prazna.\n");
		fclose(fp);
		return;
	}

	int loaded = 0;
	char line[256];
	while (!feof(fp) && loaded < MAX_MOVIES) { 
		if (!fgets(line, sizeof(line), fp)) break;
		line[strcspn(line, "\n")] = '\0';

		MOVIE m;
		memset(&m, 0, sizeof(m));
		int genre = 0;
		if (sscanf(line, "%99[^|]|%d|%f|%19[^|]|%d",
			m.title, &m.releaseYear, &m.rating,
			m.dateWatched, &genre) == 5) {
			m.movieGenre = (genre >= 0 && genre < 5) ? (Genre)genre : ACTION;
			movies[loaded++] = m;
		}
	}


	if (ferror(fp))
		fprintf(stderr, "importMoviesTXT: greska citanja: %s\n", strerror(errno));

	*count = loaded;
	printf("Uvezeno %d filmova iz '%s'.\n", loaded, FILENAME_TXT);
	fclose(fp);
}
void deleteExportFile(void) {
	if (remove(FILENAME_TXT) == 0)
		printf("Datoteka '%s' obrisana.\n", FILENAME_TXT);
	else
		perror("remove");
}
void backupMovies(void) {
	if (copyFile(FILENAME, FILENAME_BAK) == 0)
		printf("Backup kreiran: %s\n", FILENAME_BAK);
	else
		fprintf(stderr, "Backup nije uspio: %s\n", strerror(errno));
}
static int compareByRating(const void* a, const void* b) {
	float ra = ((const MOVIE*)a)->rating;
	float rb = ((const MOVIE*)b)->rating;
	if (ra < rb) return 1; /* padajući redoslijed */
	if (ra > rb) return -1;
	return 0;
}


typedef int (*CmpFn)(const void*, const void*);


void sortMoviesBy(MOVIE movies[], int count, CmpFn cmp) {
	if (!movies || count < 2 || !cmp) return;
	qsort(movies, (size_t)count, sizeof(MOVIE), cmp);
}

void sortMoviesByRating(MOVIE movies[], int count) {
	sortMoviesBy(movies, count, compareByRating); 
	printf("Sortirano po ocjeni (padajuce).\n");
}

static int compareByYear(const void* a, const void* b) {
	return ((const MOVIE*)a)->releaseYear - ((const MOVIE*)b)->releaseYear;
}

static int recursiveBinarySearchYear(const MOVIE movies[], int low, int high, int year) {
	if (low > high) return -1; /* baza rekurzije */
	int mid = low + (high - low) / 2;
	if (movies[mid].releaseYear == year) return mid;
	if (movies[mid].releaseYear < year)
		return recursiveBinarySearchYear(movies, mid + 1, high, year);
	return recursiveBinarySearchYear(movies, low, mid - 1, year);
}

int searchMovieByYear(MOVIE movies[], int count, int year) {
	if (!movies || count == 0) return -1;
	sortMoviesBy(movies, count, compareByYear); 
	return recursiveBinarySearchYear(movies, 0, count - 1, year);
}
