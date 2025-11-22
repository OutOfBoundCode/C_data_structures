#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include "hash_map.h"

uint32_t hash_fnv1a(const char *str)
{
    uint32_t hash = 2166136261u;
    unsigned char c;

    while ((c = (unsigned char)*str++))
        hash ^= c, hash *= 16777619u;

    return hash;
}

char* copy_key(const char* str){
    if (str == NULL) return NULL;
    char* cpy_str = malloc((strlen(str) + 1) * sizeof(char));

    if (cpy_str == NULL) return NULL;

    strcpy(cpy_str, str);

    return cpy_str;
}

hnode* create_node(char* key, void* value){
    if (key == NULL) return NULL;

    hnode* nd = malloc(sizeof(hnode));

    if (nd == NULL) return NULL;
    
    nd->key = key;
    nd->value = value;
    nd->next = NULL;

    return nd;
}

hash_map* create_hash_map(hmcpy cpy_value, hmfree_element free_value){
    if (cpy_value == NULL || free_value == NULL) return NULL;

    hash_map* map = malloc(sizeof(hash_map));

    if (map==NULL) return NULL;

    map->length = 0;
    map->max_size = 16;
    map->arr = malloc(map->max_size * sizeof(hnode*));
    
    if (map->arr == NULL) {
        free(map);
        return NULL;
    }

    map->cpy_value = cpy_value;
    map->free_value = free_value;

    for (size_t i = 0; i < map->max_size; i++){
        map->arr[i] = NULL;
    } 

    return map;
}

void free_hash_map(hash_map* map){
    if (map == NULL) return;

    for (size_t i = 0; i < map->max_size; i++){
        if (map->arr[i] == NULL) continue;

        hnode* current = map->arr[i]; 

        while (current != NULL){
            free(current->key);
            map->free_value(current->value);
            hnode* temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(map->arr);
    free(map);
}

int chain_node(hnode* head, hnode* nd){
    if (head == NULL || nd == NULL) return -1;

    hnode* current = head;

    while (current != NULL) {
        if (strcmp(current->key, nd->key) == 0) return -1;

        if (current->next == NULL){
            current->next = nd;
            return 0;
    }
        current = current->next;
    }

    return -1;
}

void rehash_nodes(hash_map* map, hnode** dest, size_t new_size){
    if (map == NULL || dest == NULL) return;

    for (size_t i = 0; i < map->max_size; i++){
        hnode* current = map->arr[i];

        if (current == NULL) continue;

        while (current != NULL){
            size_t new_index = hash_fnv1a(current->key) & (new_size-1);
            hnode* next = current->next;
            current->next = NULL;
            if (dest[new_index] == NULL)
                dest[new_index] = current;
            
            else
                chain_node(dest[new_index], current);

            current = next;
        }
    }
}

void resize_map(hash_map* map, double factor){
    size_t new_size = map->max_size * factor;
    hnode** new_arr = malloc(new_size * sizeof(hnode*));
    
    if (new_arr == NULL) return;

    for (size_t i = 0; i < new_size; i++){
        new_arr[i] = NULL;
    }

    rehash_nodes(map, new_arr, new_size);
    
    hnode** temp = map->arr;
    map->arr = new_arr;
    map->max_size = new_size;
    free(temp);
}

int hmadd(hash_map* map, char* key, void* value){
    if (map == NULL || key == NULL) return -1;

    char* key_cpy = copy_key(key);
    void* value_cpy = map->cpy_value(value);

    if (key_cpy == NULL || value_cpy == NULL){
        if (key_cpy) free(key_cpy);
        if (value_cpy) map->free_value(value_cpy);
        return -1;
}

    hnode* nd = create_node(key_cpy, value_cpy);
    
    if (nd == NULL){
        free(key_cpy);
        map->free_value(value_cpy);
        return -1;
    }

    size_t index = hash_fnv1a(key) & (map->max_size-1);

    if (map->arr[index] == NULL)
        map->arr[index] = nd;

    else
    {
        if (chain_node(map->arr[index], nd) == -1){
            free(key_cpy);
            map->free_value(value_cpy);
            free(nd);
            return -1;
        }
    }
    map->length++;

    if ((double) map->length / map->max_size > 0.7) resize_map(map, 2);

    return 0;
}

void* hmget(hash_map* map, char* key){
    if (map == NULL || key == NULL) return NULL;

    size_t index = hash_fnv1a(key) & (map->max_size-1);

    hnode* current = map->arr[index];

    while (current != NULL){
        if (strcmp(current->key, key) == 0) return current->value;
        current = current->next;
    }

    return NULL;
}

int hmdelete(hash_map *map, char *key){
    if (map == NULL || key == NULL) return -1;
    
    size_t index = hash_fnv1a(key) & (map->max_size-1);

    hnode* current = map->arr[index];
    hnode* prenode = NULL;

    while (current != NULL){
        if (strcmp(current->key, key) == 0) {
            
            if (prenode == NULL) map->arr[index] = current->next;
            else prenode->next = current->next;

            free(current->key);
            map->free_value(current->value);
            free(current);

            map->length--;

            if (map->max_size > 16 && (double) map->length / map->max_size < 0.25) resize_map(map, 0.5);

            return 0;
        }
        prenode = current;
        current = current->next;
    }

    return -1;
}
