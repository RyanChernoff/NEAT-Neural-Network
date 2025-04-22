#ifndef INOVATION_COUNTER_H
#define INOVATION_COUNTER_H

#include "dict.h"

typedef unsigned int gene_id;

typedef struct inovation_counter_header *inovation_counter_t;

//Precondition: hash != NULL and equiv != NULL
//Postcondition: Result is not NULL
inovation_counter_t inovation_counter_new(size_t compacity, key_hash_fn *hash, key_equiv_fn *equiv, key_free_fn key_free);

//Precondition: I != NULL
entry inovation_counter_get(inovation_counter_t I, key k);

//Precondition: I != NULL
//Postcondition: inovation_counter_get(I, k) != NULL
gene_id inovation_counter_add(inovation_counter_t I, key k);

//Precondition: I != NULL and inovation_counter_get(I, k) != NULL
//Postcondition: inovation_counter_get(I, k) == NULL
entry inovation_counter_remove(inovation_counter_t I, key k);

//Precondition: I != NULL
//Postcondition: I is freed
void inovation_counter_free(inovation_counter_t I);

#endif