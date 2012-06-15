#include <assert.h>
#include <stdlib.h>

#include "tuple.h"

struct tuple_list *
tl_cons (struct tuple _val, struct tuple_list * queue) {
    struct tuple_list * head = NULL;

    /* Having a copy could be great */

    if (!(head = malloc(sizeof *head))) {
	perror("tl_cons can't allocate memory !");
	return NULL;
    }

    head->val.fst = _val.fst;
    head->val.snd = _val.snd;
    head->next = queue;

    return head;
}

struct tuple_list *
tl_pop (struct tuple_list * list, struct tuple * popped) {
    assert(list != NULL);
    struct tuple_list * head = list;
    struct tuple_list * tail = list->next;

    if (popped) {
	popped->fst = head->val.fst;
	popped->snd = head->val.snd;
    }

    free(head);

    return tail;
}

void
tl_free (struct tuple_list * list) {
    struct tuple_list * elem;

    while (list) {
	elem = list;
	list = list->next;
	free(elem);
    }

    free(list);
i}

#ifdef DEBUG_MODE

int
main (void) {
    /* Test */
}

#endi

