#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>

typedef unsigned int vertex;
typedef double activation_fn(double);

typedef struct network_header *network_t;

//Precondition: input > 0, output > 0, size >= input + output
//Postcondition: Result is not NULL
network_t network_new(size_t input, size_t output, size_t size, activation_fn *F);

//Precondition: N != NULL and start < end
void network_add_connection(network_t N, vertex start, vertex end, double weight);

//Precondition: N != NULL
//Postcondition: Result is not NULL
double *network_calc(network_t N, double *input);

//Precondition: N != NULL
//Postcondition: Result is not NULL
activation_fn *network_get_activation(network_t N);

//Precondition: N != NULL
//Postcondition: N is freed
void network_free(network_t N);

#endif