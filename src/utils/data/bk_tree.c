#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bk_tree.h"
#include "../algorithms/levenshtein.h"

struct bk_tree *
bk_tree_create (void) {
    return NULL;
}

static struct bk_tree *
insert_son (const char * word, int dist, struct bk_tree * tree) {
    if (!tree->sons[dist].son) {
        tree->sons[dist].son = bk_tree_add(word, NULL);
    } else {
        tree->sons[dist].son = bk_tree_add(word, tree->sons[dist].son);
        tree->sons[dist].dist = dist;
    }

    return tree;
}

struct bk_tree *
bk_tree_add (const char * word, struct bk_tree * tree) {
    if (!tree) {
        int word_len = strlen(word);

        if (!(tree = calloc(1, sizeof *tree)))
            goto bk_tree_add_err1;
        if (!(tree->word = calloc(word_len + 1, sizeof *(tree->word))))
            goto bk_tree_add_err2;

        strcpy(tree->word, word);
        memset(tree->sons, 0, NB_SONS * sizeof *(tree->sons));
    } else {
        int dist;

        dist = levenshtein(word, tree->word);
        tree = insert_son(word, dist, tree);
    }
    
    return tree;
    
    bk_tree_add_err2:
        free(tree), tree = NULL;
    bk_tree_add_err1:
        return NULL;
}

struct bk_tree *
bk_tree_add_dictionary (FILE * dict, struct bk_tree * tree) {
    char str[101] = {0};

    while (fgets(str, 100, dict)) {
        int i;
        for (i = 0 ; i < 100 ; ++i)
            if (str[i] == '\n') str[i] = '\0';
        tree = bk_tree_add(str, tree);
    }
    rewind(dict);

    return tree;
}

void
bk_tree_destroy (struct bk_tree * tree) {
    int i;
    if (!tree) return;

    free(tree->word), tree->word = NULL;

    for (i = 0 ; i < NB_SONS ; ++i) {
        bk_tree_destroy(tree->sons[i].son);
    }

    free(tree), tree = NULL;
}

LIST_NAME(string)
bk_tree_search(const char * word, int range, struct bk_tree * tree) {
    LIST_NAME(string) res = NULL;
    int dist, i;

    if (!tree) return NULL;

    dist = levenshtein(word, tree->word);
    
    if (dist <= range) {
        LIST_CONS(char *, string, tree->word, res);
    }

    for (i = 0 ; i < NB_SONS ; ++i) {
        int son_dist = tree->sons[i].dist;
        if (dist - range <= son_dist && son_dist <= dist + range) {
            LIST_NAME(string) tmp = NULL;
            
            tmp = bk_tree_search(word, range, tree->sons[i].son);

            if (!res) res = tmp;
            else LIST_CONCAT(char *, string, res, tmp);
        }
    }

    return res;
}

