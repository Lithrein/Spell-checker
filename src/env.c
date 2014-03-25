#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils/data/bk_tree.h"
#include "utils/data/bloom_filter.h"
#include "utils/data/list.h"
#include "env.h"
#include "hash.h"

struct env *
env_create () {
    struct env * env = NULL;
    int i = 0;

    hashfunc_t hash[NB_HASH] = {
        bernstein_hash,
        larson_hash,
        kr_hash,
        x65599_hash,
        fnv1a_hash,
        weinberger_hash,
        ramakrishna_hash,
        sdbm_hash
    };

    if (!(env = malloc(sizeof *env)))
        goto env_create_err1;

    env->filter = bloomf_create(BLOOM_SIZE, NB_HASH);
    for (i = 0 ; i < NB_HASH ; ++i)
        bloomf_add_hashfunc(hash[i], env->filter);

    env->bk_tree = bk_tree_create();
    env->benchmark = 0;
    env->time_bk_create = 0;
    env->time_bf_create = 0;

    return env;    

    env_create_err1:
        return NULL;
}

struct env *
env_load_file (char * path, struct env * env) {
    FILE * dico = fopen(path, "r");
    clock_t begin, end;
   
    if (!(env->benchmark))
        printf("Chargement du fichier : %s. \n", path);
    if (!dico) {
        perror("load");
        return env;
    }
    
    if (!(env->benchmark))
        printf("Ajout au filtre de bloom : ");
    begin = clock();
    bloomf_add_dictionary(dico, env->filter);
    end = clock();
    env->time_bf_create = (int) ((double) (end - begin) / CLOCKS_PER_SEC);

    if (!(env->benchmark))
        printf("ok\n");

    if (!(env->benchmark))
        printf("Creation du Bk-Tree associe: ");

    begin = clock();
    env->bk_tree = bk_tree_add_dictionary(dico, env->bk_tree);
    end = clock();
    env->time_bk_create = (int) ((double) (end - begin) / CLOCKS_PER_SEC);

    if (!(env->benchmark))
        printf("ok\n");

    fclose(dico);

    return env;
}

void
env_look_up (const char * word, struct env * env) {
        if (bloomf_check(word, env->filter)) {
            if (!(env->benchmark)) 
                puts("Ce mot est dans le dictionnaire.");
        } else {
            LIST_NAME(string) res = bk_tree_search(word, BK_DIST, env->bk_tree);
            if (!(env->benchmark)) {
                puts("Ce mot n'est pas dans le dictionnaire.");
                puts("Suggestions de correction :");
        
                putchar('[');
                LIST_FOREACH(string, res)
                    printf("%s, ", LIST_HEAD(_list));
                LIST_FOREACH_END;
                puts("\b\b]");
            }
            LIST_FREE(string, res);
     }
}

void
env_destroy (struct env * env) {
    bloomf_destroy(env->filter);
    free(env);
}

