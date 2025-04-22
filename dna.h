#ifndef DNA_H
#define DNA_H

#include "network.h"
#include "inovation_counter.h"

typedef struct dna_header *dna_t;

//Precondition: input > 0 and output > 0
//Postcondition: Result is not NULL
dna_t dna_new(size_t input, size_t output);

//Precondition: D != NULL and I != NULL
void dna_add_connection(dna_t D, inovation_counter_t I);

//Precondition: D != NULL and I != NULL
void dna_add_node(dna_t D, inovation_counter_t I);

//Precondition: D != NULL
void dna_mutate_weight(dna_t D);

//Precondition: D != NULL and I != NULL
void dna_mutate(dna_t D, inovation_counter_t I);

//Precondition: dom != NULL and rec != NULL
//Postcondition: Result is not NULL
dna_t dna_combine(dna_t dom, dna_t rec);

//Precondition: D != NULL
//Postcondition: Result is not NULL
dna_t dna_copy(dna_t D);

//Precondition: D != NULL
//Postcondition: Result is not NULL
network_t dna_to_network(dna_t D, activation_fn *F);

//Postcondition: Result is not NULL
inovation_counter_t dna_make_inovation_counter(size_t compacity);

//Precondition: D1 != NULL and D2 != NULL
double dna_distance(dna_t D1, dna_t D2, double c1, double c2, double c3);

//Precondition: D != NULL
void dna_print(dna_t D);

//Precondition: D != NULL
//Postcondition: D is freed
void dna_free(dna_t D);

#endif