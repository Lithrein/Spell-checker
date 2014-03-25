#ifndef H_LIST
#define H_LIST

#define LIST_DEFINITION(type, name)                                            \
    struct name##_list {                                                       \
        type elem;                                                             \
        struct name##_list * next;                                             \
}                                                                                

#define LIST_NAME(name) struct name##_list *                                   \

#define LIST_HEAD(list)                                                        \
    (list)->elem

#define LIST_TAIL(list)                                                        \
    (list)->next

#define LIST_CONS(type, name, value, list) do {                                \
    struct name##_list * new_head = NULL;                                      \
    if((new_head = calloc(1, sizeof * new_head))) {                            \
        new_head->elem = (type) (value);                                       \
        new_head->next = (list);                                               \
        list = new_head;                                                       \
    }                                                                          \
} while (0 /* constcond */)                                                   

#define LIST_FREE(name, list) do {                                             \
    struct name##_list * cur = NULL;                                           \
    while (list) {                                                             \
        cur = (list);                                                          \
        list = (list)->next;                                                   \
        free(cur);                                                             \
    }                                                                          \
} while (0 /* constcond */)

#define LIST_CONCAT(type, name, list, tail) do {                               \
    struct name##_list * _list = (list);                                       \
    for (;_list->next != NULL; _list = _list->next);                           \
    _list->next = tail;                                                        \
} while (0 /* constcond */)

#define LIST_MAP(name, f, list) do {                                           \
    struct name##_list * _list = (list);                                       \
    while (_list) {                                                            \
        _list->elem = f(_list->elem);                                          \
        _list = _list->next;                                                   \
    }                                                                          \
} while (0 /* constcond */)

#define LIST_FOREACH(name, list) do {                                          \
    struct name##_list * _list = (list);                                       \
    for (;_list; _list = _list->next) {                                        \

#define LIST_FOREACH_END                                                       \
}} while (0 /* constcond */)

#endif /* H_LIST */

