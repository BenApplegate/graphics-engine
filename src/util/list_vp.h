/// Author: Benjamin Applegate
/// Implements a resizable list of void pointers

#pragma once
#include <stddef.h>

struct list_vp {
    void** data; //The data stored in the list
    size_t size; //The number of elements currently stored in the
    size_t capacity; //The currently allocated size of the list
};

/// Create a new list and allocate space to inial capacity
/// @param initial_capacity Any value less than zero will be set to 1
/// @return The newly created list
struct list_vp list_vp_new(size_t initial_capacity);

/// Delete allocated space for list, list should not be used after this is called
/// @param list The list to delete
void list_vp_delete(const struct list_vp* list);

/// Resets all data in allocated list to nullptr, and resets size
/// @warning Does not free data contained within list, make sure any heap allocated data within the list is freed before calling to avoid memory leaks
/// @param list The list to clear
void list_vp_clear(struct list_vp* list);

/// Resizes the list to the new capacity, copies old data into new space
/// If new capacity is smaller than old capacity, data will be copied from the old buffer into the new one upto the new capacity
/// @param list the list to resize
/// @param new_capacity the new capacity for the list
void list_vp_resize(struct list_vp* list, size_t new_capacity);

/// Adds an item to the end of the list, resizing it if needed
/// @param list the list to insert the item into
/// @param item the item to insert into the list
void list_vp_add(struct list_vp* list, void* item);

/// Returns the item at the specified index in the list
/// @param list the list to access data from
/// @param index the index to retrieve data from
/// @returns The item at the specified index in the list, or NULLPTR if index is out of list bounds
void* list_vp_at(const struct list_vp* list, size_t index);