#include <assert.h>
#include <stdlib.h>

#include "tuple.h"

#ifdef DEBUG_MODE_TUPLE
#include <stdio.h>
#endif

struct tuple_list *
tl_cons (struct tuple _val, struct tuple_list * queue) {
    struct tuple_list * head = NULL;

    if (!(head = malloc(sizeof *head))) {
        perror("tl_cons");
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

}

#ifdef DEBUG_MODE_TUPLE

int
main (void) {
    int i;
    struct tuple t = { (void *)1, (void *)1};
    struct tuple_list * list = NULL;

    for (i = 0 ; i < 5 ; ++i) {
        void * tmp = t.snd;
        list = tl_cons(t, list);
        t.snd =  (int) t.fst + (int) t.snd;
        t.fst =  tmp;
    }

    TL_FOREACH(list)
	    printf("fst = %d\tsnd = %d\n", (int) _fst, (int) _snd);
    TL_FOREACH_END(list);

    tl_free(list);

    return 0;
}

#endif

