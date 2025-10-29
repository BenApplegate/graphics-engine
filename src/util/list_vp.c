/// Author: Benjamin Applegate

#include "list_vp.h"

#include <stdio.h>
#include <stdlib.h>

struct list_vp list_vp_new(size_t initial_capacity) {
    //Clamp initial capacity to a minimum of 1
    if (initial_capacity < 1) initial_capacity = 1;

    //Create new list struct and allocate buffer to initial capacity
    struct list_vp list = {nullptr, 0, initial_capacity};
    list.data = malloc(initial_capacity * sizeof(void*));

    //Print error message if memory allocation fails
    if (list.data == nullptr) {
        fprintf(stderr, "Memory allocation failed for list_vp_new\n");
    }

    return list;
}

void list_vp_delete(const struct list_vp* list) {
    free(list->data);
}

void list_vp_clear(struct list_vp* list) {
    //Set all data in the list back to nullptr
    for (int i = 0; i < list->capacity; i++) {
        list->data[i] = nullptr;
    }

    //Reset list size while maintaining capacity
    list->size = 0;
}

void list_vp_resize(struct list_vp* list, const size_t new_capacity) {
    //Reallocate buffer to new capacity and print error if realloc fails
    void* new_data = realloc(list->data, new_capacity * sizeof(void*));
    if (new_data == nullptr) {
        fprintf(stderr, "Memory allocation failed for list_vp_resize, retaining old size\n");
        return;
    }

    //Update struct to represent new list location and capacity
    list->data = new_data;
    list->capacity = new_capacity;
}

void list_vp_add(struct list_vp* list, void* item) {
    //Check if the list needs to resize
    if (list->size >= list->capacity) {
        //Double list capacity
        //The growth factor of 2 was chosen basically at random but should be fine for now
        //It may be worth considering different growth factors in the future if this list gets used for much larger sizes than it currently is
        //It is important to balance not allocating too much space that won't be used vs not allocating too frequently
        list_vp_resize(list, list->capacity * 2);
    }

    //Insert item into list and increment list size
    list->data[list->size++] = item;
}

void* list_vp_at(const struct list_vp* list, const size_t index) {
    //Check that the index is not out of bounds
    if (index >= list->size) return nullptr;

    //Fetch and return item
    return list->data[index];
}