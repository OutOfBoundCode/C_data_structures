#pragma once

#include <sys/types.h>

typedef void* (*alcpy) (void*);
typedef void (*alfree_element) (void*); 
typedef int (*alcompare) (void*, void*); 

/**
 * @brief Array list structure.
 */
typedef struct {
    ssize_t length;      /**< Number of elements currently in the list */
    ssize_t max_size;    /**< Maximum capacity of the array */
    void** arr;          /**< Pointer to the array of element pointers */
    alcpy cpy;  // a pointer to a copy function that takes a void pointer and return a pointer of the copy of the memory
    alfree_element free_element; // a pointer to a free function that takes a void pointer and free the pointed to memory
    alcompare compare;  // a pointer to compare function that takes two element and return 0 on success (the two element match) and -1 on failure.

} array_list;

/**
 * @brief Create a new array list with a specified initial size.
 * @param array_size Initial capacity of the array list.
 * @note if the initial size is <=0 the size of the array will be defaulted to 10
 * @param cpy a pointer to a copy function that takes void* and return a void pointer to a copy of the memory on success, and return NULL on failure 
 * @param free_element a pointer to a free function that takes a void pointer and free the pointed memory
 * @param compare Function pointer to compare the passed element with the list elements and returning the index when there is a match.
 * @note compare passed function should return 0 on success (the two element match) and -1 on failure.
 * @note the size of the array doubles when full
 * @return Pointer to the newly created array list, or NULL on failure.
 */
array_list* create_array_list(ssize_t array_size, alcpy cpy, alfree_element free_element, alcompare compare);

/**
 * @brief Free the array list and its elements.
 * @param list Pointer to the array list.
*/
void free_array_list(array_list *list);

/**
 * @brief Get the index of an element in the array list.
 * @param list Pointer to the array list.
 * @param element Pointer to the element to find.
 * @return Index of the element, or -1 if not found.
 */
ssize_t alget_index(const array_list *list, void* element);

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
 * @return 0 on success, -1 on failure.
 */
int alset(array_list *list, ssize_t index, void* element);

/**
 * @brief Append an element to the end of the array list.
 * @param list Pointer to the array list.
 * @param element Pointer to the element to append.
 * @return 0 on success, -1 on failure.
 * @note when the length (the count of elements inside) of the array equals the max_size the array doubles its size
 */
int alappend(array_list *list, void* element);

/**
 * @brief Add an element at a specific index.
 * @param list Pointer to the array list.
 * @param index Index at which to insert the element.
 * @param element Pointer to the element to add.
 * @return 0 on success, -1 on failure.
 * @note when the length (the count of elements inside) of the array equals the max_size the array doubles its size
 */
int aladd(array_list *list, ssize_t index, void* element);

/**
 * @brief Remove the last element from the array list.
 * @param list Pointer to the array list.
 * @return 0 on success, -1 on failure.
 * @note when the length (the count of elements inside) of the array is <= 1/4 max_size the array shrinks to half its size
 */
int alpop(array_list *list);

/**
 * @brief Delete the element at a specific index.
 * @param list Pointer to the array list.
 * @param index Index of the element to delete.
 * @return 0 on success, -1 on failure.
 * @note when the length (the count of elements inside) of the array is <= 1/4 max_size the array shrinks to half its size
 */
int aldelete(array_list *list, ssize_t index);

