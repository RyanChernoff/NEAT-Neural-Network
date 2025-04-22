#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "network.h"
#include "dict.h"
#include "inovation_counter.h"

typedef unsigned int priority_t;

typedef struct gene_header gene;
struct gene_header{
  gene_id id;
  vertex start;
  vertex end;
  double weight;
  bool active;
  gene *next;
};

enum node_type_header{
  END,
  HIDDEN
};
typedef enum node_type_header node_type;

struct cgene_header{
  vertex start;
  vertex end;
};
typedef struct cgene_header cgene;

struct dna_header{
  gene *start;
  gene *end;
  dict_t priority;
  size_t num_genes;
  size_t num_active_genes;
  size_t size;
  size_t input;
  size_t output;
};
typedef struct dna_header dna;

//helper funcions

size_t min(size_t a, size_t b){
  if(a < b) return a;
  return b;
}

void *uint_copy(void *i){
  unsigned int *p = malloc(sizeof(unsigned int));
  *p = *((unsigned int *)i);
  return p;
}

double rand_norm(){
  double sum = 0;
  for(char i = 0; i < 10; i++){
    sum += (double)rand() / (double)RAND_MAX;
  }
  return sum - 5;
}

unsigned int vertex_hash(key k){
  return *((vertex *)k);
}

bool vertex_equiv(key k1, key k2){
  return *((vertex *)k1) == *((vertex *)k2);
}

unsigned int cgene_hash(key k){
  cgene *C = (cgene *)k;
  return 65537 * C->start + C->end;
}

bool cgene_equiv(key k1, key k2){
  cgene *c1 = (cgene *)k1;
  cgene *c2 = (cgene *)k2;
  return c1->start == c2->start && c1->end == c2->end;
}

cgene *gene_to_cgene(dna *D, gene *G){
  cgene *C = malloc(sizeof(cgene));
  C->start = G->start;
  C->end = G->end;
  return C;
}

gene *gene_copy(gene* G){
  gene *new_gene = malloc(sizeof(gene));
  new_gene->id = G->id;
  new_gene->start = G->start;
  new_gene->end = G->end;
  new_gene->weight = G->weight;
  new_gene->active = G->active;
  new_gene->next = NULL;
  return new_gene;
}

gene *dna_get_active_gene(dna *D){
  unsigned int mutation = rand() % D->num_active_genes;
  gene *G = D->start;
  unsigned int i = 0;
  while(i < mutation || !G->active){
    if(G->active) i++;
    G = G->next;
  }
  return G;
}

bool dna_has_connection(dna *D, vertex start, vertex end, inovation_counter_t I){
  cgene *C = malloc(sizeof(cgene));
  C->start = start;
  C->end = end;
  gene_id *id = (gene_id *)inovation_counter_get(I, (key) C);
  if(id != NULL){
    gene *G = D->start;
    while(G != NULL){
      if(G->id >= *id){
        free(C);
        if(G->id == *id) return true;
        return false;
      }
      G = G->next;
    }
  }
  free(C);
  return false;
}

void dna_add_gene(dna *D, gene *G){
  D->num_genes++;
  D->num_active_genes++;
  if(D->start == NULL){
    D->start = G;
    D->end = G;
    return;
  }
  if(D->start->id > G->id){
    G->next = D->start;
    D->start = G;
    return;
  }
  if(D->end->id < G->id){
    D->end->next = G;
    D->end = G;
    return;
  }
  gene *prev = D->start;
  while(prev->next != NULL){
    if(prev->next->id > G->id){
      G->next = prev->next;
      prev->next = G;
      return;
    }
    prev = prev->next;
  }
}

gene *dna_make_gene(dna *D, vertex start, vertex end, double weight, inovation_counter_t I){
  gene *G = malloc(sizeof(gene));
  G->start = start;
  G->end = end;
  G->weight = weight;
  G->active = true;
  G->next = NULL;

  key k = (key)gene_to_cgene(D, G);
  gene_id *id = (gene_id *)inovation_counter_get(I, k);
  if(id == NULL) {
    G->id = inovation_counter_add(I, k);
  } else{
    G->id = *id;
    free(k);
  }
  return G;
}

priority_t dna_incr_priority(dna *D, vertex v, bool add){
  vertex *k = malloc(sizeof(vertex));
  *k = v;
  priority_t *e = (priority_t *)dict_get(D->priority, (key) k);
  priority_t p = *e;
  
  for(vertex i = 0; i < D->size; i++){
    *k = i;
    e = (priority_t *)dict_get(D->priority, (key) k);
    if(*e >= p){
      if(add) (*e)++;
      else (*e)--;
    }
  }
  free(k);
  return p;
}
//end helper functions

dna *dna_new(size_t input, size_t output){
  dna *D = malloc(sizeof(dna));
  D->start = NULL;
  D->end = NULL;
  D->num_active_genes = 0;
  D->num_genes = 0;
  D->size = input + output;
  D->input = input;
  D->output = output;
  D->priority = dict_new(D->size*2, &vertex_hash, &vertex_equiv, &free, &free);
  for(vertex i = 0; i < D->size; i++){
    vertex *k = malloc(sizeof(vertex));
    priority_t *e = malloc(sizeof(priority_t));
    *k = i;
    *e = i;
    dict_add(D->priority, (key) k, (entry) e);
  }
  return D;
}

void dna_add_connection(dna *D, inovation_counter_t I){
  vertex start = rand() % (D->size - D->output);
  vertex end = D->input;
  if(start >= D->input){
    if(start < D->input + D->output) start += D->output;
    end += (rand() % (D->size - D->input - 1));
    if(start == end) end = D->size - 1;
  } else{
    end += (rand() % (D->size - D->input));
  }
  if(dna_has_connection(D, start, end, I)) return;
  
  gene *G = dna_make_gene(D, start, end, ((double)rand() * 2.0 / (double)RAND_MAX) - 1.0, I);
  
  dna_add_gene(D, G);

  vertex *v1 = malloc(sizeof(vertex));
  vertex *v2 = malloc(sizeof(vertex));
  *v1 = start;
  *v2 = end;
  priority_t *p1 = (priority_t *)dict_get(D->priority, (key) v1);
  priority_t *p2 = (priority_t *)dict_get(D->priority, (key) v2);

  if(*p1 > *p2){
    priority_t new_end = dna_incr_priority(D, start, true);
    dna_incr_priority(D, end, false);
    *p2 = new_end;
  }

  //Fix cycles
  gene *fix = D->start;
  while(fix != NULL){
    *v1 = fix->start;
    *v2 = fix->end;
    priority_t *p1 = (priority_t *)dict_get(D->priority, (key) v1);
    priority_t *p2 = (priority_t *)dict_get(D->priority, (key) v2);
    if(*p1 > *p2){
      fix->active = false;
      D->num_active_genes--;
    }
    fix = fix->next;
  }
  
  free(v1);
  free(v2);
}

void dna_add_node(dna *D, inovation_counter_t I){
  gene *G = dna_get_active_gene(D);
  G->active = false;
  D->num_active_genes--;
  
  gene *G1 = dna_make_gene(D, G->start, D->size, 1.0, I);
  gene *G2 = dna_make_gene(D, D->size, G->end, G->weight, I);
  
  dna_add_gene(D, G1);
  dna_add_gene(D, G2);
  
  vertex *k = malloc(sizeof(vertex));
  *k = D->size;

  priority_t *e = malloc(sizeof(priority_t));
  vertex end = G->end;
  if(end < D->input + D->output) end = D->input;
  *e = dna_incr_priority(D, end, true);

  dict_add(D->priority, (key) k, (entry) e);
  
  D->size++;
}

void dna_mutate_weight(dna *D){
  gene *G = D->start;
  double change = rand_norm();
  if(fabs(change) < 0.001) change = 0.001;
  while(G != NULL){
    unsigned int mutation = rand() % 10;
    if(mutation < 9){
      G->weight += change;
    } else G->weight = ((double)rand()) / ((double)RAND_MAX);
    G = G->next;
  }
}

void dna_mutate(dna *D, inovation_counter_t I){
  if(D->num_active_genes == 0){
    dna_add_connection(D, I);
    dna_add_connection(D, I);
    return;
  }
  if(rand() % 10 < 8) dna_mutate_weight(D);
  if(rand() % 20 == 0) dna_add_connection(D, I);
  if(rand() % 100 < 3) dna_add_node(D, I);
}

dna *dna_combine(dna *dom, dna *rec){
  dna *D = malloc(sizeof(dna));
  D->start = NULL;
  D->end = NULL;
  D->num_active_genes = 0;
  D->num_genes = 0;
  D->size = dom->size;
  D->input = dom->input;
  D->output = dom->output;
  D->priority = dict_copy(dom->priority, &uint_copy, &uint_copy);
  
  gene *G1 = dom->start;
  gene *G2 = rec->start;
  while(G1 != NULL){
    gene *new_gene = gene_copy(G1);
    
    while(G2 != NULL && G2->id < G1->id) G2 = G2->next;
    
    if(G2 != NULL && G2->id == G1->id) {
      if(rand() % 2 == 0) new_gene->weight = G2->weight;
      if(!G1->active || !G2->active){
        if(rand() % 4 == 0) new_gene->active = true;
        else new_gene->active = false;
      }
    }

    D->num_genes++;
    if(new_gene->active) D->num_active_genes++;
    
    if(D->start == NULL) {
      D->start = new_gene;
      D->end = new_gene;
    } else{
      D->end->next = new_gene;
      D->end = new_gene;
    }
      
    G1 = G1->next;
  }
  return D;
}

dna *dna_copy(dna *D){
  dna *new_dna = malloc(sizeof(dna));
  new_dna->input = D->input;
  new_dna->output = D->output;
  new_dna->num_active_genes = D->num_active_genes;
  new_dna->num_genes = D->num_genes;
  new_dna->size = D->size;
  new_dna->priority = dict_copy(D->priority, &uint_copy, &uint_copy);
  if(new_dna->num_genes == 0){
    new_dna->start = NULL;
    new_dna->end = NULL;
    return new_dna;
  }
  new_dna->start = gene_copy(D->start);
  new_dna->end = new_dna->start;
  gene *G = D->start->next;
  while(G != NULL){
    new_dna->end->next = gene_copy(G);
    new_dna->end = new_dna->end->next;
    G = G->next;
  }
  return new_dna;
}

network_t dna_to_network(dna *D, activation_fn *F){
  network_t N = network_new(D->input, D->output, D->size, F);
  gene *G = D->start;
  vertex *start = malloc(sizeof(vertex));
  vertex *end = malloc(sizeof(vertex));
  while(G != NULL){
    if(G->active) {
      *start = G->start;
      *end = G->end;
      priority_t *pstart = (priority_t *)dict_get(D->priority, (key) start);
      priority_t *pend = (priority_t *)dict_get(D->priority, (key) end);
      network_add_connection(N, *pstart, *pend, G->weight);
    }
    G = G->next;
  }
  free(start);
  free(end);
  return N;
}

inovation_counter_t dna_make_inovation_counter(size_t compacity){
  return inovation_counter_new(compacity, &cgene_hash, &cgene_equiv, &free);
}

double dna_distance(dna *D1, dna *D2, double c1, double c2, double c3){
  size_t dis = 0;
  size_t exc = 0;
  double weight = 0;
  double num = (double)min(D1->num_genes, D2->num_genes);
  gene *G1 = D1->start;
  gene *G2 = D2->start;
  while(G1 != NULL || G2 != NULL){
    if(G1 == NULL){
      exc++;
      G2 = G2->next;
    } else if(G2 == NULL){
      exc++;
      G1 = G1->next;
    } else if(G1->id == G2->id){
      weight += fabs(G1->weight - G2->weight);
      G1 = G1->next;
      G2 = G2->next;
    } else if(G1->id < G2->id){
      dis++;
      G1 = G1->next;
    } else{
      dis++;
      G2 = G2->next;
    }
  }
  return c1*((double)dis)/num + c2*((double)exc)/num + c3*weight;
}

void dna_print(dna *D){
  gene *G = D->start;
  while(G != NULL){
    printf("gene %d: %d -> %d\t\tActive: %d\t\tWeight: %f\n", G->id, G->start, G->end, G->active ? 1 : 0, G->weight);
    G = G->next;
  }
}

void dna_free(dna *D){
  gene *G = D->start;
  while(G != NULL){
    gene *next = G->next;
    free(G);
    G = next;
  }
  dict_free(D->priority);
  free(D);
}