#ifndef H_HASH
#define H_HASH

/* Liste des fonctions de hashage. */

unsigned int bernstein_hash (const char *);
unsigned int larson_hash (const char *);
unsigned int kr_hash (const char *);
unsigned int x65599_hash (const char *);
unsigned int fnv1a_hash (const char *);
unsigned int weinberger_hash (const char *);
unsigned int ramakrishna_hash (const char *);
unsigned int sdbm_hash (const char *);

#endif /* H_HASH */

