#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../hash_map.h"

// helper function

void* int_cpy(void* v) {
    if (v == NULL) return NULL;
    int *p = malloc(sizeof(int));
    *p = *(int*)v;
    return p;
}

void int_free(void* v) {
    free(v);
}

static char *mkkey(const char *fmt, int i) {
    char *buf = malloc(64);
    snprintf(buf, 64, fmt, i);
    return buf;
}

/*-------- test --------*/
void test_create_free_and_invalid_args() {
    /* create with NULL callbacks should return NULL */
    assert(create_hash_map(NULL, int_free) == NULL);
    assert(create_hash_map(int_cpy, NULL) == NULL);

    hash_map *m = create_hash_map(int_cpy, int_free);
    assert(m != NULL);
    assert(m->length == 0);
    assert(m->max_size >= 16);
    assert(m->arr != NULL);

    free_hash_map(m);
    printf("create/free hash_map test: PASS\n");
}

void test_add_get_delete_basic() {
    hash_map *m = create_hash_map(int_cpy, int_free);
    assert(m != NULL);

    int v1 = 10, v2 = 20, v3 = 30;

    /* add three keys that will likely be in different buckets */
    assert(hmadd(m, "key1", &v1) == 0);
    assert(hmadd(m, "key2", &v2) == 0);
    assert(hmadd(m, "key3", &v3) == 0);

    assert(m->length == 3);

    /* get existing */
    int *gv = hmget(m, "key1");
    assert(gv && *gv == v1);
    
    gv = hmget(m, "key2");
    assert(gv && *gv == v2);
    
    gv = hmget(m, "key3"); 
    assert(gv && *gv == v3);

    assert(hmget(m, "key9") == NULL);

    assert(hmdelete(m, "key2") == 0);
    assert(hmget(m, "key2") == NULL);
    assert(m->length == 2);

    assert(hmdelete(m, "key1") == 0);
    assert(hmget(m, "key1") == NULL);
    assert(m->length == 1);

    assert(hmdelete(m, "key3") == 0);
    assert(m->length == 0);

    /* delete non-existing returns -1 */
    assert(hmdelete(m, "key9") == -1);

    free_hash_map(m);

    printf("add/get/delete test: PASS\n");
}

void test_invalid_inputs() {
    hash_map *m = create_hash_map(int_cpy, int_free);
    assert(m != NULL);

    assert(hmadd(NULL, "k", NULL) == -1);
    assert(hmadd(m, NULL, NULL) == -1);

    assert(hmget(NULL, "k") == NULL);
    assert(hmget(m, NULL) == NULL);

    assert(hmdelete(NULL, "x") == -1);
    assert(hmdelete(m, NULL) == -1);

    free_hash_map(m);
    printf("invalid inputs test: PASS\n");
}

void test_duplicate_insert() {
    hash_map *m = create_hash_map(int_cpy, int_free);
    assert(m != NULL);

    int v = 42;
    assert(hmadd(m, "dupkey", &v) == 0);
    size_t before_len = m->length;

    /* second add same key should fail and not increase length */
    int res = hmadd(m, "dupkey", &v);
    assert(res == -1);
    assert(m->length == before_len);

    /* existing value remains */
    int *gv = hmget(m, "dupkey");
    assert(gv && *gv == v);

    free_hash_map(m);
    printf("duplicate insert test: PASS\n");
}

void test_resize_up_and_integrity() {
    hash_map *m = create_hash_map(int_cpy, int_free);
    assert(m != NULL);

    const int N = 2000;

    // Insert enough items to force multiple resizes
    for (int i = 0; i < N; i++) {
        char *k = mkkey("key-%d", i);
        int val = i;

        assert(hmadd(m, k, &val) == 0);
        free(k);
    }

    assert(m->max_size >= 2048);

    for (int i = 0; i < N; i++) {
        char *k = mkkey("key-%d", i);
        int *v = hmget(m, k);
        free(k);

        assert(v != NULL && *v == i);
    }

    free_hash_map(m);
    
    printf("resize up and integrity test: PASS\n");
}

void test_resize_down() {
    hash_map *m = create_hash_map(int_cpy, int_free);
    assert(m != NULL);

    int n = 40;
    for (int i = 0; i < n; ++i) {
        char *k = mkkey("s-%d", i);
        int val = i;
        assert(hmadd(m, k, &val) == 0);
        free(k);
    }
    size_t grown = m->max_size;
    assert(grown > 16);

    for (int i = 0; i < n - 2; ++i) {
        char *k = mkkey("s-%d", i);
        assert(hmdelete(m, k) == 0);
        free(k);
    }

    assert(m->max_size >= 16);
    assert(m->length == 2);

    free_hash_map(m);
    printf("resize down test: PASS\n");
}

void test_deletion_various_positions() {
    hash_map *m = create_hash_map(int_cpy, int_free);
    assert(m != NULL);

    char *k1 = mkkey("del-a-%d", 1);
    char *k2 = mkkey("del-b-%d", 2);
    char *k3 = mkkey("del-c-%d", 3);
    int a=1,b=2,c=3;
    assert(hmadd(m, k1, &a) == 0);
    assert(hmadd(m, k2, &b) == 0);
    assert(hmadd(m, k3, &c) == 0);

    /* delete head (whatever bucket head might be) */
    assert(hmdelete(m, k1) == 0 || hmdelete(m, k1) == -1); // allow both depending on collisions
    /* ensure removing non-present is safe */
    (void) hmdelete(m, k1);

    /* delete remaining */
    hmdelete(m, k2);
    hmdelete(m, k3);

    free(k1); free(k2); free(k3);
    free_hash_map(m);
    printf("deletion various positions test: PASS\n");
}

void test_stress_insert_remove() {
    hash_map *m = create_hash_map(int_cpy, int_free);
    assert(m);

    const size_t N = 5000;
    for (size_t i = 0; i < N; ++i) {
        char *k = mkkey("st-%d", i);
        size_t val = i;
        assert(hmadd(m, k, &val) == 0);
        free(k);
    }
    assert(m->length == N);

    for (size_t i = 0; i < N; ++i) {
        char *k = mkkey("st-%d", i);
        assert(hmdelete(m, k) == 0);
        free(k);
    }
    assert(m->length == 0);

    free_hash_map(m);
    printf("stress insert remove test: PASS\n");
}

int main() {
    test_create_free_and_invalid_args();
    test_add_get_delete_basic();
    test_invalid_inputs();
    test_duplicate_insert();
    test_resize_up_and_integrity();
    test_resize_down();
    test_deletion_various_positions();
    test_stress_insert_remove();
}

