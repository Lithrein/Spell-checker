#ifndef H_TUPLE
#define H_TUPLE

/* Data strutures */

struct tuple {
    void * fst;
    void * snd;
};

struct tuple_list {
    struct tuple val;
    struct tuple_list * next;
};

#define TL_FOREACH(tp_list) do {                                               \
    struct tuple_list * _tmp = (tp_list);                                      \
    while (_tmp) {								                               \
        void * _fst = _tmp->val.fst						                       \
	    ,* _snd = _tmp->val.snd;						                       \

    
#define TL_FOREACH_END(tp_list)							                       \
    _tmp = _tmp->next; }} while (0 /* constcond */)			                   \


struct tuple_list * tl_cons (struct tuple, struct tuple_list *);
struct tuple_list * tl_pop  (struct tuple_list *, struct tuple *);
void tl_free (struct tuple_list *);

#endif	/* H_TUPLE */

