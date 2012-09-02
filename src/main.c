#include <getopt.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "bloom_filter.h"
#include "dictionary.h"
#include "tuple.h"

#define VERSION_NO          ???
#define VERSION_STR(s)      L ## #s
#define VERSION(VER_NO)     VERSION_STR(VER_NO)

/* Flags des options */
#define F_INTERACTIVE       0x01
#define F_WORD              0x02
#define F_DEFINITION        0x04
#define F_ACRONYM           0x08
#define F_VERSION           0x10
#define F_HELP              0x20

struct flag {
    int flags;
    char * word;
    char * acronyms;
    char * definitions;
};

void
usage (void) {
    wchar_t * text = L"usage : wtf [-i | -a | -s | -h | -v]                   \n\
-i --interactive : Mode interactif.                                           \n\
    La commande `:l' permet de charger une paire de fichiers.                 \n\
    La commande `:r' permet de supprimer une paires de fichiers.              \n\
    La commande `:s' permet de chercher la signification d'un acronyme.       \n\
                                                                              \n\
-w --word       : acronyme à chercher                                         \n\
                                                                              \n\
-d --definition : fichier de définitions.                                     \n\
                                                                              \n\
-a --acronym    : fichier d'acronymes                                         \n\
                                                                              \n\
-v --version    : Affiche le numéro de version.                               \n\
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
    const char * optstring = "iw:d:a:vh";
    struct flag * result = NULL;
    struct option long_options [] = {
        /*   name     /    arguments     / flag / val */
        {"interactive", no_argument      , NULL, 'i'},
        {"word"       , required_argument, NULL, 'w'},
        {"definition" , required_argument, NULL, 'd'},
        {"acronym"    , required_argument, NULL, 'a'},
        {"version"    , no_argument      , NULL, 'v'},
        {"help"       , no_argument      , NULL, 'h'},
        { NULL        , 0                , NULL,  0 }
    };

    result = malloc(sizeof *result);
    if (result == NULL) goto err;
    
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
                result->flags |= F_DEFINITION;
                result->definitions = optarg;
                break;
            case 'a':
                result->flags |= F_ACRONYM;
                result->acronyms = optarg;
                break;
            case 'v':
                result->flags |= F_VERSION;
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
    struct tuple_list * dicos = NULL;

    setlocale(LC_ALL, "");

    if (argc < 2) {
        usage();
    } else {
        options = parse_args(argc, argv);
        if (options == NULL)
            return EXIT_FAILURE;
        
        /* Traitement des options */
        if (options->flags & F_HELP) {
            usage();
            return EXIT_SUCCESS;
        } else if (options->flags & F_VERSION) {
            version();
            return EXIT_SUCCESS;
        }

        if ((options->flags & F_DEFINITION) || (options->flags & F_ACRONYM)) {
            if ((options->flags & F_DEFINITION) && (options->flags & F_ACRONYM)) {
                dicos = load_dictionary(options->acronyms, options->definitions, dicos);
            } else if (!(options->flags & F_INTERACTIVE)) {
                perror("acronym's and definition's files are both needed "
                       "unless you set up interactive mode.");
                return EXIT_SUCCESS;
            }
        }

        if (options->flags & F_WORD) {
            /* search dico and print answer */
        } else if (options->flags & F_INTERACTIVE) {
            interactive_mode(dicos);
        } else {
            perror("Nothing to be done!");
        }
    
        free(options);
    }

    return EXIT_SUCCESS;
}
