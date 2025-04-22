/**
 * This runs multiple iterations of the NEAT algorithm to create and assess multiple generations of 
 * networks.
 */
#ifndef NEAT_H
#define NEAT_H

#include "dna.h"

//Postcondition: Result >= 0
typedef double fit_fn(network_t N);

typedef struct neat_header *neat_t;

/**
 * @brief creates a new instance of NEAT
 * @param size the number of networks in each generation
 * @param input the number of input nodes to each network
 * @param output the number of output nodes of each network
 * @param dist_thresh the minimum distance between two networks to classify as different species
 * @param c1 the weight on distinct genes when comparing networks
 * @param c2 the weight on excess genes when comparing networks
 * @param c3 the weight on total weight distance when comparing networks
 * @param fit the fitness function for evalutating networks
 * @param activation the function to apply to the output of each node in a network
 */
//Precondition: size > 1, input > 0, output > 0, and fit != NULL
//Postcondition: Result is not NULL
neat_t neat_new(size_t size, size_t input, size_t output, double dist_thresh, double c1, double c2, double c3, fit_fn *fit, activation_fn *activation);

/**
 * @brief evaluates all networks in a generation and computes the next generation
 * @param N the NEAT instance to iterate
 */
//Precondition: N != NULL
bool neat_next_gen(neat_t N);

/**
 * @brief returns the best fitness of all networks in the current generation
 * @param N the NEAT instance to query
 */
//Precondition: N != NULL
double neat_best_fitness(neat_t N);

/**
 * @brief returns an array of length n containing the n best fitness of networks in the generation
 * @param N the NEAT instance to query
 * @param n the number of fitnesses to return
 */
//Precondition: N != NULL
double *neat_n_best_fitness(neat_t N, size_t n);

/**
 * @brief returns the fitness of every network in the generation
 * @param N the NEAT instance to query
 */
//Precondition: N != NULL
double *neat_gen_fitness(neat_t N);

/**
 * @brief returns the most fit network
 * @param N the NEAT instance to query
 */
//Don't free result
//Precondition: N != NULL
network_t neat_get_most_fit(neat_t N);

/**
 * @brief returns an array of the n most fit networks
 * @param N the NEAT instance to query
 * @param n the number of networks to return
 */
//Free resulting array, don't free elements of resulting array
//Precondition: N != NULL
network_t *neat_get_n_most_fit(neat_t N, size_t n);

/**
 * @brief returns all networks in the current generation
 * @param N the NEAT instance to query
 */
//Free resulting array, don't free elements of resulting array
//Precondition: N != NULL
network_t *neat_get_gen(neat_t N);

/**
 * @brief computes n generations and returns all resulting networks
 * @param N the NEAT instance to iterate
 * @param n the number of times to iterate
 */
//Free resulting array, don't free elements of resulting array
//Precondition: N != NULL
network_t *neat_get_nth_next_gen(neat_t N, size_t n);

/**
 * @brief frees a neat instance and all networks it directly created
 * @param N the NEAT instance to free
 */
//Precondition: N != NULL
//Postcondition: N is freed
void neat_free(neat_t N);

#endif // NEAT_H