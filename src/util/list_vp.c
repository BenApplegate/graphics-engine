#include "list_vp.h"

#include <stdio.h>
#include <stdlib.h>

struct list_vp list_vp_new(size_t initial_capacity) {
    struct list_vp list = {nullptr, 0, initial_capacity};
    list.data = malloc(initial_capacity * sizeof(void*));
    if (list.data == nullptr) {
        fprintf(stderr, "Memory allocation failed for list_vp_new\n");
    }
    return list;
}

void list_vp_delete(const struct list_vp* list) {
    free(list->data);
}

void list_vp_clear(struct list_vp* list) {
    for (int i = 0; i < list->capacity; i++) {
        list->data[i] = nullptr;
    }
    list->size = 0;
}

void list_vp_resize(struct list_vp* list, const size_t new_capacity) {
    void* new_data = realloc(list->data, new_capacity * sizeof(void*));
    if (new_data == nullptr) {
        fprintf(stderr, "Memory allocation failed for list_vp_resize, retaining old size\n");
        return;
    }
    list->data = new_data;
    list->capacity = new_capacity;
}

void list_vp_add(struct list_vp* list, void* item) {
    if (list->size >= list->capacity) {
        //The list is not big enough, and we should grow the list
        //We will double list size
        list_vp_resize(list, list->capacity * 2);
    }
    list->data[list->size++] = item;
}

void* list_vp_at(const struct list_vp* list, const size_t index) {
    if (index >= list->size) return nullptr;
    return list->data[index];
}