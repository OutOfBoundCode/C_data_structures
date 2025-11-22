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

/* create_hash_map
 * @brief creat hashmap
 * @param cpy_value a pointer to copy function that do deep copy of the value and return pointer to the copy
 * @param free_value a pointer to free function that takes pointer to the data and free it
 * @return returns pointer to the created hash_map
 * */
hash_map* create_hash_map(hmcpy cpy_value, hmfree_element free_value);

/* free_hash_map
 * @brief free hashmap
 * @param map pointer to the map to be freed
 */
void free_hash_map(hash_map* map);

/* hmadd
 * @brief add key-value to the map
 * @param map pointer to the map to be added to
 * @param key pointer to the key string
 * @param value pointer to the value 
 * @return returns 0 on success, -1 on failure
 */
int hmadd(hash_map* map, char* key, void* value);

/* hmget
 * @brief get the value associated to certain key
 * @param map pointer to the map to be get from
 * @param key pointer to the key string
 * @return returns a pointer to the value, NULL if the key doesn't exist
 */
void* hmget(hash_map* map, char* key);

/* hmdelete
 * @brief delete key-value by passing the key
 * @param map pointer to the map to be deleted from
 * @param key pointer to the key string
 * @return returns 0 on success, -1 on failure
 */
int hmdelete(hash_map* map, char* key);
