/************************
*  Projekt c. 3         *
*  Autor: Jan Vavra     *
*  Login: xvavra20      *
************************/

/**
 * Kostra programu pro 3. projekt IZP 2016/17
 *
 * Jednoducha shlukova analyza
 * Complete linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/clsrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>

#define LOAD_PROBLEM \
{  \
	fprintf(stderr, "Chyba pri nacitani ze souboru. Ukoncuji\n"); \
	return 1; \
}
#define ARG_PROBLEM(narr) \
{ \
	for (int j = 0; j < narr; j++) \
		clear_cluster(&clusters[j]); \
	free(clusters); \
	fprintf(stderr, "Chyba ve vstupnich argumentech. Ukoncuji\n"); \
	return 1; \
}

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
	assert(c != NULL);
	assert(cap >= 0);

	c->size = 0;
	c->capacity = cap;
	if (cap > 0)
		c->obj = malloc(cap * sizeof(struct obj_t));
	else
		c->obj = NULL;

	return;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
	free(c->obj);
	c->obj = NULL;
	c->size = 0;
	c->capacity = 0;

	return;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
	assert(c);

	if (c->size == c->capacity)
		if (resize_cluster(c, c->capacity + CLUSTER_CHUNK) == NULL)
			return;

	c->obj[c->size] = obj;
	c->size++;

	return;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

	for (int i = 0; i < c2->size; i++)
	{
		append_cluster(c1, c2->obj[i]);
	}
	sort_cluster(c1);

	return;
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
	assert(idx < narr);
	assert(narr > 0);

	do
	{
		clear_cluster(&carr[idx]);
		init_cluster(&carr[idx], carr[idx + 1].capacity);
		merge_clusters(&carr[idx], &carr[idx + 1]);
		idx++;

	}while (idx < narr - 1);
	clear_cluster(&carr[idx]);

	return idx;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

	float x = o2->x - o1->x;
	float y = o2->y - o1->y;
	return sqrtf(x*x + y*y);
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

	float furthest = 0.0;

	for (int i = 0; i < c1->size; i++)
	{
		float temp;
		for (int j = 0; j < c2->size; j++)
		{
			temp = obj_distance(&(c1->obj[i]), &(c2->obj[j]));
			if (temp > furthest)
				furthest = temp;
		}
	}

	return furthest;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

	float nearest = 0;
	narr -= 1;

	for (int i = 0; i < narr; i++)
	{
		float temp;
		for (int j = narr; j > i; j--)
		{
			//ulozeni cisla pro porovnani
			temp = cluster_distance(&carr[i], &carr[j]);
			if (temp < nearest || (i == 0 && j == narr))
			{
				nearest = temp;
				*c1 = i;
				*c2 = j;
			}
		}
	}
	return;
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
	assert(arr != NULL);

	char line[20];
	int clust;
	char *leftover;
	int err = -1;
	FILE *file;

	file = fopen(filename, "r");
	if (file == NULL)
		return (-1);

	if (fgets(line, 20, file) != NULL)
	{
		//zjisteni jestli se na 1. radku nachazi "count" a ulozi N do clust
		leftover = strtok(line, " =");
		if (strcmp(leftover, "count") == 0)
		{
			leftover = strtok(NULL, "\n");
			clust = strtol(leftover, NULL, 10);
		}
		else
			return (-1);
	}

	//alokovani pameti a nacteni hodnot ze souboru
	*arr = malloc(clust * sizeof(struct cluster_t));
	for (int i = 0; i < clust; i++)
	{
		struct obj_t obj;
		init_cluster(&((*arr)[i]), CLUSTER_CHUNK);
		if(fscanf(file, "%d %f %f", &obj.id, &obj.x, &obj.y) == 3 && 0 <= obj.x && 0 <= obj.y && 1000 >= obj.x && 1000 >= obj.y)
		{
			obj.x = floorf(obj.x);
			obj.y = floorf(obj.y);
			append_cluster(&((*arr)[i]), obj);
		}
		else
			err = i;

		//overeni duplicity id
		for (int j = i - 1; j >= 0; j--)
		{
			if (obj.id == (*arr)[j].obj[0].id)
			{
				err = i;
				break;
			}
		}

		if (err != (-1))
			break;
	}
	if (fclose(file) == EOF)
		return (-1);

	//pokud radek nesplnil podminky pro vstup, uvolnime pamet a nastavime *arr na NULL
	if (err >= 0)
	{
		for (int j = 0; j <= err; j++)
			clear_cluster(&((*arr)[j]));
		free(*arr);
		*arr = NULL;
	}
	return clust;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
	struct cluster_t *clusters;
	int num_of_clust = 0;
	char *leftover = "\0";

	if (argc == 3 || argc == 2)
	{
		//cteni ze souboru
		int loaded = load_clusters(argv[1], &clusters);
		if (loaded == (-1) || clusters == NULL)
			LOAD_PROBLEM;

		//precteni ciselneho argumentu ze vstupu
		if (argv[2] == NULL)
			num_of_clust = 1;
		else
			num_of_clust = strtol(argv[2], &leftover, 10);

		if (num_of_clust <= 0 || loaded < num_of_clust || *leftover != 0)
			ARG_PROBLEM(loaded);

		//sdruzovani shluku
		for (; loaded > num_of_clust; loaded--)
		{
			int c1;
			int c2;
			find_neighbours(clusters, loaded, &c1, &c2);

			merge_clusters(&clusters[c1], &clusters[c2]);
			remove_cluster(clusters, loaded, c2);
		}

		//tisk shluku
		print_clusters(clusters, num_of_clust);

		//uvolneni pameti pro objekty/clustery
		for (int j = 0; j < loaded; j++)
			clear_cluster(&clusters[j]);
		free(clusters);
	}
	else
	{
		fprintf(stderr, "Nespravny pocet argumentu. Ukoncuji.\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

