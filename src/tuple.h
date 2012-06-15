#ifndef H_TUPLE
#define H_TUPLE

/* Data strutures */

struct tuple {
    void ** fst;
    void ** snd;
};

struct tuple_list {
    struct tuple val;
    struct tuple_list * next;
};

#define TL_FOREACH (tp_list) do {						\
while (tp_list) {								\
    void * _fst = tp_list->val.fst						\
	 , _snd = tp_list->val.snd;						\

    
#define TL_FOREACH_END tp_list = tp_list->next; }} (0 /* constcond */		\


struct tuple_list * tl_cons (struct tuple, struct tuple_list *);
struct tuple_list * tl_pop  (struct tuple_list *, struct tuple *);
void tl_free (struct tuple_list *);

#endif	/* H_TUPLE */
