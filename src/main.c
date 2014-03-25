#include <getopt.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

#include "utils/data/bloom_filter.h"
#include "env.h"
#include "imode.h"

#define VERSION_NO          ???
#define VERSION_STR(s)      L ## #s
#define VERSION(VER_NO)     VERSION_STR(VER_NO)

/* Flags des options */
#define F_INTERACTIVE       0x01
#define F_WORD              0x02
#define F_DICO              0x04
#define F_VERSION           0x08
#define F_BENCHMARK         0x10
#define F_HELP              0x20

struct flag {
    int flags;
    char * word;
    char * dict;
};

void
usage (void) {
    wchar_t * text = L"usage : wtf [-i | -s | -h | -v]                        \n\
-i --interactive : Mode interactif.                                           \n\
    La commande `:load file' permet de charger un dictionnaire.               \n\
    La commande `:help' affiche les commandes du mode interactif.             \n\
    La commande `:quit' permet de quitter le mode interactif.                 \n\
                                                                              \n\
-w --word       : mot à chercher.                                           \n\
                                                                              \n\
-d --dict       : fichier de définitions.                                     \n\
                                                                              \n\
-v --version    : Affiche le numéro de version.                               \n\
                                                                              \n\
-b --benchmark  : Active le mode `benchmark' :                                \n\
                    * seules les mesures sont affichees,                      \n\
                    * les options `-d' et `-w' sont obligatoires,             \n\
                    * l'argument de `-w' doit etre un fichier de mots.        \n\
                                                                              \n\
-h --help       : Affiche ce message d'aide.                                  \n\
                                                                              \n\
Note : Le fichier d'acronymes et de définitions doivent être en correspondance u\
n pour un.";

    wprintf(L"%ls\n", text);
}

void
version (void) {
    wchar_t * fmt = L"wtf. version %ls.\n";
    wchar_t * ver = VERSION(VERSION_NO);

    wprintf(fmt, ver);
}

struct flag *
parse_args(int argc, char * argv[]) {
    int c = -1;
    int option_index = 0;
    const char * optstring = "iw:d:vbh";
    struct flag * result = NULL;
    struct option long_options [] = {
        /*   name     /    arguments     / flag / val */
        {"interactive", no_argument      , NULL, 'i'},
        {"word"       , required_argument, NULL, 'w'},
        {"dict"       , required_argument, NULL, 'd'},
        {"version"    , no_argument      , NULL, 'v'},
        {"benchmark"  , no_argument      , NULL, 'b'},
        {"help"       , no_argument      , NULL, 'h'},
        { NULL        , 0                , NULL,  0 }
    };

    result = malloc(sizeof *result);
    if (result == NULL) goto err;
    result->flags = 0;
    result->word = result->dict = NULL;   
 
    opterr = 0; /* Pas de rapport d'erreurs. */ 
    optind = 1;

    while ((c = getopt_long(argc, argv, optstring,
            long_options, &option_index)) != EOF) {
        switch (c) {
            case 'i':
                result->flags |= F_INTERACTIVE;
                break;
            case 'w':
                result->flags |= F_WORD;
                result->word = optarg;
                break;
            case 'd':
                result->flags |= F_DICO;
                result->dict = optarg;
                break;
            case 'v':
                result->flags |= F_VERSION;
                break;
            case 'b':
                result->flags |= F_BENCHMARK;
                break;
            case 'h':
            default :
                result->flags |= F_HELP;
        }
    }

    return result;
    
    err:
        return NULL;
}

int
main (int argc, char * argv[]) {
    struct flag * options = NULL;
    struct env * env = env_create();

    setlocale(LC_ALL, "");

    if (argc < 2) {
        usage();
        goto end;
    }

    options = parse_args(argc, argv);
    if (options == NULL)
        goto end;
    
    /* Traitement des options */
    if (options->flags & F_HELP) {
        usage();
        goto end;
    } else if (options->flags & F_VERSION) {
        version();
        goto end;
    }

    if (options->flags & F_BENCHMARK)
        env->benchmark = 1;    

    if (options->flags & F_DICO) {
        env_load_file(options->dict, env);
    } else if (!(options->flags & F_INTERACTIVE)) {
        printf("dictionary's file is needed unless interactive mode is on.");
        goto end;
    }

    if (options->flags & F_WORD) {
        if (!(options->flags & F_BENCHMARK)) {
            env_look_up(options->word, env);
        } else {
            FILE * words = fopen(options->word, "r");
            char str[101] = {0};
            clock_t begin, end;

            begin = clock();
            while (fgets(str, 100, words)) {
                int i;
                for (i = 0 ; i < 100 ; ++i)
                if (str[i] == '\n') str[i] = '\0';
                env_look_up(str, env);
            }
            end = clock();
            rewind(words);

            fclose(words);
            puts("Temps de creation :");
            printf("\t * filtre de bloom : %d\n", env->time_bf_create);
            printf("\t * bk_tree : %d\n", env->time_bk_create);
            
            printf("Temps de recherche total : %d\n", (int) ((end - begin) / CLOCKS_PER_SEC));
        }
    } else if (options->flags & F_INTERACTIVE) {
        interactive_mode(env);
    } else {
        puts("Nothing to be done!");
    }

    end:
        env_destroy(env);
        free(options);

    return EXIT_SUCCESS;
}
