#ifndef _LIST_H
#define _LIST_H

#include <stddef.h>

#define DEFINE_LIST(type)               \
    struct type##_list {                \
        struct type *head;              \
        struct type *tail;              \
        uint32_t size;                  \
    };

#define DEFINE_LINK(type)               \
    struct type *next_##type;           \
    struct type *prev_##type;

#define INIT_LIST(list)                 \
    list.head = list.tail = (void *) 0; \
    list.size = 0;

#define IMPLEMENT_LIST(type)                                                    \
    void append_##type##_list(struct type##_list *list, struct type *data) {    \
        list->tail->next_##type = data;                                         \
        data->prev_##type = list->tail;                                         \
        list->tail = data;                                                      \
        data->next_##type = NULL;                                               \
        list->size++;                                                           \
    }                                                                           \
    void prepend_##type##_list(struct type##_list *list, struct type *data) {   \
        data->next_##type = list->head;                                         \
        data->prev_##type = NULL;                                               \
        list->head = data;                                                      \
        list->size++;                                                           \
    }                                                                           \
    struct type *peek_##type##_list(struct type##_list *list) {                 \
        return list->head;                                                      \
    }                                                                           \
    struct type *dequeue_##type##_list(struct type##_list *list) {              \
        struct type *first = list->head;                                        \
        list->head = first->next_##type;                                        \
        list->head->prev_##type = NULL;                                         \
        list->size--;                                                           \
        return first;                                                           \
    }                                                                           \
    uint32_t size_##type##_list(struct type##_list *list) {                     \
        return list->size;                                                      \
    }                                                                           \
    struct type *next_##type##_list(struct type *data) {                        \
        return data->next_##type;                                               \
    }                                                                           \


#endif
