/**
 * The inovation counter tracks the different genes that show up across multiple iterations of NEAT.
 * Each new gene is assigned a unique ID and added to a dictionary.
 */
#ifndef INOVATION_COUNTER_H
#define INOVATION_COUNTER_H

#include "dict.h"

typedef unsigned int gene_id;

typedef struct inovation_counter_header *inovation_counter_t;

/**
 * @brief creates a new inovation counter
 * @param compacity the initial size of the counter
 * @param hash the hash function for the genes
 * @param equiv the equivalence function for the genes
 * @param key_free the free function for genes
 */
//Precondition: hash != NULL and equiv != NULL
//Postcondition: Result is not NULL
inovation_counter_t inovation_counter_new(size_t compacity, key_hash_fn *hash, key_equiv_fn *equiv, key_free_fn key_free);

/**
 * @brief gets the ID of a given key
 * @param I the counter to get the ID from
 * @param k the key to get the ID of
 */
//Precondition: I != NULL
entry inovation_counter_get(inovation_counter_t I, key k);

/**
 * @brief adds a key to the counter with a new ID
 * @param I the counter to add to
 * @param k the key to add
 */
//Precondition: I != NULL
//Postcondition: inovation_counter_get(I, k) != NULL
gene_id inovation_counter_add(inovation_counter_t I, key k);

/**
 * @brief removes a key from the counter
 * @param I the counter to remove from
 * @param k the key to remove
 */
//Precondition: I != NULL and inovation_counter_get(I, k) != NULL
//Postcondition: inovation_counter_get(I, k) == NULL
entry inovation_counter_remove(inovation_counter_t I, key k);

/**
 * @brief frees an inovation counter and all keys in it. 
 * 
 * If key_free was NULL this does not free the keys.
 * 
 * @param I the inovation counter to free
 */
//Precondition: I != NULL
//Postcondition: I is freed
void inovation_counter_free(inovation_counter_t I);

#endif