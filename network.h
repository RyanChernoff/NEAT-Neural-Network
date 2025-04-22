/*
    These are network objects which consist of multiple nodes and connection with weights. 
    Network evaluation occurs in linear time with respect to the number of nodes.
*/
#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>

typedef unsigned int vertex;
typedef double activation_fn(double);

typedef struct network_header *network_t;

/**
 * @brief creates a new network
 * @param input the number of input nodes
 * @param output the number of output nodes
 * @param size total number of nodes in the network
 * @param F function to apply to all node output
 */
//Precondition: input > 0, output > 0, size >= input + output
//Postcondition: Result is not NULL
network_t network_new(size_t input, size_t output, size_t size, activation_fn *F);

/**
 * @brief adds a connection between two nodes
 * @param N the network to alter
 * @param start the start vertex in the connection
 * @param end the end vertex of the connection
 * @param weight the weight of the connection
 */
//Precondition: N != NULL and start < end
void network_add_connection(network_t N, vertex start, vertex end, double weight);

/**
 * @brief computes the result of running the network on the given input
 * @param N the network to run
 * @param input the values for the input nodes
 */
//Precondition: N != NULL
//Postcondition: Result is not NULL
double *network_calc(network_t N, double *input);

/**
 * @brief returns a pointer to the activation function of a network
 * @param N the network to query
 */
//Precondition: N != NULL
//Postcondition: Result is not NULL
activation_fn *network_get_activation(network_t N);

/**
 * @brief frees a network
 * @param N the network to free
 */
//Precondition: N != NULL
//Postcondition: N is freed
void network_free(network_t N);

#endif