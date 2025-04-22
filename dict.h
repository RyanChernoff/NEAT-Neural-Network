/*
    This is a generic hash dictionary. The user can have any type be the key or entry.
    However, it is up to the user to define the hash function and the equivalence function for te keys.
    They must also define both a key and entry free function so that the dictionary can free its contents.
    
    For copying the dictionary, it makes a deep copy and will require functions for making copies of the
    keys and the entries.
*/
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

/** 
 * @brief creates a new dictionary
 * @param compacity the starting size of the dict
 * @param hash the hash function for keys
 * @param equiv the equivalence function for evaluating if two keys are the same
 * @param key_free the free function for a key
 * @param entry_free the free function for an entry
*/
//Precondition: hash != NULL and equiv != NULL
//Postcondition: Result is not NULL
dict_t dict_new(size_t compacity, key_hash_fn *hash, key_equiv_fn *equiv, key_free_fn key_free, entry_free_fn *entry_free);

/**
 * @brief retrurns the entry associated with key k in dictionary D if it exists or NULL otherwise
 * @param D the dictionary to search through
 * @param k the key to search for
 */
//Precondition: D != NULL
entry dict_get(dict_t D, key k);

/**
 * @brief adds a key-entry pair to a dictionary or updates the entry of key k if it already exists
 * @param D the dictionary to add to
 * @param k the key of the new entry or the key to update
 * @param e the entry to be set to the new key
 */
//Precondition: D != NULL and e != NULL
//Postcondition: dict_get(D, k) != NULL
void dict_add(dict_t D, key k, entry e);

/**
 * @brief removes a key from a dictionary
 * @param D the dictionary to remove from
 * @param k the key to remove
 */
//Precondition: D != NULL and dict_get(D, k) != NULL
//Postcondition: dict_get(D, k) == NULL
entry dict_remove(dict_t D, key k);

/**
 * @brief creates a deep copy of a dictionary
 * @param D the dictionary to copy
 * @param key_copy a function that makes a deep copy of a key
 * @param entry_copy a function that makes a deep copy of an entry
 */
//Precondition: D != NULL, key_copy != NULL, and entry_copy != NULL
//Postcondition: Result is not NULL
dict_t dict_copy(dict_t D, key_copy_fn *key_copy, entry_copy_fn *entry_copy);

/**
 * @brief frees a dictionary and all of its contents.
 * 
 * If either key_free or entry_free are NULL this will not free the keys and entries respectively.
 * 
 * @param D the dict to free
 */
//Precondition: D != NULL
//Postcondition: D is freed
void dict_free(dict_t D);

#endif // DICT_H