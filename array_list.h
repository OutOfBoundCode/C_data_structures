#pragma once

#include <sys/types.h>

/**
 * @brief Array list structure.
 */
typedef struct {
    ssize_t length;      /**< Number of elements currently in the list */
    ssize_t max_size;    /**< Maximum capacity of the array */
    void** arr;          /**< Pointer to the array of element pointers */
} array_list;

/**
 * @brief Create a new array list with a specified initial size.
 * @param array_size Initial capacity of the array list.
 * @note if the initial size is <=0 the size of the array will be defaulted to 10
 * @note the size of the array doubles when full
 * @return Pointer to the newly created array list, or NULL on failure.
 */
array_list* create_array_list(ssize_t array_size);

/**
 * @brief Free the array list and its elements.
 * @param list Pointer to the array list.
 * @param free_element Function pointer to free the elements (can be NULL).
 * @note If the list owns the memory of elements, pass a valid free_element function; otherwise, pass NULL to avoid freeing memory not owned by the list.
 */
void free_array_list(array_list *list, void (*free_element) (void*));

/**
 * @brief Get the index of an element in the array list.
 * @param list Pointer to the array list.
 * @param element Pointer to the element to find.
 * @param compare Function pointer to compare two elements.
 * @note The compare function should return 0 if the elements match, -1 otherwise.
 * @return Index of the element, or -1 if not found.
 */
ssize_t alget_index(const array_list *list, void* element, int (*compare) (void*, void*));

/**
 * @brief Print all elements in the array list.
 * @param list Pointer to the array list.
 * @param print_element Function pointer to print each element.
 */
void alprint(const array_list *list, void (*print_element) (void*));

/**
 * @brief Reverse the array list in place.
 * @param list Pointer to the array list.
 */
void alreverse(array_list *list);

/**
 * @brief Get the element at a specific index.
 * @param list Pointer to the array list.
 * @param index Index of the element to retrieve.
 * @return Pointer to the element, or NULL if index is out of range.
 */
void *alget(const array_list *list, ssize_t index);

/**
 * @brief Set the element at a specific index.
 * @param list Pointer to the array list.
 * @param index Index of the element to set.
 * @param element Pointer to the new element.
 * @param free_element Function pointer to free the old element (can be NULL).
 * @note The old element pointer will no longer be in the list, so if the list owns it, free it using the provided function; otherwise, pass NULL.
 * @return 0 on success, -1 on failure.
 */
int alset(array_list *list, ssize_t index, void* element, void (*free_element) (void*));

/**
 * @brief Append an element to the end of the array list.
 * @param list Pointer to the array list.
 * @param element Pointer to the element to append.
 * @return 0 on success, -1 on failure.
 */
int alappend(array_list *list, void* element);

/**
 * @brief Add an element at a specific index.
 * @param list Pointer to the array list.
 * @param index Index at which to insert the element.
 * @param element Pointer to the element to add.
 * @return 0 on success, -1 on failure.
 */
int aladd(array_list *list, ssize_t index, void* element);

/**
 * @brief Remove the last element from the array list.
 * @param list Pointer to the array list.
 * @param free_element Function pointer to free the element (can be NULL).
 * @note Memory ownership rules in free_array_list apply here.
 * @return 0 on success, -1 on failure.
 */
int alpop(array_list *list, void (*free_element)(void*));

/**
 * @brief Delete the element at a specific index.
 * @param list Pointer to the array list.
 * @param index Index of the element to delete.
 * @param free_element Function pointer to free the element (can be NULL).
 * @note Memory ownership rules in free_array_list apply here.
 * @return 0 on success, -1 on failure.
 */
int aldelete(array_list *list, ssize_t index, void (*free_element)(void*));

