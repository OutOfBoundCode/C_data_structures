#pragma once

typedef struct tnode{
    void* value;
    struct tnode* lnode; // left node (the smaller)
    struct tnode* rnode; // right node (the bigger)
} tnode;

typedef struct bst {
    tnode* root;
    int (*compare)(void*, void*); // compare 2 parameters, returns positive when the first is bigger, negative when the first is smaller wise, 0 when they are equal
    void (*free_element)(void*);
    void* (*cpy) (void*);
} bst;

enum DIRECTION{RIGHT, LEFT, NONE};

/* create_bst
 * @brief binary search tree constructor 
 * @param cpy a pointer to copy pointer that return void and takes void*, it should allocate a heap memory and copy the specific data to be inserted (the data pointed by the passed pointer), it's used internally to make deep copies of passed data.
 * @param free_element a pointer to free function that takes pointer to the data and free it
 * @param compare a pointer to custom compare function that takes two void pointers return poistive if the first element is bigger, negative if it's smaller, 0 if both element are equal
 * @return it returns a pointer to the created binary search tree
 * */
bst* create_bst(void* (*cpy)(void*), void (*free_element)(void*), int (*compare)(void*, void*));

/* free_bst
 * @brief frees the memory of the bst
 * @param tree a pointer to the to-be-freed bst
 * */
void free_bst(bst* tree);

/* bstprint
 * @brief function that print the elements of the tree in the form of sorted array
 * @param tree a pointer to the bst to be printed
 * @param print_element a pointer to print function that takes void pointer of an element and print it
 * */
void bstprint(bst* tree, void (*print_element)(void*));

/* bstadd
 * @brief function that adds elements to the bst
 * @param tree a pointer to the bst to be added to
 * @param element a pointer to the element to be added (null is refused)
 * @return returns 0 on success, -1 on failure
 **/
int bstadd(bst* tree, void *element);

/* insert_ordered_list
 * @brief function that inserts an ordered list to the tree in a balanced way
 * @param tree a pointer to the bst to be inserted to
 * @param elements an array of elements to be inserted (should be sorted)
 * @param start an index of the first to-be-inserted element in the array
 * @param end an index of the last to-be-inserted element in the array
 **/
void insert_ordered_list(bst *tree , void* elements[], size_t start, size_t end);

/* bstdelete
 * @brief delete an element from the bst
 * @param tree a pointer to the bst from which the element will be deleted
 * @param element the element to be deleted
 * @return returns 0 on success, -1 on failure
 **/
int bstdelete(bst* tree, void* element);

/* bstsearch
 * @brief search whether certain element is in the tree
 * @param tree a pointer to the bst to search in
 * @param element a pointer for the element to be searched for
 * @return returns 0 on success, -1 on failure
 **/
int bstsearch(bst* tree, void* element);
