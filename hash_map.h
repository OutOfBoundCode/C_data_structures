#pragma once

#include <stddef.h>

typedef void* (*hmcpy) (void*);
typedef void (*hmfree_element) (void*); 

typedef struct hnode {
    char* key;
    void* value;
    struct hnode* next;
} hnode;

typedef struct { 
    hnode** arr;
    size_t length;
    size_t max_size;
    hmcpy cpy_value;
    hmfree_element free_value;
} hash_map;

hash_map* create_hash_map(hmcpy cpy_value, hmfree_element free_value);

void free_hash_map(hash_map* map);

int hmadd(hash_map* map, char* key, void* value);

void* hmget(hash_map* map, char* key);

int hmdelete(hash_map* map, char* key);
