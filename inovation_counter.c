#include "dict.h"

typedef unsigned int gene_id;

struct inovation_counter_header{
  gene_id counter;
  dict_t D;
};
typedef struct inovation_counter_header inovation_counter;

inovation_counter *inovation_counter_new(size_t compacity, key_hash_fn *hash, key_equiv_fn *equiv, key_free_fn key_free){
  inovation_counter *I = malloc(sizeof(inovation_counter));
  I->counter = 0;
  I->D = dict_new(compacity, hash, equiv, key_free, &free);
  return I;
}

gene_id inovation_counter_add(inovation_counter *I, key k){
  gene_id *id = malloc(sizeof(gene_id));
  *id = I->counter;
  I->counter++;
  dict_add(I->D, k, (entry)id);
  return *id;
}

entry inovation_counter_get(inovation_counter *I, key k){
  return dict_get(I->D, k);
}

entry inovation_counter_remove(inovation_counter *I, key k){
  return dict_remove(I->D, k);
}

void inovation_counter_free(inovation_counter *I){
  dict_free(I->D);
  free(I);
}