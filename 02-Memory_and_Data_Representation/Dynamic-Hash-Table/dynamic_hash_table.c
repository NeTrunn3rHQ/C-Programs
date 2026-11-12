/*
 * Dynamic Hash Table
 *
 * A string-to-integer hash table implemented with separate chaining.
 * The table grows automatically when its load factor reaches 0.75.
 *
 * Return convention:
 *   0 = success
 *   1 = failure (invalid argument, duplicate key, missing key, or allocation failure)
 *
 * Build:
 *   gcc -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow \
 *       -Wstrict-prototypes -O2 -g 05_dynamic_hash_table.c -o hash_table
 *
 * Run:
 *   ./hash_table
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HT_INITIAL_CAPACITY 4U
#define HT_MAX_LOAD_FACTOR  0.75

typedef struct node {
    char *key;
    int value;
    struct node *next;
} Node;

typedef struct {
    Node **buckets;
    size_t capacity;
    size_t size;
} HashTable;

/* Create a node and make an owned copy of key. */
static Node *create_node(const char *key, int value)
{
    Node *new_node = malloc(sizeof(*new_node));

    if (new_node == NULL) {
        return NULL;
    }

    size_t key_length = strlen(key) + 1U;
    new_node->key = malloc(key_length);

    if (new_node->key == NULL) {
        free(new_node);
        return NULL;
    }

    memcpy(new_node->key, key, key_length);
    new_node->value = value;
    new_node->next = NULL;

    return new_node;
}

/*
 * djb2 string hash.
 * Unsigned arithmetic is used intentionally; overflow wraps around.
 * The caller must provide a non-zero capacity.
 */
static size_t hash_key(const char *key, size_t capacity)
{
    size_t hash_value = 5381U;

    for (size_t i = 0U; key[i] != '\0'; ++i) {
        hash_value = (hash_value * 33U) + (unsigned char)key[i];
    }

    return hash_value % capacity;
}

/* Initialize an empty table with the requested bucket count. */
int ht_init(HashTable *ht, size_t initial_capacity)
{
    if (ht == NULL || initial_capacity == 0U) {
        return 1;
    }

    ht->buckets = NULL;
    ht->capacity = 0U;
    ht->size = 0U;

    ht->buckets = calloc(initial_capacity, sizeof(*ht->buckets));
    if (ht->buckets == NULL) {
        return 1;
    }

    ht->capacity = initial_capacity;
    return 0;
}

/*
 * Rebuild the bucket array and re-link existing nodes.
 * Node keys and values are not copied or reallocated.
 */
int ht_resize(HashTable *ht, size_t new_capacity)
{
    if (ht == NULL || ht->buckets == NULL ||
        ht->capacity == 0U || new_capacity == 0U) {
        return 1;
    }

    Node **new_buckets = calloc(new_capacity, sizeof(*new_buckets));
    if (new_buckets == NULL) {
        return 1;
    }

    for (size_t i = 0U; i < ht->capacity; ++i) {
        Node *current = ht->buckets[i];

        while (current != NULL) {
            Node *next = current->next;
            size_t index = hash_key(current->key, new_capacity);

            current->next = new_buckets[index];
            new_buckets[index] = current;
            current = next;
        }
    }

    free(ht->buckets);
    ht->buckets = new_buckets;
    ht->capacity = new_capacity;

    return 0;
}

/*
 * Insert a new key/value pair.
 * Duplicate keys are rejected; existing values are not overwritten.
 *
 * If the load-factor-triggered resize fails, the insertion remains
 * successful and the table remains valid; it will simply keep its
 * current capacity until a later resize succeeds.
 */
int ht_insert(HashTable *ht, const char *key, int value)
{
    if (ht == NULL || ht->buckets == NULL ||
        ht->capacity == 0U || key == NULL) {
        return 1;
    }

    size_t index = hash_key(key, ht->capacity);
    Node *current = ht->buckets[index];
    Node *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return 1;
        }

        previous = current;
        current = current->next;
    }

    Node *new_node = create_node(key, value);
    if (new_node == NULL) {
        return 1;
    }

    if (previous == NULL) {
        ht->buckets[index] = new_node;
    } else {
        previous->next = new_node;
    }

    ++ht->size;

    if ((double)ht->size / (double)ht->capacity >= HT_MAX_LOAD_FACTOR &&
        ht->capacity <= SIZE_MAX / 2U) {
        (void)ht_resize(ht, ht->capacity * 2U);
    }

    return 0;
}

/*
 * Find key and optionally write its value to *value.
 * Pass NULL for value if only existence needs to be checked.
 */
int ht_search(const HashTable *ht, const char *key, int *value)
{
    if (ht == NULL || ht->buckets == NULL ||
        ht->capacity == 0U || key == NULL) {
        return 1;
    }

    size_t index = hash_key(key, ht->capacity);
    Node *current = ht->buckets[index];

    while (current != NULL && strcmp(current->key, key) != 0) {
        current = current->next;
    }

    if (current == NULL) {
        return 1;
    }

    if (value != NULL) {
        *value = current->value;
    }

    return 0;
}

/* Remove a key and release its node and owned key string. */
int ht_delete(HashTable *ht, const char *key)
{
    if (ht == NULL || ht->buckets == NULL ||
        ht->capacity == 0U || key == NULL) {
        return 1;
    }

    size_t index = hash_key(key, ht->capacity);
    Node *current = ht->buckets[index];
    Node *previous = NULL;

    while (current != NULL && strcmp(current->key, key) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return 1;
    }

    if (previous == NULL) {
        ht->buckets[index] = current->next;
    } else {
        previous->next = current->next;
    }

    free(current->key);
    free(current);
    --ht->size;

    return 0;
}

/* Release all nodes and reset the table to an empty state. */
void ht_destroy(HashTable *ht)
{
    if (ht == NULL) {
        return;
    }

    if (ht->buckets != NULL) {
        for (size_t i = 0U; i < ht->capacity; ++i) {
            Node *current = ht->buckets[i];

            while (current != NULL) {
                Node *next = current->next;
                free(current->key);
                free(current);
                current = next;
            }
        }

        free(ht->buckets);
    }

    ht->buckets = NULL;
    ht->capacity = 0U;
    ht->size = 0U;
}

/* Basic tests for insertion, collisions, resizing, search, and deletion. */
int main(void)
{
    HashTable ht = {0};
    int value = 0;

    assert(ht_init(&ht, HT_INITIAL_CAPACITY) == 0);
    assert(ht_init(NULL, HT_INITIAL_CAPACITY) == 1);
    assert(ht_init(&ht, 0U) == 1);

    /* These keys collide at the initial capacity with this hash function. */
    assert(ht_insert(&ht, "a", 10) == 0);
    assert(ht_insert(&ht, "i", 20) == 0);
    assert(ht_insert(&ht, "q", 30) == 0);
    assert(ht_insert(&ht, "bob", 40) == 0);

    /* The load factor reaches 0.75, so the table should grow from 4 to 8. */
    assert(ht.capacity == 8U);
    assert(ht.size == 4U);

    /* Duplicate keys are rejected without changing the stored value. */
    assert(ht_insert(&ht, "a", 999) == 1);
    assert(ht_search(&ht, "a", &value) == 0 && value == 10);
    assert(ht.size == 4U);

    /* Search existing and missing keys. */
    assert(ht_search(&ht, "i", &value) == 0 && value == 20);
    assert(ht_search(&ht, "q", &value) == 0 && value == 30);
    assert(ht_search(&ht, "bob", &value) == 0 && value == 40);
    assert(ht_search(&ht, "missing", &value) == 1);
    assert(ht_search(&ht, "a", NULL) == 0);

    /* Delete a middle, tail, and head node from a collision chain. */
    assert(ht_delete(&ht, "i") == 0);
    assert(ht_search(&ht, "i", &value) == 1);

    assert(ht_delete(&ht, "q") == 0);
    assert(ht_search(&ht, "q", &value) == 1);

    assert(ht_delete(&ht, "a") == 0);
    assert(ht_search(&ht, "a", &value) == 1);

    assert(ht_search(&ht, "bob", &value) == 0 && value == 40);
    assert(ht.size == 1U);

    assert(ht_delete(&ht, "missing") == 1);
    assert(ht_resize(&ht, 16U) == 0);
    assert(ht_search(&ht, "bob", &value) == 0 && value == 40);

    ht_destroy(&ht);
    ht_destroy(&ht); /* Destroying an already-destroyed table is safe. */

    assert(ht.buckets == NULL);
    assert(ht.capacity == 0U);
    assert(ht.size == 0U);

    puts("ALL TESTS PASSED!");
    return 0;
}
