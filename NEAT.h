#ifndef NEAT_H
#define NEAT_H

#include "dna.h"

//Postcondition: Result >= 0
typedef double fit_fn(network_t N);

typedef struct neat_header *neat_t;

//Precondition: size > 1, input > 0, output > 0, and fit != NULL
//Postcondition: Result is not NULL
neat_t neat_new(size_t size, size_t input, size_t output, double dist_thresh, double c1, double c2, double c3, fit_fn *fit, activation_fn *activation);

//Precondition: N != NULL
bool neat_next_gen(neat_t N);

//Precondition: N != NULL
double neat_best_fitness(neat_t N);

//Precondition: N != NULL
double *neat_n_best_fitness(neat_t N, size_t n);

//Precondition: N != NULL
double *neat_gen_fitness(neat_t N);

//Don't free result
//Precondition: N != NULL
network_t neat_get_most_fit(neat_t N);

//Free resulting array, don't free elements of resulting array
//Precondition: N != NULL
network_t *neat_get_n_most_fit(neat_t N, size_t n);

//Free resulting array, don't free elements of resulting array
//Precondition: N != NULL
network_t *neat_get_gen(neat_t N);

//Free resulting array, don't free elements of resulting array
//Precondition: N != NULL
network_t *neat_get_nth_next_gen(neat_t N, size_t n);

//Precondition: N != NULL
//Postcondition: N is freed
void neat_free(neat_t N);

#endif // NEAT_H