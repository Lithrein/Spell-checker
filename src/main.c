#include <getopt.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "bloom_filter.h"

#define VERSION_NO          ???
#define VERSION_STR(s)      L ## #s
#define VERSION(VER_NO)     VERSION_STR(VER_NO)

/* Flags des options */
#define INTERACTIVE         0x01
#define WORD                0x02
#define DEFINITION          0x04
#define ACRONYM             0x08
#define VERSION             0x10
#define HELP                0x20

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
    wchar_t * fmt = L"wtf. version %ls.";
    wchar_t * ver = VERSION(VERSION_NO);

    wprintf(fmt, ver);
}

int
parse_args(int argc, char * argvi[]) {
    int flags = 0;
    struct option long_optons [] = {
        /*   name     /    arguments     / flag / val */
        {"interactive", no_argument      , NULL, 'i'},
        {"word"       , required_argument, NULL, 'w'},
        {"definition" , required_argument, NULL, 'd'},
        {"acronym"    , required_argument, NULL, 'a'},
        {"version"    , no_argument      , NULL, 'v'},
        {"help"       , no_argument      , NULL, 'h'}
    }

    
}

int
main (int argc, char * argv[]) {
    setlocale(LC_ALL, "");
    
    

    return EXIT_SUCCESS;
}
