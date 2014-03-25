#ifndef H_ENV
#define H_ENV

#define BLOOM_SIZE 2500000
#define NB_HASH 8

/**
    Fichier donnant la definition de l'environnement du mode interactif.
    Ce fichier contient des fonctions permettant d'interagir avec
    l'environnement.
*/

struct env {
    struct bloom_filter * filter; /* Le filtre de bloom */
    struct bk_tree * bk_tree; /* Le BK-Tree */

    int benchmark;
    int time_bf_create;
    int time_bk_create;
};

/* Creer un evironnement vierge */
struct env * env_create ();

/* Ajoute un fichier a l'environnement d'execution */
struct env * env_load_file (char *, struct env *);

/* Cherche un mot dans le dictionnaire de env.
   Si le mot est absent, il propose des suggestions de correction. */
void env_look_up (const char * word, struct env * env);

void env_destroy (struct env * env);

#endif /* H_ENV */

