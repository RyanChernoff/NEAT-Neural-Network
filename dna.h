/*
    This is the representation of the NEAT networks DNA so that it can be mutated. DNA is designed
    as a linked list of "genes" which can be active or inactive. These genes all represent the connection
    between two nodes and the weight of the connection. The DNA keeps track of the number of active genes
    and the number of dependencies of each gene. It also keeps track of the number of input nodes and the number of output nodes.
 */
#ifndef DNA_H
#define DNA_H

#include "network.h"
#include "inovation_counter.h"

typedef struct dna_header *dna_t;

/**
 * @brief creates a new DNA strand
 * @param input the number of input nodes for the network created by the DNA
 * @param output the number of output nodes for the network created by the DNA
 */
//Precondition: input > 0 and output > 0
//Postcondition: Result is not NULL
dna_t dna_new(size_t input, size_t output);

/**
 * @brief creates a new random connection between two nodes in the network
 * @param D the DNA to add the connection gene to
 * @param I the inovation counter to assign the new gene a unique ID so that it can be tracked for reproduction
 */
//Precondition: D != NULL and I != NULL
void dna_add_connection(dna_t D, inovation_counter_t I);

/**
 * @brief splits a random connection to create a new node (A -> B becomes A -> C -> B)
 * @param D the DNA to add the node to
 * @param I the inocation counter to assign the new gene a unique ID for reproduction purposes
 */
//Precondition: D != NULL and I != NULL
void dna_add_node(dna_t D, inovation_counter_t I);

/**
 * @brief randomly alters the weight of a random connection
 * @param D the DNA to mutate
 */
//Precondition: D != NULL
void dna_mutate_weight(dna_t D);

/**
 * @brief picks a random type of mutation and applies it to the DNA
 * @param D the dna to mutate
 * @param I the inovation counter to keep track of new genes
 */
//Precondition: D != NULL and I != NULL
void dna_mutate(dna_t D, inovation_counter_t I);

/**
 * @brief combines to strands of DNA into one "child"
 * @param dom the dominant parent's DNA
 * @param rec the recesive parent's DNA
 */
//Precondition: dom != NULL and rec != NULL
//Postcondition: Result is not NULL
dna_t dna_combine(dna_t dom, dna_t rec);

/**
 * @brief makes a deep copy of a strand of DNA
 * @param D the DNA to copy
 */
//Precondition: D != NULL
//Postcondition: Result is not NULL
dna_t dna_copy(dna_t D);

/**
 * @brief Converts a strand of DNA into a functional neural network
 * @param D the DNA to convert
 * @param F the activation function to apply to the output of each node in the network
 */
//Precondition: D != NULL
//Postcondition: Result is not NULL
network_t dna_to_network(dna_t D, activation_fn *F);

/**
 * @brief creates a new inovation counter for tracking genes
 * @param compacity the initial compacity of the counter
 */
//Postcondition: Result is not NULL
inovation_counter_t dna_make_inovation_counter(size_t compacity);

/**
 * @brief determines how closely related two strands of DNA are
 * @param D1 the first DNA to compare
 * @param D2 the second DNA to compare
 * @param c1 the weight on the number of distinct genes
 * @param c2 the weight on the number of excess genes
 * @param c3 the weight on the difference in weights of shared genes
 */
//Precondition: D1 != NULL and D2 != NULL
double dna_distance(dna_t D1, dna_t D2, double c1, double c2, double c3);

/**
 * @brief prints DNA
 * @param D the DNA to print
 */
//Precondition: D != NULL
void dna_print(dna_t D);

/**
 * @brief frees a strand of DNA and all of its genes
 * @param D the DNA to free
 */
//Precondition: D != NULL
//Postcondition: D is freed
void dna_free(dna_t D);

#endif