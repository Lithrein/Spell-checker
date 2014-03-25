#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "bloom_filter.h"

#define set_bit(a, n) (a[n / CHAR_BIT] |= (1 << ((n) % CHAR_BIT)))
#define get_bit(a, n) (a[n / CHAR_BIT] &  (1 << ((n) % CHAR_BIT)))

struct bloom_filter *
bloomf_create (size_t size, size_t nb_funcs) {
    struct bloom_filter * bloom = NULL;
    int i;

    bloom = malloc(sizeof *bloom);
    if (bloom) {
        bloom->filter = calloc((size + CHAR_BIT - 1) / CHAR_BIT, sizeof(char));
        if (!bloom->filter)
            goto bloomf_create_err1;
        else {
            bloom->hashfuncs = malloc(nb_funcs * sizeof(*(bloom->hashfuncs)));
            if (!bloom->hashfuncs)
                goto bloomf_create_err2;
        }
    }
   
    bloom->nfuncs = 0;
    bloom->nfuncs_max = nb_funcs;
    bloom->size = size;

    return bloom;
 
    bloomf_create_err2:
        free(bloom->filter), bloom->filter = NULL;
    bloomf_create_err1:
        free(bloom), bloom = NULL;

    return NULL;
}

struct bloom_filter *
bloomf_add_hashfunc (hashfunc_t f, struct bloom_filter * bloom) {
    if (bloom->nfuncs < bloom->nfuncs_max) {
        bloom->hashfuncs[bloom->nfuncs] = f;
        bloom->nfuncs++;
    }
    
    return bloom;
}

struct bloom_filter *
bloomf_add (const char * word, struct bloom_filter * bloom) {
   int i;

    for (i = 0 ; i < bloom->nfuncs ; ++i)
        set_bit(bloom->filter, bloom->hashfuncs[i](word) % bloom->size);

    return bloom; 
}

struct bloom_filter *
bloomf_add_dictionary (FILE * dict, struct bloom_filter * bloom) {
    char str[101] = {0};

    while (fgets(str, 100, dict)) {
        int i;
        for (i = 0 ; i < 100 ; ++i)
            if (str[i] == '\n') str[i] = '\0';
        bloomf_add(str, bloom);
    }

    rewind(dict);

    return bloom;
}

void
bloomf_destroy (struct bloom_filter * bloom) {
    free(bloom->hashfuncs), bloom->hashfuncs = NULL;
    free(bloom->filter), bloom->filter = NULL;
    free(bloom), bloom = NULL;
}

int
bloomf_check (const char * word, struct bloom_filter * bloom) {
    int i;

    for (i = 0 ; i < bloom->nfuncs ; ++i)
        if (!(get_bit(bloom->filter, bloom->hashfuncs[i](word) % bloom->size)))
            return 0;

    return 1;
}

