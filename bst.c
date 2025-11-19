#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

tnode* create_node(void* value){
    if (value == NULL) return NULL;

    tnode* n = malloc(sizeof(tnode));

    if (n == NULL) return NULL;

    n->value = value;
    n->rnode = NULL;
    n->lnode= NULL;

    return n;
}

bst* create_bst(void* (*cpy)(void*), void (*free_element)(void*), int (*compare)(void*, void*)){
    if (cpy == NULL || compare == NULL || free_element == NULL) return NULL;

    bst* tree = malloc(sizeof(bst));

    if (tree == NULL) return NULL;

    tree->cpy = cpy;
    tree->compare = compare;
    tree->root = NULL;
    tree->free_element = free_element;

    return tree;
}

void free_node(tnode* root,  void (*free_element)(void*)){
    if (root == NULL) return;

    free_node(root->lnode, free_element);
    free_node(root->rnode, free_element);

    free_element(root->value);

    free(root);
}

void free_bst(bst* tree){
    if (tree == NULL) return;
    
    free_node(tree->root, tree->free_element);

    free(tree);
}

void print_node(tnode* root, void (*print_element)(void*)){
    if (root == NULL || print_element == NULL) return;
    
    print_node(root->lnode, print_element);
    print_element(root->value);
    printf(", ");
    print_node(root->rnode, print_element);
}

void bstprint(bst* tree, void (*print_element)(void*)){
    if (tree == NULL || tree->root == NULL || print_element == NULL) {
        printf("[]\n");
        return;
    }
    printf("[");
    print_node(tree->root, print_element);
    printf("\b\b]\n");
}

tnode* search_node(tnode* root, int (*compare)(void*, void*), void* value){
    if (root == NULL || compare == NULL || value == NULL) return NULL;

    int sign = compare(value, root->value);

    if (sign == 0) return root;
    
    if (sign > 0) return search_node(root->rnode, compare, value);
    
    if (sign < 0) return search_node(root->lnode, compare, value);

    return NULL;
}

tnode* search_parent_node(tnode* root, int (*compare)(void*, void*), void* value){
    if (root == NULL || compare == NULL || value == NULL) return NULL;
    
    if (root->rnode != NULL) 
        if (compare(value, root->rnode->value) == 0) 
            return root;

    if (root->lnode != NULL)
        if (compare(value, root->lnode->value) == 0) 
            return root;

    int sign = compare(value, root->value);

    if (sign == 0) return NULL;
    
    if (sign > 0) return search_parent_node(root->rnode, compare, value);
    
    if (sign < 0) return search_parent_node(root->lnode, compare, value);

  return NULL;
}


int bstsearch(bst* tree, void* element){
    if (tree == NULL || element == NULL) return -1;
    return (search_node(tree->root, tree->compare, element) != NULL? 0 : -1);
}

int bstadd(bst* tree, void *element){
    if (tree == NULL || element == NULL) return -1;

    void* element_cpy = tree->cpy(element);

    if (tree->root == NULL){
         tree->root = create_node(element_cpy);
         if (tree->root == NULL) {
            tree->free_element(element_cpy);
            return -1;
        }
     return 0;

    }

    tnode* current = tree->root;

    while (current != NULL){

        int sign = tree->compare(element, current->value);
        
        if (sign == 0) {
            tree->free_element(element_cpy);
            return -1;
        } 
        
        if (sign > 0){
            if (current->rnode == NULL){
                current->rnode = create_node(element_cpy);
                
                if (current->rnode == NULL) {
                tree->free_element(element_cpy);
                return -1;
                }
                return 0;
            }
                current = current->rnode;
        }

        if (sign < 0){ 
            if (current->lnode == NULL){
                current->lnode = create_node(element_cpy);
            if (current->lnode == NULL) {
                tree->free_element(element_cpy);
                return -1;
            }
            return 0;
    }
            current = current->lnode;
        }
                }
}


void insert_ordered_list(bst *tree , void* elements[], size_t start, size_t end){
    if (tree == NULL || elements == NULL) return;

    if (start > end) return;

    size_t middle_index = (start + end) / 2;

    bstadd(tree, elements[middle_index]);
    
    if (middle_index > 0)
        insert_ordered_list(tree, elements, start, middle_index - 1);

    insert_ordered_list(tree, elements, middle_index + 1, end);
}

tnode* find_min(tnode* root){
    if (root == NULL) return NULL;
    
    tnode* min = root;

    while(root != NULL){
        min = root;
        root = root->lnode;
    }

    return min;
}

enum DIRECTION get_child_direction(tnode* parent, tnode* child){
    if (parent == NULL) return NONE;
    if (parent->rnode == child) return RIGHT;
    if (parent->lnode == child) return LEFT;

    return NONE;
}

int bstdelete(bst* tree, void* element){
    if (tree == NULL || element == NULL) return -1;

    tnode* deleted_node= search_node(tree->root, tree->compare, element);
    tnode* parent_node = search_parent_node(tree->root, tree->compare, element);
    
    if (deleted_node == NULL) return -1;

    if (parent_node == NULL){
        tnode* new_root;
        if (deleted_node->lnode == NULL && deleted_node->rnode == NULL) new_root = NULL;

        else if (deleted_node->lnode == NULL) new_root = tree->root->rnode;

        else if (deleted_node->rnode == NULL) new_root = tree->root->lnode;
    
        else {
            tnode* min = find_min(deleted_node->rnode);
            tnode* min_parent = search_parent_node(tree->root, tree->compare, min->value);
            
            tnode* min_right_node = min->rnode;

            tree->free_element(tree->root->value);
            tree->root->value = min->value;
            
            enum DIRECTION direction = get_child_direction(min_parent, min);

            switch (direction) {
                case LEFT: min_parent->lnode = min_right_node;break;
                case RIGHT: min_parent->rnode = min_right_node;break;
                default: return -1;
    }        

            free(min);

            return 0;
        }

        tree->free_element(deleted_node->value);
        free(deleted_node);
        tree->root = new_root;

        return 0;
    }

    enum DIRECTION direction = get_child_direction(parent_node, deleted_node); // tells wich one of the parents children the deleted node is
     
    if (deleted_node->lnode == NULL && deleted_node->rnode == NULL) {
        switch (direction) {
            case LEFT: parent_node->lnode = NULL; break;
            case RIGHT: parent_node->rnode = NULL; break;
            default: return -1;
        }

        tree->free_element(deleted_node->value);
        free(deleted_node);

        return 0;
    }

    if (deleted_node->lnode == NULL){
        switch (direction) {
            case LEFT: parent_node->lnode = deleted_node->rnode;break;
            case RIGHT: parent_node->rnode = deleted_node->rnode;break;
            default: return -1;
        }
        
        tree->free_element(deleted_node->value);
        free(deleted_node);

        return 0;

}
    if (deleted_node->rnode == NULL){
        switch (direction) {
            case LEFT: parent_node->lnode = deleted_node->lnode;break;
            case RIGHT: parent_node->rnode = deleted_node->lnode;break;
            default: return -1;
        }
        
        tree->free_element(deleted_node->value);
        free(deleted_node);

        return 0;
}

    tnode* min = find_min(deleted_node->rnode);
    tnode* min_parent = search_parent_node(tree->root, tree->compare, min->value);
    tnode* min_right_node = min->rnode;

    direction = get_child_direction(min_parent, min);

    switch (direction) {
            case LEFT: min_parent->lnode = min_right_node;break;
            case RIGHT: min_parent->rnode = min_right_node;break;
            default: return -1;
    }        

    tree->free_element(deleted_node->value);
    deleted_node->value = min->value;
            
    free(min);

    return 0;
}
