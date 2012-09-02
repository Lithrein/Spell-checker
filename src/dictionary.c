#include <stdlib.h>
#include <stdio.h>

#include "tuple.h"
#include "dictionary.h"

/**

  Charge les fichiers sur lesquels pointent les variables de chemins /words/ et
/definitions/. Si le chargement d'un des deux fichiers venait à échouer. La
liste non modifié serait retournés. Autrement, les deux fichiers sont placés
dans un tuple qui est ajouté en début de liste.

nouvelle_liste = ( words, definitions ) :: ancienne_liste

*/
struct tuple_list *
load_dictionary (const char * words, const char * definitions, struct tuple_list * dicos) {
    FILE * words_file, * definitions_file;
    struct tuple new_dico;
    struct tuple_list * new_list;

    words_file = definitions_file = NULL;
    new_list = NULL;

    words_file = fopen(words, "r");
    if (words_file == NULL) {
        perror("load_dictionary");
        return dicos;
    }

    definitions_file = fopen(definitions, "r");
    if (definitions_file == NULL) {
        fclose(words_file);
        perror("load_dictionary");
        return dicos;
    }

    new_dico.fst = words_file;
    new_dico.snd = definitions_file;

    new_list = tl_cons(new_dico, dicos);

    return new_list ? new_list : dicos;
}


void
clear_dictionaries (struct tuple_list * dicos) {
    struct tuple_list * elem = NULL;

    while (dicos != NULL) {
        elem = dicos->next;

        fclose(dicos->val.fst), dicos->val.fst = NULL;
        fclose(dicos->val.snd), dicos->val.snd = NULL;
        free(dicos), dicos = NULL;

        dicos = elem;
    }
}

#ifdef DEBUG_MODE_DICTIONARY

int
main (void) {
    int i;
    char str[100] = {0};
    struct tuple_list * dicos = NULL;

    dicos = load_dictionary("wtf", "define", dicos);
    clear_dictionaries(dicos);

    return 0;
}

#endif 
