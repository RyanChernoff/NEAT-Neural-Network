#include <stdlib.h>

typedef unsigned int vertex;
typedef double activation_fn(double);

typedef struct node_header node;
struct node_header{
  double weight;
  vertex v;
  node *next;
};

struct network_header{
  size_t input;
  size_t output;
  size_t size;
  node **data;
  activation_fn *F;
};
typedef struct network_header network;

double default_activation_fn(double x){
  return x;
}

network *network_new(size_t input, size_t output, size_t size, activation_fn *F){
  network *N = malloc(sizeof(network));
  N->input = input;
  N->output = output;
  N->size = size;
  N->F = F == NULL ? &default_activation_fn : F;
  N->data = calloc(size, sizeof(node*));
  return N;
}

// can't add same connection twice
void network_add_connection(network *N, vertex start, vertex end, double weight){
  node *n = malloc(sizeof(node));
  n->weight = weight;
  n->v = end;
  n->next = N->data[start];
  N->data[start] = n;
}

double *network_calc(network *N, double *input){
  double *weights = calloc(N->size, sizeof(double));
  double *output = malloc(N->output*sizeof(double));
  for(size_t i = 0; i < N->input; i++){
    weights[i] = input[i];
  }
  for(size_t i = 0; i < N->size - N->output; i++){
    node *n = N->data[i];
    while(n != NULL){
      weights[n->v] += (*(N->F))(n->weight * weights[i]);
      n = n->next;
    }
  }
  for(size_t i = 0; i < N->output; i++){
    output[i] = (*(N->F))(weights[N->size - N->output + i]);
  }
  free(weights);
  return output;
}

activation_fn *network_get_activation(network *N){
  return N->F;
}

void network_free(network *N){
  for(size_t i = 0; i < N->size; i++){
    node *n = N->data[i];
    while(n != NULL){
      node *next = n->next;
      free(n);
      n = next;
    }
  }
  free(N->data);
  free(N);
}

typedef network *network_t;