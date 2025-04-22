#include "matrix.h"
#include "dna.h"
#include "NEAT.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double sig(double x){
  return 2.0/(1.0+exp(-4.9*x)) - 1.0;
}

double xor_test(network_t N){
  double score = 4.0;
  double *in = malloc(sizeof(double)*3);
  in[0] = 0.0;
  in[1] = 0.0;
  in[2] = 1.0;
  double *out = network_calc(N, in);
  score -= *out * *out;
  free(out);
  in[0] = 1.0;
  out = network_calc(N, in);
  if(*out < 0) score -= 1.0;
  else score -= (1.0 - *out) * (1.0 - *out);
  free(out);
  in[1] = 1.0;
  out = network_calc(N, in);
  score -= *out * *out;
  free(out);
  in[0] = 0.0;
  out = network_calc(N, in);
  if(*out < 0) score -= 1.0;
  else score -= (1.0 - *out) * (1.0 - *out);
  free(out);
  free(in);
  return score;
}

int main(void) {
  srand(time(NULL));
  neat_t N = neat_new(150, 3, 1, 3.0 , 1.0, 1.0, 0.4, &xor_test, &sig);
  bool b = true;
  double fitt = 0.0;
  for(int i = 0; i < 300 && b && fitt < 3.999; i++) {
    b = neat_next_gen(N);
    fitt = neat_best_fitness(N);
    if(!b || fitt >= 3.999) printf("Generation: %d\n", i + 1);
  }
  double *fit = neat_n_best_fitness(N, 10);
  for(size_t i = 0; i < 10; i++){
    printf("%f\n", fit[i]);
  }

  network_t f = neat_get_most_fit(N);
  double *in = malloc(sizeof(double)*3);
  in[0] = 0.0;
  in[1] = 0.0;
  in[2] = 1.0;
  double *out = network_calc(f, in);
  printf("(0, 0): %f\n", *out);
  free(out);
  in[0] = 1.0;
  out = network_calc(f, in);
  printf("(1, 0): %f\n", *out);
  free(out);
  in[1] = 1.0;
  out = network_calc(f, in);
  printf("(1, 1): %f\n", *out);
  free(out);
  in[0] = 0.0;
  out = network_calc(f, in);
  printf("(0, 1): %f\n", *out);
  free(out);
  free(in);
  free(fit);
  neat_free(N);
  return 0;
}