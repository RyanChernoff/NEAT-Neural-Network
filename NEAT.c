#include "dna.h"

typedef double fit_fn(network_t N);

struct individual_header {
  dna_t dna;
  network_t net;
  double fit;
};
typedef struct individual_header individual;

typedef struct species_header species;
struct species_header {
  dna_t dna;
  double fit;
  size_t stag_count;
  species *next;
};

typedef unsigned int species_id;

struct species_list_header {
  species *start;
  species *end;
  size_t num_species;
};
typedef struct species_list_header species_list;

struct neat_header {
  size_t size; // > 1
  double dist_thresh;
  double c1;
  double c2;
  double c3;
  individual **individuals;
  species_list *species;
  inovation_counter_t counter;
  fit_fn *fit;
};
typedef struct neat_header neat;

void sort_individuals(individual **individuals, size_t lo, size_t hi) {
  if (lo == hi || lo == hi - 1)
    return;

  size_t pivot = lo + (hi - lo) / 2;
  size_t start = pivot + 1;
  size_t i = lo;
  while (i < pivot) {
    if (individuals[i]->fit < individuals[pivot]->fit) {
      individual *temp = individuals[i];
      individuals[i] = individuals[pivot - 1];
      individuals[pivot - 1] = individuals[pivot];
      individuals[pivot] = temp;
      pivot--;
    } else {
      i++;
    }
  }

  for (size_t j = start; j < hi; j++) {
    if (individuals[j]->fit >= individuals[pivot]->fit) {
      individual *temp = individuals[j];
      individuals[j] = individuals[pivot + 1];
      individuals[pivot + 1] = individuals[pivot];
      individuals[pivot] = temp;
      pivot++;
    }
  }

  sort_individuals(individuals, lo, pivot);
  sort_individuals(individuals, pivot + 1, hi);
}

size_t num_same_species(neat *N, dna_t D) {
  size_t count = 0;
  for (size_t i = 0; i < N->size; i++) {
    if (dna_distance(D, N->individuals[i]->dna, N->c1, N->c2, N->c3) <
        N->dist_thresh) {
      count++;
    }
  }
  return count;
}

species_id get_species(species_list *list, dna_t D, double c1, double c2,
                       double c3, double dist_thresh) {
  species_id id = 0;
  species *S = list->start;
  while (S != NULL) {
    if (dna_distance(S->dna, D, c1, c2, c3) < dist_thresh)
      return id;
    id++;
    S = S->next;
  }
  return id;
}

species_list *get_new_species_list(neat *N) {
  species_list *list = malloc(sizeof(species_list));
  species *S = malloc(sizeof(species));
  S->dna = dna_copy(N->individuals[0]->dna);
  S->fit = N->individuals[0]->fit;
  S->stag_count = 0;
  S->next = NULL;
  list->start = S;
  list->end = S;
  list->num_species = 1;
  for (size_t i = 0; i < N->size; i++) {
    species_id id = get_species(list, N->individuals[i]->dna, N->c1, N->c2,
                                N->c3, N->dist_thresh);
    if (id == list->num_species) {
      S = malloc(sizeof(species));
      S->dna = dna_copy(N->individuals[i]->dna);
      S->fit = N->individuals[i]->fit;
      S->stag_count = 0;
      S->next = NULL;
      list->end->next = S;
      list->end = S;
      list->num_species++;
    }
  }
  return list;
}

unsigned int species_hash(key k) { return *((species_id *)k); }

bool species_equiv(key k1, key k2) {
  return *((species_id *)k1) == *((species_id *)k2);
}

void species_list_free(species_list *list) {
  species *S = list->start;
  while (S != NULL) {
    species *next = S->next;
    dna_free(S->dna);
    free(S);
    S = next;
  }
  free(list);
}

neat *neat_new(size_t size, size_t input, size_t output, double dist_thresh,
               double c1, double c2, double c3, fit_fn *fit,
               activation_fn *activation) {
  neat *N = malloc(sizeof(neat));
  N->size = size;
  N->individuals = malloc(size * sizeof(individual *));
  N->counter = dna_make_inovation_counter(size);
  for (size_t i = 0; i < size; i++) {
    individual *I = malloc(sizeof(individual));
    I->dna = dna_new(input, output);
    dna_mutate(I->dna, N->counter);
    I->net = dna_to_network(I->dna, activation);
    I->fit = (*fit)(I->net);
    N->individuals[i] = I;
  }
  N->dist_thresh = dist_thresh;
  N->c1 = c1;
  N->c2 = c2;
  N->c3 = c3;
  N->fit = fit;
  sort_individuals(N->individuals, 0, N->size);
  N->species = get_new_species_list(N);
  return N;
}

double neat_best_fitness(neat *N) { return N->individuals[0]->fit; }

double *neat_n_best_fitness(neat *N, size_t n) {
  double *fitness = malloc(n * sizeof(double));
  for (size_t i = 0; i < n; i++) {
    fitness[i] = N->individuals[i]->fit;
  }
  return fitness;
}

double *neat_gen_fitness(neat *N) { return neat_n_best_fitness(N, N->size); }

// no need to free network
network_t neat_get_most_fit(neat *N) { return N->individuals[0]->net; }

// need to free array but not elements
network_t *neat_get_n_most_fit(neat *N, size_t n) {
  network_t *gen = malloc(n * sizeof(network_t));
  for (size_t i = 0; i < n; i++) {
    gen[i] = N->individuals[i]->net;
  }
  return gen;
}

// need to free array but not elements
network_t *neat_get_gen(neat *N) { return neat_get_n_most_fit(N, N->size); }

bool neat_next_gen(neat *N) {
  dict_t species_dict = dict_new(N->species->num_species, &species_hash,
                                 &species_equiv, &free, NULL);
  size_t num_old_species = N->species->num_species;
  species **old_species = malloc(num_old_species * sizeof(species *));
  species *temp = N->species->start;
  for (size_t i = 0; i < num_old_species; i++) {
    old_species[i] = temp;
    temp = temp->next;
  }
  
  species_id *id = malloc(sizeof(species_id));
  for (size_t i = 0; i < N->size; i++) {
    *id = get_species(N->species, N->individuals[i]->dna, N->c1, N->c2, N->c3,
                      N->dist_thresh);
    species_list *list = (species_list *)dict_get(species_dict, id);
    if (list != NULL) {
      list->end->next = malloc(sizeof(species));
      list->end = list->end->next;
      list->end->dna = N->individuals[i]->dna;
      list->end->fit = N->individuals[i]->fit;
      list->end->stag_count = 0;
      list->end->next = NULL;
      list->num_species++;
    } else {
      list = malloc(sizeof(species_list));
      list->start = malloc(sizeof(species));
      list->end = list->start;
      list->num_species = 1;

      list->end->dna = N->individuals[i]->dna;
      list->end->fit = N->individuals[i]->fit;
      list->end->stag_count = 0;
      list->end->next = NULL;

      dict_add(species_dict, (key)id, (entry)list);
      
      if (*id == N->species->num_species) {
        N->species->end->next = malloc(sizeof(species));
        N->species->end = N->species->end->next;
        N->species->end->dna = dna_copy(N->individuals[i]->dna);
        N->species->end->fit = N->individuals[i]->fit;
        N->species->end->stag_count = 0;
        N->species->end->next = NULL;
        N->species->num_species++;
      }
      id = malloc(sizeof(species_id));
    }
  }

  species_list **species_groups =
      malloc(N->species->num_species * sizeof(species_list *));
  double *fitness = malloc(N->species->num_species * sizeof(double));
  double total_fitness = 0;
  for (size_t i = 0; i < N->species->num_species; i++) {
    *id = i;
    species_list *list = (species_list *)dict_get(species_dict, id);
    species_groups[i] = list;
    if(list == NULL){
      fitness[i] = 0;
      continue;
    }
    
    double total = 0;
    species *S = list->start;
    while (S != NULL) {
      total += S->fit;
      S = S->next;
    }
    fitness[i] = total / (double)list->num_species;
    if (i < num_old_species) {
      if (old_species[i]->stag_count >= 15)
        fitness[i] = 0;
      else if (fitness[i] - old_species[i]->fit < 0)
        old_species[i]->stag_count++;
      else {
        old_species[i]->fit = fitness[i];
        old_species[i]->stag_count = 0;
      }
    }
    total_fitness += fitness[i];
  }
  free(id);
  dict_free(species_dict);
  free(old_species);

  if (total_fitness == 0) {
    free(fitness);
    for (size_t i = 0; i < num_old_species; i++) {
      if(species_groups[i] == NULL) continue;
      temp = species_groups[i]->start;
      while (temp != NULL) {
        species *next = temp->next;
        free(temp);
        temp = next;
      }
      free(species_groups[i]);
    }
    free(species_groups);
    return false;
  }

  size_t *num_offspring = malloc(N->species->num_species * sizeof(size_t));
  size_t rem = N->size;
  size_t last = 0;
  for (size_t i = 0; i < N->species->num_species; i++) {
    num_offspring[i] = (size_t)(fitness[i] * ((double)N->size) / total_fitness);
    rem -= num_offspring[i];
    if(num_offspring[i] != 0) last = i;
  }
  num_offspring[last] += rem;
  free(fitness);

  individual **next_gen = malloc(N->size * sizeof(individual *));
  size_t index = 0;
  for (size_t i = 0; i < N->species->num_species; i++) {
    if (num_offspring[i] == 0)
      continue;

    temp = species_groups[i]->start;
    size_t num_parents = species_groups[i]->num_species < 5
                             ? species_groups[i]->num_species
                             : species_groups[i]->num_species / 2;
    for (size_t j = 0; j < num_offspring[i]; j++) {
      individual *I = malloc(sizeof(individual));
      if (j % num_parents != 0) {
        I->dna = dna_combine(temp->dna, temp->next->dna);
        temp = temp->next;
      } else {
        I->dna = dna_combine(temp->dna, species_groups[i]->start->dna);
        temp = species_groups[i]->start;
      }
      if (j > 0 || species_groups[i]->num_species < 5)
        dna_mutate(I->dna, N->counter);
      I->net = dna_to_network(I->dna,
                              network_get_activation(N->individuals[0]->net));
      I->fit = (*N->fit)(I->net);
      next_gen[index] = I;
      index++;
    }
  }

  for (size_t i = 0; i < N->size; i++) {
    network_free(N->individuals[i]->net);
    free(N->individuals[i]);
  }
  free(N->individuals);
  N->individuals = next_gen;
  sort_individuals(N->individuals, 0, N->size);

  num_old_species = N->species->num_species;
  species *prev = NULL;
  temp = N->species->start;
  for (size_t i = 0; i < num_old_species; i++) {
    if (num_offspring[i] == 0) {
      if (prev != NULL) {
        prev->next = temp->next;
        if(prev->next == NULL) N->species->end = prev;
      }
      else N->species->start = temp->next;
      species *dead = temp;
      temp = temp->next;
      dna_free(dead->dna);
      free(dead);
      N->species->num_species--;
    } else {
      species_list *group = species_groups[i];
      size_t member = rand() % group->num_species;
      species *S = group->start;
      for (size_t j = 0; j < member; j++)
        S = S->next;
      dna_free(temp->dna);
      temp->dna = dna_copy(S->dna);
      prev = temp;
      temp = temp->next;
    }
  }
  free(num_offspring);

  for (size_t i = 0; i < num_old_species; i++){
    if(species_groups[i] != NULL) species_list_free(species_groups[i]);
  }
  free(species_groups);

  return true;
}

// need to free array but not elements
network_t *neat_get_nth_next_gen(neat *N, size_t n) {
  for (size_t i = 0; i < n; i++) {
    if (!neat_next_gen(N))
      break;
  }
  return neat_get_gen(N);
}

void neat_free(neat *N) {
  for (size_t i = 0; i < N->size; i++) {
    dna_free(N->individuals[i]->dna);
    network_free(N->individuals[i]->net);
    free(N->individuals[i]);
  }
  free(N->individuals);

  species_list_free(N->species);

  inovation_counter_free(N->counter);
  free(N);
}