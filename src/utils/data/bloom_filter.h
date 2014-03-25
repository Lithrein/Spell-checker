#ifndef H_BLOOM_FILTER
#define H_BLOOM_FILTER

#include <stddef.h> /* size_t */

typedef unsigned int (*hashfunc_t)(const char *);

struct bloom_filter {
    size_t size;
    unsigned char * filter;
    size_t nfuncs, nfuncs_max;
    hashfunc_t * hashfuncs;
};

struct bloom_filter * bloomf_create (size_t, size_t);
struct bloom_filter * bloomf_add_hashfunc (hashfunc_t, struct bloom_filter *);
struct bloom_filter * bloomf_add (const char *, struct bloom_filter *);
struct bloom_filter * bloomf_add_dictionary (FILE *, struct bloom_filter *);
void bloomf_destroy (struct bloom_filter *);

int bloomf_check (const char *, struct bloom_filter *);

#endif

