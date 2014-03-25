#ifndef H_BK_TREE
#define H_BK_TREE

#include "list.h"

#define NB_SONS  30
#define BK_DIST   2

LIST_DEFINITION(char *, string);

struct bk_arc {
    int dist;
    struct bk_tree * son;
};

struct bk_tree {
    char * word;
    struct bk_arc sons[NB_SONS];
};

struct bk_tree * bk_tree_create (void);
struct bk_tree * bk_tree_add (const char *, struct bk_tree *);
struct bk_tree * bk_tree_add_dictionary (FILE *, struct bk_tree *);
void bk_tree_destroy (struct bk_tree *);

LIST_NAME(string) bk_tree_search (const char *, int, struct bk_tree *);

#endif /* H_BK_TREE */

