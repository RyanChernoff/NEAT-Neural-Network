#ifndef DICT_H
#define DICT_H

#include <stdlib.h>
#include <stdbool.h>

typedef void *entry;
typedef void *key;

typedef unsigned int key_hash_fn(key k);
typedef bool key_equiv_fn(key k1, key k2);
typedef void key_free_fn(key k);
typedef void entry_free_fn(entry k);
typedef key key_copy_fn(key k);
typedef entry entry_copy_fn(entry k);

typedef struct dict_header *dict_t;

//Precondition: hash != NULL and equiv != NULL
//Postcondition: Result is not NULL
dict_t dict_new(size_t compacity, key_hash_fn *hash, key_equiv_fn *equiv, key_free_fn key_free, entry_free_fn *entry_free);

//Precondition: D != NULL
entry dict_get(dict_t D, key k);

//Precondition: D != NULL and e != NULL
//Postcondition: dict_get(D, k) != NULL
void dict_add(dict_t D, key k, entry e);

//Precondition: D != NULL and dict_get(D, k) != NULL
//Postcondition: dict_get(D, k) == NULL
entry dict_remove(dict_t D, key k);

//Precondition: D != NULL, key_copy != NULL, and entry_copy != NULL
//Postcondition: Result is not NULL
dict_t dict_copy(dict_t D, key_copy_fn *key_copy, entry_copy_fn *entry_copy);

//Precondition: D != NULL
//Postcondition: D is freed
void dict_free(dict_t D);

#endif // DICT_H