#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/data/bloom_filter.h"
#include "env.h"
#include "imode.h"

#define min(a,b) (((a) < (b)) ? (a) : (b))

static void
prompt (void) {
    printf(PROMPT);
}

static int
read_line (char * buf) {
    return scanf(" %99[^\n]", buf);
}

static int
count (char * str, char c) {
    int cnt = 0;
    
    while (*str++)
        if (*str == c) ++cnt;

    return cnt;
}

static char **
split (char * str, char c) {
    char ** words = NULL;
    int deb = 0, fin = 0, i = 0;
    int len = strlen(str);
    int sz = 1 + count(str, c);

    if(!(words = calloc(sz, sizeof *words))) {
        goto split_err1;
    }

    for (i = deb = fin = 0 ; i < sz ; ++i) {
        for (fin = fin + 1 ; fin < len && str[fin] != c ; ++fin);
        if (!(words[i] = calloc(fin - deb + 1, sizeof *words[i]))) {
            goto split_erri;
        }
        memcpy(words[i], str + deb, fin - deb);
        deb = fin + 1;
    }

    return words;

    split_erri:
        while (i--) free(words[i]), words[i] = NULL;
        free(words), words = NULL;
        
    split_err1:
        return NULL;
}

static int
parse (char * str, struct env * env) {
    char ** words = split(str, ' ');
    int sz = 1 + count(str, ' ');
    int i, quit = 0;

    if (str[0] == ':') { /* Il s'agit d'une commande */
        int cmd_len = strlen(words[0]);
        if (!strcmp(words[0], ":load")) {
            if (sz < 2) {
            } else {
                env_load_file( words[1], env);
            }
        } else if (!strcmp(words[0], ":quit")) {
            quit = 1;
        } else if (!strcmp(words[0], ":help")) {
            printf("Liste des commandes :\n");
            printf(":load dico -> Charge le fichier dictionnaire `dico'.\n");
            printf(":quit -> Quitte le mode inte©ractif.\n");
            printf(":help -> Affiche ce message d'aide.");
            printf("Note : Les noms de fichiers ne doivent pas contenir d'espaces.\n");
        } else {
            printf("%s : Commande inconnue.\n", words[0]);
        }
    } else { /* Il s'agit d'une simple recherche */
       env_look_up(words[0], env); 
    }
    
    for (i = 0 ; i < sz ; ++i)
        free(words[i]), words[i] = NULL;
    free(words);

    return quit;
}

void
interactive_mode(struct env * env) {
    int quit = 0;
    char buf[100] = {0};

    while (!quit) {
        prompt();
        if (!read_line(buf)) return;

        quit = parse(buf, env);
    }
}

